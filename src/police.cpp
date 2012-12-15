#include "police.h"
#include "graphics_engine.h"

#include "defines.h"

void Police::Init() {

    GraphEng* graphics = GraphEng::getInstance();

    mySpr.setTexture(*graphics->getTexture("img/police.png"));

}

void Police::Update() {

    Npc::Update();

    //TO DO

}
