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

    // Stores orthogonally adjacent squares
    Square *up = nullptr;
    Square *down = nullptr;
    Square *left = nullptr;
    Square *right = nullptr;

    // Stores diagonally adjacent square
    Square *upLeft = nullptr;
    Square *upRight = nullptr;
    Square *downLeft = nullptr;
    Square *downRight = nullptr;

    // Stores knight move squares
    Square *knightUpLeft = nullptr;    // Two up, one left
    Square *knightUpRight = nullptr;   // Two up, one right
    Square *knightDownLeft = nullptr;  // Two down, one left
    Square *knightDownRight = nullptr; // Two down, one right
    Square *knightLeftUp = nullptr;    // Two left, one up
    Square *knightLeftDown = nullptr;  // Two left, one down
    Square *knightRightUp = nullptr;   // Two right, one up
    Square *knightRightDown = nullptr; // Two right, one down
};

/* Represents a chess piece */
class Piece
{
public:
    // Constructor
    Piece(bool isWhite, char notation);

    // Destructor
    virtual ~Piece();

    // Returns notation of piece
    char getNotation() const;

    // Returns colour of piece
    bool getIsWhite() const;

    // Sets notation of piece
    void setNotation(char notation);

    // Sets moved to true
    void setMoved();

    // Gets moved
    bool getMoved() const;

    // Decides if a move is valid
    bool virtual isValidMove(Move *newMove) const;

    // Returns true if piece can attack orthogonally
    bool virtual attackOrthogonal() const = 0;

    // Returns true if piece can attack diagonally
    bool virtual attackDiagonal() const = 0;

private:
    char notation;      // Notation of piece
    bool isWhite;       // True if piece is white
    bool moved = false; // Stores whether or not a piece has moved
};

/* Pawn class */

class Pawn : public Piece
{
public:
    // Constructor
    Pawn(bool isWhite, char notation);

    // Decides if a move is valid
    bool isValidMove(Move *newMove) const override;

    // Returns true if piece can attack orthogonally
    bool virtual attackOrthogonal() const override;

    // Returns true if piece can attack diagonally
    bool virtual attackDiagonal() const override;
};

/* Knight class */

class Knight : public Piece
{
public:
    // Constructor
    Knight(bool isWhite);

    // Decides if a move is valid
    bool isValidMove(Move *newMove) const override;

    // Returns true if piece can attack orthogonally
    bool virtual attackOrthogonal() const override;

    // Returns true if piece can attack diagonally
    bool virtual attackDiagonal() const override;
};

/* Bishop class */

class Bishop : public Piece
{
public:
    // Constructor
    Bishop(bool isWhite);

    // Decides if a move is valid
    bool isValidMove(Move *newMove) const override;

    // Returns true if piece can attack orthogonally
    bool virtual attackOrthogonal() const override;

    // Returns true if piece can attack diagonally
    bool virtual attackDiagonal() const override;
};

/* Rook class */

class Rook : public Piece
{
public:
    // Constructor
    Rook(bool isWhite);

    // Decides if a move is valid
    bool isValidMove(Move *newMove) const override;

    // Returns true if piece can attack orthogonally
    bool virtual attackOrthogonal() const override;

    // Returns true if piece can attack diagonally
    bool virtual attackDiagonal() const override;
};

/* Queen class */

class Queen : public Piece
{
public:
    // Constructor
    Queen(bool isWhite);

    // Decides if a move is valid
    bool isValidMove(Move *newMove) const override;

    // Returns true if piece can attack orthogonally
    bool virtual attackOrthogonal() const override;

    // Returns true if piece can attack diagonally
    bool virtual attackDiagonal() const override;
};

/* King class */

class King : public Piece
{
public:
    // Constructor
    King(bool isWhite);

    // Sets check status
    void setCheck(bool inCheck);

    // Gets check status of King
    bool getCheckStatus() const;

    // Sets location of King
    void setSquare(Square *square);

    // Gets location of King
    Square *getSquare() const;

    // Decides if a move is valid
    bool isValidMove(Move *newMove) const override;

    // Returns true if piece can attack orthogonally
    bool virtual attackOrthogonal() const override;

    // Returns true if piece can attack diagonally
    bool virtual attackDiagonal() const override;

private:
    bool inCheck; // Stores whether or not King is in check
    Square *square;
};

/* Represents a move in the game */
class Move
{
public:
    // Default constructor
    Move(bool isWhite, const std::string &notation, Move *prevMove, Square (&grid)[8][8]);

    // Castle rook constructor
    Move(bool isWhite, char startCol, char startRow, Square *end);

    // Gets piece
    char getPiece() const;

    // Gets colour
    char getIsWhite() const;

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

    // Gets enPassant
    bool getEnPassant() const;

    // Gets promotion
    char getPromotion() const;

    // Gets notation
    const std::string &getNotation() const;

    // Gets prevMove
    Move *getPrevMove() const;

private:
    // Regex pattern
    static const std::regex pattern;

    // Stores piece
    char piece;
    bool isWhite;

    // Stores starting and finishing square of piece
    char startCol = '\0';
    char startRow = '\0';
    Square *end = nullptr;

    // Type of move
    bool capture;
    bool castle;
    bool check;
    bool mate;
    bool enPassant;
    char promotion = '\0';

    // PGN notation of move
    const std::string notation;

    // Previous move
    Move *prevMove = nullptr;
};

#endif