#include "npc.h"
#include "defines.h"
#include "animation.h"
#include "input_engine.h"

void Npc::Init() {

    //Empty



}

void Npc::Update() {

    updateBBox();



    m_anim->Update(InputEng::getInstance()->getFrameTime().asSeconds());

}

void Npc::Draw() {

    if (!alive) return;

    //sf::Sprite* spr = m_anim->getCurrentFrame();

    mySpr.setPosition(m_position);
    App->draw(mySpr);


}

