#pragma once

#include <queue>
#include "event.h"

#include "city.h"
#include "police.h"
#include "item.h"
#include "person.h"

#include <list>

#include "player.h"

class GameReg {
public:
	GameReg();
        static GameReg* getInstance();

        //Event Queue
        std::queue<Event*> eventQueue;

        City *city;
        Player* player;


        std::list<Person>* personList;
        std::list<Police>* policeList;
        std::list<Item>* itemList;

private:
        static GameReg* myInstance;
        static bool imInstanced;
        static void deleteInstance();
};
