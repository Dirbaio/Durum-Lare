#include "npc.h"
#include "defines.h"

void Npc::Init() {

    //Empty

}

void Npc::Update() {

    updateBBox();

}

void Npc::Draw() {

    if (!alive) return;

    mySpr.setPosition(m_position);
    App->draw(mySpr);

}
