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
    whiteKing = new King('W');
    grid['1' - '1']['a' - 'a'].piece = new Rook('W');
    grid['1' - '1']['b' - 'a'].piece = new Knight('W');
    grid['1' - '1']['c' - 'a'].piece = new Bishop('W');
    grid['1' - '1']['d' - 'a'].piece = new Queen('W');
    grid['1' - '1']['e' - 'a'].piece = whiteKing;
    whiteKing->setSquare(&grid['1' - '1']['e' - 'a']);
    grid['1' - '1']['f' - 'a'].piece = new Bishop('W');
    grid['1' - '1']['g' - 'a'].piece = new Knight('W');
    grid['1' - '1']['h' - 'a'].piece = new Rook('W');

    // Black pieces
    for (int j = 0; j < 8; j++)
    {
        grid['7' - '1'][j].piece = new Pawn('B', 'a' + j);
    }
    blackKing = new King('B');
    grid['8' - '1']['a' - 'a'].piece = new Rook('B');
    grid['8' - '1']['b' - 'a'].piece = new Knight('B');
    grid['8' - '1']['c' - 'a'].piece = new Bishop('B');
    grid['8' - '1']['d' - 'a'].piece = new Queen('B');
    grid['8' - '1']['e' - 'a'].piece = blackKing;
    blackKing->setSquare(&grid['8' - '1']['e' - 'a']);
    grid['8' - '1']['f' - 'a'].piece = new Bishop('B');
    grid['8' - '1']['g' - 'a'].piece = new Knight('B');
    grid['8' - '1']['h' - 'a'].piece = new Rook('B');
}

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
};

// Gets which player's turn it is
char Board::getTurn() const
{
    return turn;
}

// Used to request board to make move
Move *Board::requestMove(const std::string &notation, Move *prevMove)
{
    // Stores requested move
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
        std::cout << "Start square given" << std::endl;

        if (grid[newMove->getStartRow() - '1'][newMove->getStartCol() - 'a'].piece &&
            grid[newMove->getStartRow() - '1'][newMove->getStartCol() - 'a'].piece->isValidMove(newMove) &&
            isPathClear(newMove))
        {
            std::cout << "Found = true" << std::endl;
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
                // Temporarily sets start position
                newMove->setStartCol('a' + j);
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

    // Handles castle logic
    Move *castleRookMove = nullptr;

    // If castle, does rook move and checks if relevant squares are attacked
    if (newMove->getCastle())
    {
        // Parameters depending on black/white short/long castle
        char row = (turn == 'W') ? '1' : '8';
        char colKing;
        char colRook;
        if (newMove->getEnd()->col == 'g')
        {
            colKing = 'g';
            colRook = 'h';
        }
        else
        {
            colKing = 'd';
            colRook = 'a';
        }

        // Makes sure no piece attacking square King is crossing over, and there's a piece in rook square
        if (squareAttacked(&grid[row - '1'][colKing - 'a'], turn) || !grid[row - '1'][colRook - 'a'].piece)
        {
            std::cout << "Attacking King square / Found no rook" << std::endl;

            return nullptr;
        }

        // Constructs castle rook move
        castleRookMove = new Move(turn, colRook, row, &grid[row - '1'][colKing - 1 - 'a']);

        // Checks castle rook move is valid and makes move
        if (grid[row - '1'][colRook - 'a'].piece->isValidMove(castleRookMove))
        {
            makeMove(castleRookMove);
        }
    }

    // Makes move, storing any captured piece
    Piece *captured = makeMove(newMove);

    // Ensures current player's king is not in check after move is complete
    if ((turn == 'W' && squareAttacked(whiteKing->getSquare(), turn)) || (turn == 'B' && squareAttacked(blackKing->getSquare(), turn)))
    {
        std::cout << "Own King in check after completion of move" << std::endl;

        undoMove(newMove, captured);

        return nullptr;
    }

    // Ensures check status of opposition King matches notation
    if (turn == 'W')
    {
        whiteKing->setCheck(false);
        blackKing->setCheck(newMove->getCheck());

        if (squareAttacked(blackKing->getSquare(), 'B') != newMove->getCheck())
        {
            undoMove(newMove, captured);

            std::cout << "Move check notation is incorrect" << std::endl;

            return nullptr;
        }
    }
    else
    {
        blackKing->setCheck(false);
        whiteKing->setCheck(newMove->getCheck());

        if (squareAttacked(whiteKing->getSquare(), 'W') != newMove->getCheck())
        {
            undoMove(newMove, captured);

            std::cout << "Move check notation is incorrect" << std::endl;

            return nullptr;
        }
    }

    // Changes turn
    turn = (turn == 'W') ? 'B' : 'W';

    // Deletes captured piece
    delete captured;

    // Assigns moved to piece as true
    newMove->getEnd()->piece->setMoved();

    // Returns newMove if succesful
    return newMove;
}

// Makes Move
Piece *Board::makeMove(Move *move)
{
    // Captured piece to return
    Piece *captured = nullptr;
    if (move->getCapture())
    {
        captured = move->getEnd()->piece;
    }

    // Assigns piece to be moved
    Piece *toMove = grid[move->getStartRow() - '1'][move->getStartCol() - 'a'].piece;

    // Removes from previous square
    grid[move->getStartRow() - '1'][move->getStartCol() - 'a'].piece = nullptr;

    // Moves piece to new square
    move->getEnd()->piece = toMove;

    // If piece moved is King change King square
    if (move->getPiece() == 'K')
    {
        if (turn == 'W')
        {
            whiteKing->setSquare(move->getEnd());
        }
        else
        {
            blackKing->setSquare(move->getEnd());
        }
    }

    return captured;
}

// Undoes move
void Board::undoMove(Move *move, Piece *captured)
{
    // Moves back piece
    grid[move->getStartRow() - '1'][move->getStartCol() - 'a'].piece = move->getEnd()->piece;

    // Reverts end square back to prior state
    move->getEnd()->piece = captured;

    // If piece moved is King change King square back
    if (move->getPiece() == 'K')
    {
        if (turn == 'W')
        {
            whiteKing->setSquare(&grid[move->getStartRow() - '1'][move->getStartCol() - 'a']);
        }
        else
        {
            blackKing->setSquare(&grid[move->getStartRow() - '1'][move->getStartCol() - 'a']);
        }
    }
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

    // If long castle, checks the next square as well
    if (newMove->getCastle() && newMove->getEnd()->col == 'c' && grid[currRow][currCol + colStep].piece)
    {
        return false;
    }

    return true;
}

// Checks if a square is attacked by a piece
bool Board::squareAttacked(Square *square, char player) const
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
                if (curr->piece->getColour() != player &&
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
                if (curr->piece->attackOrthogonal() && curr->piece->getColour() != player)
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
                if (curr->piece->getColour() != player &&
                    (curr->piece->getNotation() == 'K' || curr->piece->attackDiagonal()))
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
                if (curr->piece->attackDiagonal() && curr->piece->getColour() != player)
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
        if (curr && curr->piece && curr->piece->getNotation() == 'N' && curr->piece->getColour() != player)
        {
            return true;
        }
    }

    // Checks for opposing pawns
    if (player == 'W')
    {
        // Checks for pawn on down left
        if (square->downLeft && square->downLeft->piece &&
            square->downLeft->piece->getNotation() == square->col - 1 &&
            square->downLeft->piece->getColour() != player)
        {
            return true;
        }
        // Checks for pawn on down right
        if (square->downRight && square->downRight->piece &&
            square->downRight->piece->getNotation() == square->col + 1 &&
            square->downRight->piece->getColour() != player)
        {
            return true;
        }
    }
    else
    {
        // Checks for pawn on up left
        if (square->upLeft && square->upLeft->piece &&
            square->upLeft->piece->getNotation() == square->col - 1 &&
            square->upLeft->piece->getColour() != player)
        {
            return true;
        }
        // Checks for pawn on up right
        if (square->upRight && square->upRight->piece &&
            square->upRight->piece->getNotation() == square->col + 1 &&
            square->upRight->piece->getColour() != player)
        {
            return true;
        }
    }

    // If couldn't find an attacking piece, return false
    return false;
}