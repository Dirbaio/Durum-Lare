#include "player.h"
#include <SFML/Graphics.hpp>
#include "graphics_engine.h"

void Player::Init() {

    GraphEng* graphics = GraphEng::getInstance();

    mySpr.setTexture(*graphics->getTexture("img/player.png"));
}

void Player::Update() {

}

void Player::Draw() {

}
