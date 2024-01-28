#ifndef GAME_H
#define GAME_H

#include "board.h"

/* Represents the game state */
class Game
{
public:
    // Constructor
    Game();

    // Destructor
    ~Game();

    Board &getBoard();

    void startGame();

    // void printPGN() const;

    void promptMove();

private:
    unsigned short int move;  // Stores move
    Move *prevMove = nullptr; // Stores last move
    Board board;              // Stores board
};

#endif