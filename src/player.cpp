#include "player.h"
#include <SFML/Graphics.hpp>
#include "graphics_engine.h"
#include "defines.h"
#include "input_engine.h"
#include "game_reg.h"

void Player::Init() {

    GraphEng* graphics = GraphEng::getInstance();

    mySpr.setTexture(*graphics->getTexture("img/player.png"));

    m_position.x = 0;
    m_position.y = 0;

    myVel.x = 64*3;
    myVel.y = 64*3;

}

void Player::Update() {

    InputEng* input = InputEng::getInstance();

    if (input->getKeyState(InputEng::PLAYER_UP))
        m_position.y -= myVel.y*input->getFrameTime().asSeconds();
    if (input->getKeyState(InputEng::PLAYER_DOWN))
        m_position.y += myVel.y*input->getFrameTime().asSeconds();
    if (input->getKeyState(InputEng::PLAYER_LEFT))
        m_position.x -= myVel.x*input->getFrameTime().asSeconds();
    if (input->getKeyState(InputEng::PLAYER_RIGHT))
        m_position.x += myVel.x*input->getFrameTime().asSeconds();


    if (input->getKeyDown(InputEng::PLAYER_ACTION)) {

        GameReg* gameReg = GameReg::getInstance();

        gameReg->eventQueue.push(new EventPlayerAction());

    }


}

void Player::Draw() {

    mySpr.setPosition(m_position);
    App->draw(mySpr);

}
