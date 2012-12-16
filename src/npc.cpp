#include "npc.h"
#include "defines.h"
#include "animation.h"

void Npc::Init() {

    //Empty



}

void Npc::Update() {

    updateBBox();

}

void Npc::Draw() {

    if (!alive) return;

    //sf::Sprite* spr = m_anim->getCurrentFrame();

    mySpr.setPosition(m_position);
    App->draw(mySpr);


}

