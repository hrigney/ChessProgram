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
    bool squareAttacked(Square *square, bool player) const;

    // Checks if castle squares are attacked
    // bool castleSquaresAttacked()

    // Checks if King is checkmated
    bool kingMated(Move *newMove) const;

    // Checks if piece can be captured
    bool pieceCantBeCaptured(Square *pieceSquare) const;

    /* Changes to make
    Add King checked function that looks at only the piece that moved and pieces on the line from where the piece made
    Only call square attacked function on King's end location if the King is the piece that has moved
    Just figure it out. You can do better.*/

private:
    bool whiteTurn;    // Stores which player's turn it is
    Square grid[8][8]; // Stores grid
    King *kings[2];    // Stores Kings, white is in index 1 (true), black is index 0 (false)
};

#endif