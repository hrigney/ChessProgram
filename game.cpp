#include "game.h"

Game::Game() : move(0), prevMove(nullptr), board(Board())
{
}

Game::~Game()
{
    Move *curr = prevMove;
    Move *toDelete = nullptr;

    // Deletes all dynamically allocated moves
    while (curr)
    {
        toDelete = curr;
        curr = curr->getPrevMove();
        delete toDelete;
    }
}

Board &Game::getBoard()
{
    return board;
}

void Game::startGame()
{
    // Start of game text
    std::cout << "GAME STARTED" << std::endl;
    std::cout << std::endl;
    board.displayBoard();
    std::cout << std::endl;

    // Starts prompt move cycle
    promptMove();
}

void Game::promptMove()
{

    // Stores the new move
    Move *newMove = nullptr;

    while (!newMove)
    {
        // Requests a move
        std::cout << "Enter move: ";
        std::string input;
        std::getline(std::cin, input);

        // Attempts to create a new move using input
        newMove = board.requestMove(input, prevMove);
    }
    std::cout << std::endl;
    board.displayBoard();
    std::cout << std::endl;

    // Stores newMove
    prevMove = newMove;

    // Prompts new move
    promptMove();
}