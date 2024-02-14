#include "board.h"

/* Board class */

// Constructor
Board::Board() : whiteTurn(true)
{
    // Sets up grid
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            // Adds square coordinates
            grid[i][j].row = '1' + i;
            grid[i][j].col = 'a' + j;

            // Adds adjacent squares
            if (i >= 1)
                grid[i][j].down = &grid[i - 1][j];
            if (i <= 6)
                grid[i][j].up = &grid[i + 1][j];
            if (j >= 1)
            {
                grid[i][j].left = &grid[i][j - 1];
                if (i >= 1)
                    grid[i][j].downLeft = &grid[i - 1][j - 1];
                if (i <= 6)
                    grid[i][j].upLeft = &grid[i + 1][j - 1];
            }
            if (j <= 6)
            {
                grid[i][j].right = &grid[i][j + 1];
                if (i >= 1)
                    grid[i][j].downRight = &grid[i - 1][j + 1];
                if (i <= 6)
                    grid[i][j].upRight = &grid[i + 1][j + 1];
            }

            // Adds knight squares
            if (i <= 5 && j >= 1)
                grid[i][j].knightUpLeft = &grid[i + 2][j - 1];
            if (i <= 6 && j >= 2)
                grid[i][j].knightLeftUp = &grid[i + 1][j - 2];
            if (i <= 5 && j <= 6)
                grid[i][j].knightUpRight = &grid[i + 2][j + 1];
            if (i <= 6 && j <= 5)
                grid[i][j].knightRightUp = &grid[i + 1][j + 2];
            if (i >= 2 && j >= 1)
                grid[i][j].knightDownLeft = &grid[i - 2][j - 1];
            if (i >= 1 && j >= 2)
                grid[i][j].knightLeftDown = &grid[i - 1][j - 2];
            if (i >= 2 && j <= 6)
                grid[i][j].knightDownRight = &grid[i - 2][j + 1];
            if (i >= 1 && j <= 5)
                grid[i][j].knightRightDown = &grid[i - 1][j + 2];
        }
    }

    // Assigns pieces to starting positions

    // White pieces
    for (int j = 0; j < 8; j++)
    {
        grid['2' - '1'][j].piece = new Pawn(true, 'a' + j);
    }
    kings[1] = new King(true);
    grid['1' - '1']['a' - 'a'].piece = new Rook(true);
    grid['1' - '1']['b' - 'a'].piece = new Knight(true);
    grid['1' - '1']['c' - 'a'].piece = new Bishop(true);
    grid['1' - '1']['d' - 'a'].piece = new Queen(true);
    grid['1' - '1']['e' - 'a'].piece = kings[1];
    kings[1]->setSquare(&grid['1' - '1']['e' - 'a']);
    grid['1' - '1']['f' - 'a'].piece = new Bishop(true);
    grid['1' - '1']['g' - 'a'].piece = new Knight(true);
    grid['1' - '1']['h' - 'a'].piece = new Rook(true);

    // Black pieces
    for (int j = 0; j < 8; j++)
    {
        grid['7' - '1'][j].piece = new Pawn(false, 'a' + j);
    }
    kings[0] = new King(false);
    grid['8' - '1']['a' - 'a'].piece = new Rook(false);
    grid['8' - '1']['b' - 'a'].piece = new Knight(false);
    grid['8' - '1']['c' - 'a'].piece = new Bishop(false);
    grid['8' - '1']['d' - 'a'].piece = new Queen(false);
    grid['8' - '1']['e' - 'a'].piece = kings[0];
    kings[0]->setSquare(&grid['8' - '1']['e' - 'a']);
    grid['8' - '1']['f' - 'a'].piece = new Bishop(false);
    grid['8' - '1']['g' - 'a'].piece = new Knight(false);
    grid['8' - '1']['h' - 'a'].piece = new Rook(false);
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
                if (!grid[i][j].piece->getIsWhite())
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
    return whiteTurn;
}

// Used to request board to make move
Move *Board::requestMove(const std::string &notation, Move *prevMove)
{
    // Stores requested move
    Move *newMove = nullptr;

    // Tries to construct move
    try
    {
        newMove = new Move(whiteTurn, notation, prevMove, grid);
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
        std::cout << "Invalid move" << std::endl;

        return nullptr;
    }

    // Handles castle logic
    Move *castleRookMove = nullptr;

    // If castle, does rook move and checks if relevant squares are attacked
    if (newMove->getCastle())
    {
        // Parameters depending on black/white short/long castle
        char row = whiteTurn ? '1' : '8';
        char colStart;
        char colEnd;
        if (newMove->getEnd()->col == 'g')
        {
            colStart = 'h';
            colEnd = 'f';
        }
        else
        {
            colStart = 'a';
            colEnd = 'd';
        }

        // Makes sure no piece attacking square King is crossing over, and there's a piece in rook square
        if (squareAttacked(&grid[row - '1'][colEnd - 'a'], whiteTurn) ||
            !grid[row - '1'][colStart - 'a'].piece)
        {
            std::cout << "Attacking King square / Found no rook" << std::endl;

            return nullptr;
        }

        // Constructs castle rook move
        castleRookMove = new Move(whiteTurn, colStart, row, &grid[row - '1'][colEnd - 'a']);

        // Checks castle rook move is valid and makes move
        if (grid[row - '1'][colStart - 'a'].piece->isValidMove(castleRookMove))
        {
            makeMove(castleRookMove);
        }
    }

    // Makes move, storing any captured piece
    Piece *captured = makeMove(newMove);

    // Stores true if needs to call undoMove
    bool doUndoMove = false;

    // Ensures current player's king is not in check after move is complete
    if (squareAttacked(kings[whiteTurn]->getSquare(), whiteTurn))
    {
        std::cout << "Own King in check after move" << std::endl;
        doUndoMove = true;
    }

    // Ensures check status of opposition King matches notation
    else if (squareAttacked(kings[!whiteTurn]->getSquare(), !whiteTurn) != newMove->getCheck())
    {
        std::cout << "Move check notation is incorrect" << std::endl;
        doUndoMove = true;
    }

    // Ensures mate status of opposition King matches notation
    else if (newMove->getCheck() && (kingMated(newMove) != newMove->getMate()))
    {
        std::cout << "Move checkmate notation is incorrect" << std::endl;
        doUndoMove = true;
    }

    // Undoes move if necessary
    if (doUndoMove)
    {
        undoMove(newMove, captured);

        if (castleRookMove)
        {
            undoMove(castleRookMove, nullptr);
        }

        return nullptr;
    }

    /* Logic to implement
    if newMove is checkmate
        end game
    */

    // Updates check status
    kings[whiteTurn]->setCheck(false);
    kings[!whiteTurn]->setCheck(newMove->getCheck());

    // Changes turn
    whiteTurn = !whiteTurn;

    // Deletes captured piece
    delete captured;

    // Assigns moved to piece as true
    newMove->getEnd()->piece->setMoved();

    // If move is a pawn, updates notation
    if (newMove->getPiece() >= 'a' && newMove->getPiece() <= 'h' && newMove->getCapture())
    {
        newMove->getEnd()->piece->setNotation(newMove->getEnd()->col);
    }

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
        if (move->getEnPassant()) // If capture is en passant
        {
            if (whiteTurn)
            {
                captured = move->getEnd()->down->piece;
                move->getEnd()->down->piece = nullptr;
            }
            else
            {
                captured = move->getEnd()->up->piece;
                move->getEnd()->up->piece = nullptr;
            }
        }
        else
        {
            captured = move->getEnd()->piece;
        }
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
        kings[whiteTurn]->setSquare(move->getEnd());
    }

    return captured;
}

// Undoes move
void Board::undoMove(Move *move, Piece *captured)
{
    // Moves back piece
    grid[move->getStartRow() - '1'][move->getStartCol() - 'a'].piece = move->getEnd()->piece;

    // Reverts end square back to prior state
    if (move->getEnPassant())
    {
        if (whiteTurn)
        {
            move->getEnd()->down->piece = captured;
        }
        else
        {
            move->getEnd()->up->piece = captured;
        }
        move->getEnd()->piece = nullptr;
    }
    else
    {
        move->getEnd()->piece = captured;
    }

    // If piece moved is King change King square back
    if (move->getPiece() == 'K')
    {
        kings[whiteTurn]->setSquare(&grid[move->getStartRow() - '1'][move->getStartCol() - 'a']);
    }
}

// Checks that the path is clear to move piece
bool Board::isPathClear(Move *newMove) const /* Change from addition to multi dimension array of directions by adding 1 to rowStep and colStep */
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

// Checks if a square is attacked by a piece, player is colour of defender
bool Board::squareAttacked(Square *square, bool player) const
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
                if (curr->piece->getIsWhite() != player &&
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
                if (curr->piece->attackOrthogonal() && curr->piece->getIsWhite() != player)
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
                if (curr->piece->getIsWhite() != player &&
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
                if (curr->piece->attackDiagonal() && curr->piece->getIsWhite() != player)
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
        if (curr && curr->piece && curr->piece->getNotation() == 'N' && curr->piece->getIsWhite() != player)
        {
            return true;
        }
    }

    // Checks for opposing pawns
    if (player)
    {
        // Checks for pawn on down left
        if (square->downLeft && square->downLeft->piece &&
            square->downLeft->piece->getNotation() == square->col - 1 &&
            square->downLeft->piece->getIsWhite() != player)
        {
            return true;
        }
        // Checks for pawn on down right
        if (square->downRight && square->downRight->piece &&
            square->downRight->piece->getNotation() == square->col + 1 &&
            square->downRight->piece->getIsWhite() != player)
        {
            return true;
        }
    }
    else
    {
        // Checks for pawn on up left
        if (square->upLeft && square->upLeft->piece &&
            square->upLeft->piece->getNotation() == square->col - 1 &&
            square->upLeft->piece->getIsWhite() != player)
        {
            return true;
        }
        // Checks for pawn on up right
        if (square->upRight && square->upRight->piece &&
            square->upRight->piece->getNotation() == square->col + 1 &&
            square->upRight->piece->getIsWhite() != player)
        {
            return true;
        }
    }

    // If couldn't find an attacking piece, return false
    return false;
}

// Checks if King is checkmated
bool Board::kingMated(Move *move) const
{
    // Stores multidimensional array of directions
    std::array<std::array<Square * Square::*, 3>, 3> directions = {{{&Square::downLeft, &Square::down, &Square::downRight},
                                                                    {&Square::left, nullptr, &Square::right},
                                                                    {&Square::upLeft, &Square::up, &Square::upRight}}};

    // Stores square being searched
    Square *curr;

    // Checks whether there any surrounding squares the King can move to
    for (auto &row : directions)
    {
        for (Square *Square::*direction : row)
        {
            if (direction)
            {
                curr = kings[!whiteTurn]->getSquare()->*direction;

                if (curr && !curr->piece &&
                    !squareAttacked(curr, !whiteTurn))
                {
                    return false;
                }
            }
        }
    }

    // Tracks whether the moved piece checks the King
    bool movedPieceChecks = false;

    // If piece moved is knight
    if (move->getPiece() == 'N')
    {
        // Iterates through knight movements
        std::array<Square * Square::*, 8> knightDirections = {
            &Square::knightUpLeft, &Square::knightUpRight,
            &Square::knightDownLeft, &Square::knightDownRight,
            &Square::knightLeftUp, &Square::knightLeftDown,
            &Square::knightRightUp, &Square::knightRightDown};

        for (Square *Square::*direction : knightDirections)
        {
            curr = move->getEnd()->*direction;

            // If find King
            if (curr == kings[!whiteTurn]->getSquare())
            {
                movedPieceChecks = true;
                break;
            }
        }
    }
    // If piece moved is pawn
    else if (move->getPiece() >= 'a' && move->getPiece() <= 'h')
    {
        if (whiteTurn)
        {
            // Checks if pawn is up left or up right from King
            if (kings[!whiteTurn]->getSquare()->upLeft == move->getEnd() ||
                kings[!whiteTurn]->getSquare()->upRight == move->getEnd())
            {
                movedPieceChecks = true;
            }
        }
        else
        {
            // Checks if pawn is down left or down right from King
            if (kings[!whiteTurn]->getSquare()->downLeft == move->getEnd() ||
                kings[!whiteTurn]->getSquare()->downRight == move->getEnd())
            {
                movedPieceChecks = true;
            }
        }
    }
    // If any other piece has moved
    else
    {
        // Stores how far moved piece is from King col and row wise
        int rowMove = move->getEnd()->row - kings[!whiteTurn]->getSquare()->row;
        int colMove = move->getEnd()->col - kings[!whiteTurn]->getSquare()->col;

        // Basic check to ensure it is geometrically possible to move from King to piece
        if (rowMove == 0 || colMove == 0 || std::abs(colMove) == std::abs(rowMove))
        {
            // Stores array index used to access direction
            int rowStep = (rowMove > 0) - (rowMove < 0) + 1;
            int colStep = (colMove > 0) - (colMove < 0) + 1;

            // Start at Kings square
            curr = kings[!whiteTurn]->getSquare()->*directions[rowStep][colStep];

            // Move in direction of moved piece, stopping when we run into a piece
            while (!curr->piece)
            {
                curr = curr->*directions[rowStep][colStep];
            }

            // If King sees piece and piece attacks
            if (curr == move->getEnd() &&
                (((!rowMove || !colMove) && curr->piece->attackOrthogonal()) || // If moves orthogonally and attacks orthogonally
                 ((rowMove && colMove) && curr->piece->attackDiagonal())))      // If moves diagonally and attacks diagonally
            {
                movedPieceChecks = true;
            }
        }
    }

    // If moved piece checks and can't be captured, return true
    if (movedPieceChecks && pieceCantBeCaptured(move->getEnd()))
    {
        return true;
    }

    // Checks for discovered checks

    // Stores how far moved piece starting square is from King col and row wise
    int rowMove = move->getStartRow() - kings[!whiteTurn]->getSquare()->row;
    int colMove = move->getStartCol() - kings[!whiteTurn]->getSquare()->col;

    // Basic check to ensure it is geometrically possible to move from King to any discovered checks
    if (rowMove == 0 || colMove == 0 || std::abs(colMove) == std::abs(rowMove))
    {
        // Stores array index used to access direction
        int rowStep = (rowMove > 0) - (rowMove < 0) + 1;
        int colStep = (colMove > 0) - (colMove < 0) + 1;

        // Start at Kings square
        curr = kings[!whiteTurn]->getSquare()->*directions[rowStep][colStep];

        // Move in direction of moved piece starting square, stopping when we run into a piece or get to the end of the board
        while (curr && !curr->piece)
        {
            curr = curr->*directions[rowStep][colStep];
        }

        // If King sees an opposing piece and piece attacks
        if (curr && curr->piece && curr->piece->getIsWhite() == whiteTurn &&
            (((!rowMove || !colMove) && curr->piece->attackOrthogonal()) || // If moves orthogonally and attacks orthogonally
             ((rowMove && colMove) && curr->piece->attackDiagonal())))      // If moves diagonally and attacks diagonally
        {
            if (movedPieceChecks || pieceCantBeCaptured(curr))
            {
                return true;
            }

            movedPieceChecks = true; // In case of rare double discovered check
        }
    }

    // En passant discovered check
    if (move->getEnPassant())
    {
    }

    return false;
}

// Checks if piece can be captured
bool Board::pieceCantBeCaptured(Square *pieceSquare) const
{
    return !squareAttacked(pieceSquare, pieceSquare->piece->getIsWhite());
}