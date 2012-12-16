#include "item.h"
#include "defines.h"

void Item::Init() {



}


void Item::Update() {



}

void Item::Draw() {

    mySpr.setPosition(m_position);
    App->draw(mySpr);

}

void Item::setSprite(sf::Sprite spr) {

    mySpr = spr;

}
