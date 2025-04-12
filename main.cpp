
#include "Game.h"

int main(int argc, char* argv[])
{
    Game game;
    game.init();
    game.run();
    game.cleanUp();
    return 0;
}
