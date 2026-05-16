#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <winsock2.h>
#include <string>

#include "../Shared/Map.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int clientCode()
{
    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

    SOCKET clientSocket = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(54000);



    string ipAddress;
    cout << "Enter server IP: ";
    cin >> ipAddress;



    serverAddr.sin_addr.s_addr =
        inet_addr(ipAddress.c_str()); // .c_str changes cpp string to c string



    if (connect(
        clientSocket,
        (sockaddr*)&serverAddr,
        sizeof(serverAddr)
    ) == SOCKET_ERROR)
    {
        cout << "Connection failed\n";
        closesocket(clientSocket);
        WSACleanup();
        system("pause");

        return 1;
    }

    // determines player1 or player2
    int myIndex;

    recv(
        clientSocket,
        (char*)&myIndex,
        sizeof(myIndex),
        0
    );

    // gameLoop
    while (true)
    {
        GameState gameState;

        // recv gameState
        recv(
            clientSocket,
            (char*)&gameState,
            sizeof(gameState),
            0
        );

        draw(gameState.board);

        // gameover state
        if (gameState.gameOver)
        {
            cout << "\nGame Over\n";
            break;
        }


        if (myIndex == gameState.currentTurn)
        {
            int move;
            while (true)
            {
                cout << "\nYour turn: ";
                cin >> move;

                if (
                    cin.fail() ||
                    move < 1 ||
                    move > 9
                    )
                {
                    cin.clear();
                    cin.ignore(1000, '\n'); // ignore 1000 chars until newline (\n) is found

                    cout << "Invalid input\n";
                }
                else
                {
                    break;
                }
            }

            // send move to server
            send(
                clientSocket,
                (char*)&move,
                sizeof(move),
                0
            );
        }
        else
        {
            cout << "\nWaiting for opponent...\n";
        }
    }

    closesocket(clientSocket);
    WSACleanup();
    system("pause");

    return 0;
}

int main()
{
    clientCode();
}