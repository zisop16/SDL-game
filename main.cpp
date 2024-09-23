#include <iostream>
#include "src/Game/Game.h"


using namespace std;

int main(int argc, char** argv) {
    Game game;
    game.Initialize();
    game.RunLoop();
    game.Shutdown();


    
    return 0;
}