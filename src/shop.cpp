#include "shop.h"
#include "defines.h"
#include "graphics_engine.h"
#include <iostream>
#include "utils.h"
#include "item.h"
#include "item_factory.h"
#include "input_engine.h"
#include "game_scene.h"

void Shop::Init() {

    GraphEng* graphics = GraphEng::getInstance();

    m_shopSprite.setTexture(*(graphics->getTexture("img/shop.png")));
    m_shopSprite.setOrigin(m_shopSprite.getTextureRect().width/2,
                           m_shopSprite.getTextureRect().height/2);
    m_shopStats.message = "This is a real bargain!";

    Item* item = ItemFactory::MakeNewItem(scene, ITEM_BURGER);
    m_shopStats.items.push_back(*item);
    delete item;

    item = ItemFactory::MakeNewItem(scene, ITEM_GLASSES);
    m_shopStats.items.push_back(*item);
    delete item;

    item = ItemFactory::MakeNewItem(scene, ITEM_SHOE);
    m_shopStats.items.push_back(*item);
    delete item;
}

void Shop::setItem(int n, int type) {
    Item* item = ItemFactory::MakeNewItem(scene, type);
    m_shopStats.items[n] = (*item);
    delete item;
}

void Shop::Update() {

    //TODO FIX
    /*
    GameReg* gameReg = GameReg::getInstance();

    sf::Vector2f pos = m_position;


    m_isPlayerNear = (Utils::distance(gameReg->player->getPosition(), pos) < 32.0f);

    if (!m_isPlayerNear) {
         m_shopStats.message = "This is a real bargain!";
        return;
    }

    int keyBuyItem = -1;

    if (scene->input.getKeyDown(InputEng::SHOP_BUY_1)) keyBuyItem = 0;
    if (scene->input.getKeyDown(InputEng::SHOP_BUY_2)) keyBuyItem = 1;
    if (scene->input.getKeyDown(InputEng::SHOP_BUY_3)) keyBuyItem = 2;

    if (keyBuyItem >= 0 && m_shopStats.items[keyBuyItem].getItemType() != ITEM_NONE) {
        if (m_shopStats.items[keyBuyItem].getValue() > gameReg->player->getMoney()) {
            m_shopStats.message = "Go get more money!";
        }
        else {
                gameReg->player->onBuy(m_shopStats.items[keyBuyItem]);
                setItem(keyBuyItem, ITEM_NONE);
                m_shopStats.message = "Le thanks!";
        }

    }*/

}

void Shop::Draw() {
    m_shopSprite.setPosition(m_position);
    App->draw(m_shopSprite);

}
