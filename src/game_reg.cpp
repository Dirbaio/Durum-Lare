#include "game_reg.h"
#include <stdlib.h>

GameReg* GameReg::myInstance = 0;
bool GameReg::imInstanced = false;

GameReg::GameReg() {
        if (!imInstanced) {
                atexit(GameReg::deleteInstance);
                imInstanced = true;
        }
}

GameReg* GameReg::getInstance() {
        if (GameReg::myInstance == 0) {
                myInstance = new GameReg();
        }

        return myInstance;
}

void GameReg::deleteInstance()
{
        if (myInstance) delete myInstance;
}
