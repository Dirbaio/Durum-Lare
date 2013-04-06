#include "item_factory.h"
#include "item.h"

#include "graphics_engine.h"

#include "utils.h"

namespace ItemFactory{

Item* MakeNewItem(GameScene* sc, int type) {

    GraphEng* graphics = GraphEng::getInstance();

    switch(type) {

    case ITEM_NONE: {

        Item* item = new Item(sc, ITEM_NONE);
        item->Init();

        sf::Sprite spr;
        spr.setTexture(*graphics->getTexture("img/items.png"));
        spr.setOrigin(8, 8);
        spr.setTextureRect(sf::IntRect(0*16, 0*16, 16, 16));
        item->setSprite(spr);
        item->setValue( 0 );
        return item;

        break;
    }

    case ITEM_MONEY: {

        Item* item = new Item(sc, ITEM_MONEY);
        item->Init();

        sf::Sprite spr;
        spr.setTexture(*graphics->getTexture("img/items.png"));
        spr.setOrigin(8, 8);
        spr.setTextureRect(sf::IntRect(0*16, 0*16, 16, 16));
        item->setSprite(spr);
        item->setValue( 1 );
        return item;

        break;
    }

    case ITEM_BIG_MONEY: {

        Item* item = new Item(sc, ITEM_BIG_MONEY);
        item->Init();

        sf::Sprite spr;
        spr.setTexture(*graphics->getTexture("img/items.png"));
        spr.setOrigin(8, 8);
        spr.setTextureRect(sf::IntRect(1*16, 0*16, 16, 16));
        item->setSprite(spr);
        item->setValue( 5 );
        return item;

        break;
    }

    case ITEM_SHOE: {

        Item* item = new Item(sc, ITEM_SHOE);
        item->Init();

        sf::Sprite spr;
        spr.setTexture(*graphics->getTexture("img/items.png"));
        spr.setOrigin(8, 8);
        spr.setTextureRect(sf::IntRect(0*16, 1*16, 16, 16));
        item->setSprite(spr);
        item->setValue( 150 );
        return item;

        break;
    }

    case ITEM_GLASSES: {

        Item* item = new Item(sc, ITEM_GLASSES);
        item->Init();

        sf::Sprite spr;
        spr.setTexture(*graphics->getTexture("img/items.png"));
        spr.setOrigin(8, 8);
        spr.setTextureRect(sf::IntRect(1*16, 1*16, 16, 16));
        item->setSprite(spr);
        item->setValue( 200 );
        return item;

        break;
    }

    case ITEM_BURGER: {

        Item* item = new Item(sc, ITEM_BURGER);
        item->Init();

        sf::Sprite spr;
        spr.setTexture(*graphics->getTexture("img/items.png"));
        spr.setOrigin(8, 8);
        spr.setTextureRect(sf::IntRect(1*16, 2*16, 16, 16));
        item->setSprite(spr);
        item->setValue( 45 );
        return item;

        break;
    }

    }

}

}
