#include "common.h"
#include "board.h"
#include "game.h"
#include "pieces.h"

int main()
{
    Game newgame = Game();

    // newgame.getBoard().displayBoard();

    // Move *newMove = newgame.getBoard().requestMove("d4", nullptr);
    // newMove = newgame.getBoard().requestMove("d5", nullptr);
    // newMove = newgame.getBoard().requestMove("Bf4", nullptr);
    // newMove = newgame.getBoard().requestMove("Nf6", nullptr);

    // if (newMove)
    // {
    //     std::cout << "Not nullptr" << std::endl;
    // }
    // else
    // {
    //     std::cout << "nullptr" << std::endl;
    // }

    // newgame.getBoard().displayBoard();

    newgame.startGame();

    return 0;
}