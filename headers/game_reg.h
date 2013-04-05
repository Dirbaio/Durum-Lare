#pragma once

#include <queue>
#include "event.h"

#include "city.h"
#include "police.h"
#include "item.h"
#include "person.h"
#include "shop.h"

#include <list>

#include "player.h"

class GameScene;

class GameReg {
public:
	GameReg();
        static GameReg* getInstance();

        //Event Queue
        std::queue<Event*> eventQueue;

        City *city;
        Player* player;
        GameScene* scene;
        
        std::list<Person>* personList;
        std::list<Police>* policeList;
        std::list<Item>* itemList;
        std::list<Shop>* shopList;

private:
        static GameReg* myInstance;
        static bool imInstanced;
        static void deleteInstance();
};
