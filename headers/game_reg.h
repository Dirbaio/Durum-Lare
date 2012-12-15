#pragma once

#include <queue>
#include "event.h"

#include "city.h"

class GameReg {
public:
	GameReg();
        static GameReg* getInstance();

        //Event Queue
        std::queue<Event*> eventQueue;
	City *city;

private:
        static GameReg* myInstance;
        static bool imInstanced;
        static void deleteInstance();
};
