#ifndef BOARD_H
#define BOARD_H

#include "common.h"
#include "pieces.h"

/* Forward declarations */
class Move;
class Board;

/* Class the contains all information about the chess board */
class Board
{
public:
    // Constructor
    Board();

    // Destructor
    ~Board();

    // Displays board
    void displayBoard() const;

    // Gets which player's turn it is
    char getTurn() const;

    // Used to request board to make move
    Move *requestMove(const std::string &notation, Move *prevMove);

    // Makes move
    void makeMove(Move *newMove);

private:
    char turn;         // Stores which player's turn it is
    Square grid[8][8]; // Stores grid
};

#endif