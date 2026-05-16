#pragma once

struct GameState
{
    char board[9];

    int currentTurn;

    bool gameOver;
};

void draw(char board[9]);