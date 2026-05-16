#include <iostream>

#include "Map.h"

using namespace std;

void draw(char board[9])
{
    system("cls");

    cout << board[0] << " | "
        << board[1] << " | "
        << board[2] << '\n';

    cout << "--+---+--\n";

    cout << board[3] << " | "
        << board[4] << " | "
        << board[5] << '\n';

    cout << "--+---+--\n";

    cout << board[6] << " | "
        << board[7] << " | "
        << board[8] << '\n';
}