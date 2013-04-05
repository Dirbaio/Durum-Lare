#include <iostream>

using namespace std;

#include "game.h"
#include <iostream>

int main(int argc, char *argv[]) {

    Game game;

    if (!game.Init())
        std::cerr << "Error: Juego no iniciado" << std::endl;

    else game.Go();

    return 0;
}
