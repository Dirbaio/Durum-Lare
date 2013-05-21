#include "npc.h"
#include "animation.h"
#include "input_engine.h"
#include "game_scene.h"

Npc::Npc(GameScene* sc) : Character(sc) {
    m_origin.x = 16;
    m_origin.y = 32;
}

void Npc::Init() {
    //Empty
}

void Npc::Update() {
    updateBBox();
    m_anim->Update(scene->input.getFrameTime().asSeconds());
}
