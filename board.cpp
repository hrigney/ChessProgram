#include "board.h"

/* Board class */

// Constructor
Board::Board()
{
    // Sets up board values
    turn = 'W';

    // Sets up grid
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            // Adds square coordinates
            grid[i][j].row = '1' + i;
            grid[i][j].col = 'a' + j;

            // Adds adjacent squares
            if (i > 0)
            {
                grid[i][j].left = &grid[i - 1][j];
                if (j > 0)
                    grid[i][j].downLeft = &grid[i - 1][j - 1];
                if (j < 7)
                    grid[i][j].upLeft = &grid[i - 1][j + 1];
            }
            if (i < 7)
            {
                grid[i][j].right = &grid[i + 1][j];
                if (j > 0)
                    grid[i][j].downRight = &grid[i + 1][j - 1];
                if (j < 7)
                    grid[i][j].upRight = &grid[i + 1][j + 1];
            }
            if (j > 0)
                grid[i][j].down = &grid[i][j - 1];
            if (j < 7)
                grid[i][j].up = &grid[i][j + 1];

            // Adds knight squares
            if (i > 1 && j > 0)
                grid[i][j].knightUpLeft = &grid[i - 2][j - 1];
            if (i > 0 && j > 1)
                grid[i][j].knightLeftUp = &grid[i - 1][j - 2];
            if (i > 1 && j < 7)
                grid[i][j].knightUpRight = &grid[i - 2][j + 1];
            if (i > 0 && j < 6)
                grid[i][j].knightRightUp = &grid[i - 1][j + 2];
            if (i < 6 && j > 0)
                grid[i][j].knightDownLeft = &grid[i + 2][j - 1];
            if (i < 7 && j > 1)
                grid[i][j].knightLeftDown = &grid[i + 1][j - 2];
            if (i < 6 && j < 7)
                grid[i][j].knightDownRight = &grid[i + 2][j + 1];
            if (i < 7 && j < 6)
                grid[i][j].knightRightDown = &grid[i + 1][j + 2];
        }
    }

    // Assigns pieces to starting positions

    // White pieces
    for (int j = 0; j < 8; j++)
    {
        grid['2' - '1'][j].piece = new Pawn('W', 'a' + j);
    }
    grid['1' - '1']['a' - 'a'].piece = new Rook('W');
    grid['1' - '1']['b' - 'a'].piece = new Knight('W');
    grid['1' - '1']['c' - 'a'].piece = new Bishop('W');
    grid['1' - '1']['d' - 'a'].piece = new Queen('W');
    grid['1' - '1']['e' - 'a'].piece = new King('W');
    grid['1' - '1']['f' - 'a'].piece = new Bishop('W');
    grid['1' - '1']['g' - 'a'].piece = new Knight('W');
    grid['1' - '1']['h' - 'a'].piece = new Rook('W');

    // Black pieces
    for (int j = 0; j < 8; j++)
    {
        grid['7' - '1'][j].piece = new Pawn('B', 'a' + j);
    }
    grid['8' - '1']['a' - 'a'].piece = new Rook('B');
    grid['8' - '1']['b' - 'a'].piece = new Knight('B');
    grid['8' - '1']['c' - 'a'].piece = new Bishop('B');
    grid['8' - '1']['d' - 'a'].piece = new Queen('B');
    grid['8' - '1']['e' - 'a'].piece = new King('B');
    grid['8' - '1']['f' - 'a'].piece = new Bishop('B');
    grid['8' - '1']['g' - 'a'].piece = new Knight('B');
    grid['8' - '1']['h' - 'a'].piece = new Rook('B');
};

// Destructor
Board::~Board()
{
    // Deletes all dynamically allocated pieces
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            delete grid[i][j].piece;
        }
    }
}

// Displays board in terminal
void Board::displayBoard() const
{
    // Get the console handle
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Cycles through all squares
    for (int i = 7; i >= 0; i--)
    {
        for (int j = 0; j < 8; j++)
        {
            // If square has a piece
            if (grid[i][j].piece)
            {

                // Changes text colour
                if (grid[i][j].piece->getColour() == 'B')
                {
                    // Sets to grey
                    SetConsoleTextAttribute(hConsole, 8);
                }
                else
                {
                    // Reset to default color
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                }

                std::cout << grid[i][j].piece->getNotation() << "  ";
            }
            else
            {
                // Sets to green
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                std::cout << grid[i][j].col << grid[i][j].row << " ";
            }
        }

        std::cout << std::endl;
    }

    // Reset to default color
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    return;
};

// Gets which player's turn it is
char Board::getTurn() const
{
    return turn;
}

// Used to request board to make move
Move *Board::requestMove(const std::string &notation, Move *prevMove)
{
    // Stores new movfe
    Move *newMove = nullptr;

    // Tries to construct move
    try
    {
        newMove = new Move(turn, notation, prevMove, grid);
    }
    catch (const std::invalid_argument &e)
    {
        // Outputs message detailing what's wrong
        std::cout << e.what() << std::endl;

        return newMove;
    }

    // Searches board for the piece trying to be moved
    bool found = false;

    // If start square given
    if (newMove->getStartCol() && newMove->getStartRow())
    {
        if (grid[newMove->getStartRow() - '1'][newMove->getStartCol() - 'a'].piece &&
            grid[newMove->getStartRow() - '1'][newMove->getStartCol() - 'a'].piece->isValidMove(newMove) &&
            isPathClear(newMove))
        {
            found = true;
        }
    }
    // If start col given
    else if (newMove->getStartCol())
    {
        for (int i = 0; i < 8; i++)
        {
            // Temporarily sets startRow
            newMove->setStartRow('1' + i);

            if (grid[i][newMove->getStartCol() - 'a'].piece &&
                grid[i][newMove->getStartCol() - 'a'].piece->isValidMove(newMove) &&
                isPathClear(newMove))
            {
                found = true;
                break;
            }
        }
    }
    // If start row given
    else if (newMove->getStartRow())
    {
        for (int j = 0; j < 8; j++)
        {
            // Temporarily sets startCol
            newMove->setStartCol('a' + j);

            if (grid[j][newMove->getStartCol() - 'a'].piece &&
                grid[j][newMove->getStartCol() - 'a'].piece->isValidMove(newMove) &&
                isPathClear(newMove))
            {
                found = true;
                break;
            }
        }
    }
    // Searches entire board
    else
    {
        for (int i = 0; i < 8 && !found; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                // Temporarily sets startCol
                newMove->setStartCol('a' + j);
                // Temporarily sets startRow
                newMove->setStartRow('1' + i);

                if (grid[i][j].piece &&
                    grid[i][j].piece->isValidMove(newMove) &&
                    isPathClear(newMove))
                {
                    found = true;
                    break;
                }
            }
        }
    }

    // If not found return null ptr
    if (!found)
    {
        return nullptr;
    }

    // Makes move
    makeMove(newMove);

    // Changes turn
    turn = (turn == 'W') ? 'B' : 'W';

    // Returns newMove if succesful
    return newMove;
}

// Makes Move
void Board::makeMove(Move *newMove)
{
    // Assigns piece to be moved
    Piece *toMove = grid[newMove->getStartRow() - '1'][newMove->getStartCol() - 'a'].piece;

    // Removes from previous square
    grid[newMove->getStartRow() - '1'][newMove->getStartCol() - 'a'].piece = nullptr;

    // Moves piece to new square
    newMove->getEnd()->piece = toMove;
}

// Checks that the path is clear to move piece
bool Board::isPathClear(Move *newMove) const
{
    // If Knight move don't need to worry
    if (newMove->getPiece() == 'N')
    {
        return true;
    }

    // Stores how far the piece moves col and row wise
    int rowMove = newMove->getEnd()->row - newMove->getStartRow();
    int colMove = newMove->getEnd()->col - newMove->getStartCol();

    // Stores step size when scouting board
    int rowStep = (rowMove > 0) - (rowMove < 0);
    int colStep = (colMove > 0) - (colMove < 0);

    // Stores current square we're searching
    int currRow = newMove->getStartRow() - '1' + rowStep;
    int currCol = newMove->getStartCol() - 'a' + colStep;

    // Makes sure there's no pieces in the way of move
    while ((currRow != newMove->getEnd()->row - '1') || (currCol != newMove->getEnd()->col - 'a'))
    {
        if (grid[currRow][currCol].piece)
        {
            return false;
        }

        currRow += rowStep;
        currCol += colStep;
    }

    return true;
}

// Checks if a square is attacked by a piece
bool Board::squareAttacked(Square *square, char byPlayer) const
{
    // Stores square currently being searched
    Square *curr = nullptr;

    // Iterates through orthogonal movements
    std::array<Square * Square::*, 4> orthoDirections = {&Square::left, &Square::right, &Square::up, &Square::down};

    for (Square *Square::*direction : orthoDirections)
    {
        curr = square->*direction;

        // First square we check for opposite King as well
        if (curr)
        {
            if (curr->piece)
            {
                if (curr->piece->getColour() == byPlayer &&
                    (curr->piece->getNotation() == 'K' || curr->piece->attackOrthogonal()))
                {
                    return true;
                }

                continue;
            }

            curr = square->*direction;
        }

        // Ensures not nullptr while traversing board
        while (curr)
        {
            if (curr->piece)
            {
                // If find attacking piece return true
                if (curr->piece->attackOrthogonal() && curr->piece->getColour() == byPlayer)
                {
                    return true;
                }

                break;
            }

            curr = curr->*direction;
        }
    }

    // Iterates through diagonal movements
    std::array<Square * Square::*, 4> diagDirections = {&Square::upLeft, &Square::upRight, &Square::downLeft, &Square::downRight};

    for (Square *Square::*direction : diagDirections)
    {
        curr = square->*direction;

        // First square we check for opposite King as well
        if (curr)
        {
            if (curr->piece)
            {
                if (curr->piece->getColour() == byPlayer &&
                    (curr->piece->getNotation() == 'K' || curr->piece->attackOrthogonal()))
                {
                    return true;
                }

                continue;
            }

            curr = square->*direction;
        }

        // Ensures not nullptr while traversing board
        while (curr)
        {
            if (curr->piece)
            {
                // If find attacking piece return true
                if (curr->piece->attackDiagonal() && curr->piece->getColour() == byPlayer)
                {
                    return true;
                }

                break;
            }

            curr = curr->*direction;
        }
    }

    // Iterates through knight movements
    std::array<Square * Square::*, 8> knightDirections = {
        &Square::knightUpLeft, &Square::knightUpRight,
        &Square::knightDownLeft, &Square::knightDownRight,
        &Square::knightLeftUp, &Square::knightLeftDown,
        &Square::knightRightUp, &Square::knightRightDown};

    for (Square *Square::*direction : knightDirections)
    {
        curr = square->*direction;

        // If find attacking piece return true
        if (curr && curr->piece && curr->piece->attackDiagonal() && curr->piece->getColour() == byPlayer)
        {
            return true;
        }
    }

    // Checks for opposing pawns
    if (byPlayer == 'W')
    {
        // Checks for pawn on down left
        if (square->downLeft && square->downLeft->piece &&
            square->downLeft->piece->getNotation() == square->col - 1 &&
            square->downLeft->piece->getColour() == byPlayer)
        {
            return true;
        }
        // Checks for pawn on down right
        if (square->downRight && square->downRight->piece &&
            square->downRight->piece->getNotation() == square->col + 1 &&
            square->downRight->piece->getColour() == byPlayer)
        {
            return true;
        }
    }
    else
    {
        // Checks for pawn on up left
        if (square->upLeft && square->upLeft->piece &&
            square->upLeft->piece->getNotation() == square->col - 1 &&
            square->upLeft->piece->getColour() == byPlayer)
        {
            return true;
        }
        // Checks for pawn on up right
        if (square->upRight && square->upRight->piece &&
            square->upRight->piece->getNotation() == square->col + 1 &&
            square->upRight->piece->getColour() == byPlayer)
        {
            return true;
        }
    }

    // If couldn't find an attacking piece, return false
    return false;
}