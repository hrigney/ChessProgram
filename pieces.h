#ifndef PIECES_H
#define PIECES_H

#include "common.h"

/* Forward declarations */
struct Square;
class Piece;
class Pawn;
class Knight;
class Bishop;
class Rook;
class Queen;
class King;
class Move;

/* Represents a square on the chess board */
struct Square
{
    // Grid location
    char col;
    char row;

    // Piece currently on square
    Piece *piece = nullptr;
};

/* Represents a chess piece */
class Piece
{
public:
    // Constructor
    Piece(char colour, char notation);

    // Destructor
    // virtual ~Piece();

    // Returns notation of piece
    char getNotation() const;

    // Returns colour of piece
    char getColour() const;

    // Sets notation of piece
    void setNotation(char notation);

    // Decides if a move is valid
    bool virtual isValidMove(Move *newMove) const;

    // Undos move
    // void virtual undoMove(Move *newMove) = 0;

private:
    char colour;   // Colour of piece
    char notation; // Notation of piece
};

/* Pawn class */

class Pawn : public Piece
{
public:
    // Constructor
    Pawn(char colour, char notation);

    // Decides if a move is valid
    bool isValidMove(Move *newMove) const override;

private:
    bool moved;
};

/* Knight class */

class Knight : public Piece
{
public:
    // Constructor
    Knight(char colour);

    // Decides if a move is valid
    bool isValidMove(Move *newMove) const override;
};

/* Bishop class */

class Bishop : public Piece
{
public:
    // Constructor
    Bishop(char colour);

    // Decides if a move is valid
    bool isValidMove(Move *newMove) const override;
};

/* Rook class */

class Rook : public Piece
{
public:
    // Constructor
    Rook(char colour);

    // Decides if a move is valid
    bool isValidMove(Move *newMove) const override;

private:
    bool moved;
};

/* Queen class */

class Queen : public Piece
{
public:
    // Constructor
    Queen(char colour);

    // Decides if a move is valid
    bool isValidMove(Move *newMove) const override;
};

/* King class */

class King : public Piece
{
public:
    // Constructor
    King(char colour);

    // Sets check status
    void setCheck(bool check);

    // Gets check status of King
    bool getCheckStatus() const;

    // Decides if a move is valid
    bool isValidMove(Move *newMove) const override;

private:
    bool moved;
    bool inCheck;
};

/* Represents a move in the game */
class Move
{
public:
    // Constructor
    Move(char colour, const std::string &notation, Move *prevMove, Square (&grid)[8][8]);

    // Gets piece
    char getPiece() const;

    // Gets colour
    char getColour() const;

    // Sets startCol
    void setStartCol(char startCol);

    // Sets startRow
    void setStartRow(char startRow);

    // Gets startCol
    char getStartCol() const;

    // Gets startRow
    char getStartRow() const;

    // Gets end square
    Square *getEnd() const;

    // Gets capture
    bool getCapture() const;

    // Gets castle
    bool getCastle() const;

    // Gets check
    bool getCheck() const;

    // Gets mate
    bool getMate() const;

    // Gets notation
    const std::string &getNotation() const;

    // Gets prevMove
    Move *getPrevMove() const;

private:
    // Regex pattern
    static const std::regex pattern;

    // Stores piece
    char piece;
    char colour;

    // Stores starting and finishing square of piece
    char startCol = '\0';
    char startRow = '\0';
    Square *end = nullptr;

    // Type of move
    bool capture;
    bool castle;
    bool check;
    bool mate;

    // PGN notation of move
    const std::string notation;

    // Previous move
    Move *prevMove = nullptr;
};

#endif