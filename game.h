#ifndef GAME_H
#define GAME_H

#include "board.h"

/* Represents the game state */
class Game
{
public:
    Game();

    Board &getBoard();

    // void startGame();

    // void printPGN() const;

    // void promptMove() const;

private:
    unsigned short int move;  // Stores move
    Move *lastMove = nullptr; // Stores last move
    Board board;              // Stores board
};

#endif