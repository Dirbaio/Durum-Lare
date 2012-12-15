#pragma once

#include <queue>
#include "event.h"

class GameReg {
public:
        GameReg();
        static GameReg* getInstance();

        //Event Queue
        std::queue<Event*> eventQueue;

private:
        static GameReg* myInstance;
        static bool imInstanced;
        static void deleteInstance();
};
