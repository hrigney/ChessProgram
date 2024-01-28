#include "game.h"

Game::Game()
{
    move = 0;
    lastMove = nullptr;
    board = Board();
}

Board &Game::getBoard()
{
    return board;
}