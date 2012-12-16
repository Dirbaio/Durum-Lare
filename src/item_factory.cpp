#include "item_factory.h"
#include "item.h"

#include "graphics_engine.h"

#include "utils.h"

namespace ItemFactory{

    Item* MakeNewItem(int type) {

        GraphEng* graphics = GraphEng::getInstance();

        switch(type) {

            case ITEM_MONEY:

                Item* item = new Item();
                item->Init();
                item->setTakeable(true);
                sf::Sprite spr;
                spr.setTexture(*graphics->getTexture("img/money.png"));
                item->setSprite(spr);
                item->setScore(Utils::randomInt(1, 20)*5);
                return item;

                break;

        }

    }

};
