#include <iostream>
#include "src/Game/Game.h"
#include <random>

using namespace std;

int main(int argc, char** argv) {
    random_device rd;
    mt19937 gen(rd());
    Game game;
    game.Initialize(gen);
    game.RunLoop();
    game.Shutdown();


    
    return 0;
}