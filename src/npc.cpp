#include "npc.h"
#include "defines.h"
#include "animation.h"
#include "input_engine.h"

Npc::Npc() {
    m_origin.x = 16;
    m_origin.y = 32;
}

void Npc::Init() {
    //Empty
}

void Npc::Update() {
    updateBBox();
    m_anim->Update(InputEng::getInstance()->getFrameTime().asSeconds());
}

void Npc::Draw() {
    if (is_alive()) return;

    //sf::Sprite* spr = m_anim->getCurrentFrame();
    mySpr.setPosition(m_position);
    mySpr.setOrigin(m_origin);
    App->draw(mySpr);
}

