#include <iostream>

using namespace std;

#include "game.h"
#include <iostream>

int main(int argc, char *argv[]) {
    Game game;
    if (!game.init()) std::cerr << "Error: Juego no iniciado" << std::endl;
    else game.update();

    return 0;
}
