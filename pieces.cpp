#include "pieces.h"

/* Piece class */

// Constructor
Piece::Piece(char colour, char notation)
{
    this->colour = colour;
    this->notation = notation;
}

// Gets notation
char Piece::getNotation() const
{
    return notation;
}

// Gets piece colour
char Piece::getColour() const
{
    return colour;
}

// Sets notation of Piece
void Piece::setNotation(char notation)
{
    this->notation = notation;
}

// Decides if a move is valid
bool Piece::isValidMove(Move *newMove) const
{
    // Checks colour matches
    if (newMove->getColour() != colour)
    {
        return false;
    }

    // Checks notation matches
    if (newMove->getPiece() != notation)
    {
        return false;
    }

    return true;
}

/* Pawn class */

// Constructor
Pawn::Pawn(char colour, char notation)
    : Piece(colour, notation)
{
    moved = false;
}

// Decides if a move is valid
bool Pawn::isValidMove(Move *newMove, Square (&grid)[8][8]) const
{
    // Calls base function
    if (!Piece::isValidMove(newMove))
    {
        return false;
    }

    // If move is a capture
    if (newMove->getCapture())
    {
        // If piece doesn't move 1 diagonal
        if ((newMove->getEnd()->row - newMove->getStartRow() != 1) ||
            (std::abs(newMove->getEnd()->col - newMove->getStartCol()) != 1))
        {
            return false;
        }

        newMove->setDirection('D');
    }
    else
    {
        // If piece doesn't move 1/2 up
        if ((newMove->getEnd()->row - newMove->getStartRow() != 1) || !((newMove->getEnd()->row - newMove->getStartRow() == 2) && !moved))
        {
            return false;
        }

        newMove->setDirection('V');
    }

    return true;
}

/* Knight class */

// Constructor
Knight::Knight(char colour)
    : Piece(colour, 'N')
{
}

// Decides if a move is valid
bool Knight::isValidMove(Move *newMove, Square (&grid)[8][8]) const
{
    // Calls base function
    bool toReturn = Piece::isValidMove(newMove);

    return toReturn;
}

/* Bishop class */

// Constructor
Bishop::Bishop(char colour)
    : Piece(colour, 'B')
{
}

// Decides if a move is valid
bool Bishop::isValidMove(Move *newMove, Square (&grid)[8][8]) const
{
    // Calls base function
    bool toReturn = Piece::isValidMove(newMove);

    return toReturn;
}

/* Rook class */

// Constructor
Rook::Rook(char colour)
    : Piece(colour, 'R')
{
    moved = false;
}

// Decides if a move is valid
bool Rook::isValidMove(Move *newMove, Square (&grid)[8][8]) const
{
    // Calls base function
    bool toReturn = Piece::isValidMove(newMove);

    return toReturn;
}

/* Queen class */

// Constructor
Queen::Queen(char colour)
    : Piece(colour, 'Q')
{
}

// Decides if a move is valid
bool Queen::isValidMove(Move *newMove, Square (&grid)[8][8]) const
{
    // Calls base function
    bool toReturn = Piece::isValidMove(newMove);

    return toReturn;
}

/* King class */

// Constructor
King::King(char colour)
    : Piece(colour, 'K')
{
    moved = false;
    inCheck = false;
}

// Decides if a move is valid
bool King::isValidMove(Move *newMove, Square (&grid)[8][8]) const
{
    // Calls base function
    bool toReturn = Piece::isValidMove(newMove);

    return toReturn;
}

/* Move class */

// Constructor
Move::Move(char colour, const std::string &notation, Move *prevMove, Square (&grid)[8][8])
    : colour(colour), notation(notation), prevMove(prevMove)
{

    // Stores regex matches
    std::smatch matches;

    // Ensures notation is valid
    if (!std::regex_match(notation, matches, pattern))
    {
        throw std::invalid_argument("Invalid notation");
    }

    // Assigns check
    if (matches[8].matched)
    {
        check = true;
        mate = false;
    }
    // Assigns mate
    else if (matches[9].matched)
    {
        mate = true;
        check = false;
    }
    else
    {
        check = false;
        mate = false;
    }

    // Checks if move is castle
    if (matches[6].matched)
    {
        piece = 'K';
        castle = true;
        capture = false;

        // Checks if move is long castle
        if (matches[7].matched)
        {
            // Assigns start and end
            if (colour == 'W')
            {
                startRow = '1';
                startCol = 'e';
                end = &grid['1' - '1']['c' - 'a'];
            }
            else
            {
                startRow = '8';
                startCol = 'e';
                end = &grid['8' - '1']['c' - 'a'];
            }
        }
        else
        {
            // Assigns start and end
            if (colour == 'W')
            {
                startRow = '1';
                startCol = 'e';
                end = &grid['1' - '1']['g' - 'a'];
            }
            else
            {
                startRow = '8';
                startCol = 'e';
                end = &grid['8' - '1']['g' - 'a'];
            }
        }
    }
    // If move is not castle
    else
    {
        castle = false;

        // Assigns piece
        piece = notation[0];

        // Assigns end location
        end = &grid[matches[5].str()[1] - '1'][matches[5].str()[0] - 'a'];

        /* Assigns capture */

        // If capture
        if (matches[1].matched && (matches[1].str().back() == 'x'))
        {
            capture = true;

            // Checks there is a piece there to be captured, or is a valid en passant move
            if (!end->piece &&
                !(prevMove &&                                            // There is a previous move
                  (end->col == prevMove->getPiece()) &&                  // Curr move end column matches prevMove piece
                  (std::abs(prevMove->getStartRow() - end->row) == 1) && // prevMove started 1 spot above/ below end location
                  (std::abs(prevMove->getEnd()->row - end->row) == 1)))  // prevMove ended 1 spot above/ below end location
            {
                throw std::invalid_argument("No piece to capture");
            }

            // Checks if piece to capture is opposition piece
            else if (end->piece->getColour() == colour)
            {
                throw std::invalid_argument("Can't capture piece of same colour");
            }
        }
        else
        {
            capture = false;

            // If not capture ensures there's no piece on square
            if (end->piece)
            {
                throw std::invalid_argument("Piece already on square");
            }
        }

        /* Assigns startCol and startRow */

        // If pawn move
        if (!matches[1].matched)
        {
            startCol = notation[0];
        }
        else
        {
            // If notation disambiguates pieces use information
            if (matches[4].matched)
            {
                startCol = matches[4].str()[0];
            }
            else
            {
                if (matches[2].matched)
                {
                    startCol = matches[2].str()[0];
                }
                if (matches[3].matched)
                {
                    startRow = matches[3].str()[0];
                }
            }
        }
    }
}

// Regex pattern
const std::regex Move::pattern("(?:(?:((?:[KQRBN]([a-h])?([1-8])?x?)|(?:([a-h])x))?([a-h][1-8]))|(O-O(-O)?))(?:(\\+)|(\\#))?");

// Gets piece
char Move::getPiece() const
{
    return piece;
}

// Gets colour
char Move::getColour() const
{
    return colour;
}

// Sets startCol
void Move::setStartCol(char startCol)
{
    this->startCol = startCol;
}

// Sets startCol
void Move::setStartRow(char startRow)
{
    this->startRow = startRow;
}

// Gets startCol
char Move::getStartCol() const
{
    return startCol;
}

// Gets startRow
char Move::getStartRow() const
{
    return startRow;
}

// Gets end square
Square *Move::getEnd() const
{
    return end;
}

// Gets capture
bool Move::getCapture() const
{
    return capture;
}

// Gets castle
bool Move::getCastle() const
{
    return castle;
}

// Gets check
bool Move::getCheck() const
{
    return check;
}

// Gets mate
bool Move::getMate() const
{
    return mate;
}

// Sets direction
void Move::setDirection(char direction)
{
    this->direction = direction;
}

// Gets startCol
char Move::getDirection() const
{
    return direction;
}

// Gets notation
const std::string &Move::getNotation() const
{
    return notation;
}

// Gets prevMove
Move *Move::getPrevMove() const
{
    return prevMove;
}