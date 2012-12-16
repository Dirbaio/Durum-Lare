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

                sf::Sprite spr;
                spr.setTexture(*graphics->getTexture("img/money.png"));
                spr.setOrigin(spr.getTextureRect().width/2, spr.getTextureRect().height/2);
                item->setSprite(spr);
                item->setScore(Utils::randomInt(1, 10));
                return item;

                break;

        }

    }

}
