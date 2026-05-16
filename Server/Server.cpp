#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>

#include "../Shared/Map.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct Player
{
    SOCKET socket;

    char symbol; // X or O
};

vector<Player> players;

char board[9] =
{
    '1', '2', '3',
    '4', '5', '6',
    '7', '8', '9'
};

int currentTurn = 0;

bool gameOver = false;



bool checkWin(char symbol)
{
    // rows
    if (
        board[0] == symbol &&
        board[1] == symbol &&
        board[2] == symbol
        ) return true;

    if (
        board[3] == symbol &&
        board[4] == symbol &&
        board[5] == symbol
        ) return true;

    if (
        board[6] == symbol &&
        board[7] == symbol &&
        board[8] == symbol
        ) return true;

    // cols
    if (
        board[0] == symbol &&
        board[3] == symbol &&
        board[6] == symbol
        ) return true;

    if (
        board[1] == symbol &&
        board[4] == symbol &&
        board[7] == symbol
        ) return true;

    if (
        board[2] == symbol &&
        board[5] == symbol &&
        board[8] == symbol
        ) return true;

    // diagonals
    if (
        board[0] == symbol &&
        board[4] == symbol &&
        board[8] == symbol
        ) return true;

    if (
        board[2] == symbol &&
        board[4] == symbol &&
        board[6] == symbol
        ) return true;

    return false;
}

// to check whether we can mark the cell or not
bool checkDraw()
{
    for (int i = 0; i < 9; i++)
    {
        if (board[i] != 'X' && board[i] != 'O')
        {
            return false;
        }
    }

    return true;
}



void sendBoard()
{
    GameState gameState;

    // update state machine
    for (int i = 0; i < 9; i++)
    {
        gameState.board[i] = board[i];
    }
    gameState.currentTurn = currentTurn;
    gameState.gameOver = gameOver;

    // send state to all players
    for (int i = 0; i < players.size(); i++)
    {
        send(
            players[i].socket,
            (char*)&gameState,
            sizeof(gameState),
            0
        );
    }
}



int serverCode()
{
    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

    SOCKET serverSocket = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );

    sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(54000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(
        serverSocket,
        (sockaddr*)&serverAddr,
        sizeof(serverAddr)
    ) == SOCKET_ERROR)
    {
        cout << "Bind failed\n";

        return 1;
    }

    listen(serverSocket, 2);

    cout << "Waiting for players...\n";

    // display local IP
    char hostName[256];
    gethostname(
        hostName,
        sizeof(hostName)
    );

    addrinfo hints = {};

    hints.ai_family = AF_INET;

    addrinfo* result;

    getaddrinfo(
        hostName,
        nullptr,
        &hints,
        &result
    );

    sockaddr_in* addr =
        (sockaddr_in*)result->ai_addr;

    char ip[INET_ADDRSTRLEN];

    inet_ntop(
        AF_INET,
        &addr->sin_addr,
        ip,
        sizeof(ip)
    );

    cout << "Server IP: "
        << ip
        << "\n\n";

    freeaddrinfo(result);

    // making clientSocket
    for (int i = 0; i < 2; i++)
    {
        SOCKET clientSocket = accept(
            serverSocket,
            nullptr,
            nullptr
        );

        if (clientSocket == INVALID_SOCKET)
        {
            cout << "Accept failed\n";

            return 1;
        }

        Player newPlayer;

        newPlayer.socket = clientSocket;

        if (i == 0)
            newPlayer.symbol = 'X'; // 1st connection = X
        else
            newPlayer.symbol = 'O'; // 2nd = O

        // add to players list
        players.push_back(newPlayer);

        send(
            clientSocket,
            (char*)&i,
            sizeof(i),
            0
        );

        cout << "Player connected\n";
    }



    sendBoard();



    while (!gameOver)
    {
        Player& currentPlayer = players[currentTurn];

        int move;

        recv(
            currentPlayer.socket,
            (char*)&move,
            sizeof(move),
            0
        );

        move--;

        if (
            move >= 0 &&
            move < 9 &&

            board[move] != 'X' &&
            board[move] != 'O'
            )
        {
            board[move] =
                currentPlayer.symbol;

            if (checkWin(currentPlayer.symbol))
            {
                gameOver = true;

                sendBoard();

                cout << currentPlayer.symbol
                    << " wins!\n";

                break;
            }



            if (checkDraw())
            {
                gameOver = true;

                sendBoard();

                cout << "Draw!\n";

                break;
            }



            currentTurn =
                1 - currentTurn;

            sendBoard();
        }
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}

int main()
{
    serverCode();
}