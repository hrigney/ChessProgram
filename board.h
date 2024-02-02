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
    Piece *makeMove(Move *newMove);

    // Undoes move
    void undoMove(Move *move, Piece *captured);

    // Checks that the path is clear to move piece
    bool isPathClear(Move *newMove) const;

    // Checks if a square is attacked by a piece
    bool squareAttacked(Square *square, char player) const;

    // Checks if castle squares are attacked
    // bool castleSquaresAttacked()

private:
    char turn;         // Stores which player's turn it is
    Square grid[8][8]; // Stores grid
    King *whiteKing;   // Stores white king
    King *blackKing;   // Stores black king
    /* Change to array of blackKing and whiteKing, change turn to isWhite true and false*/
};

#endif