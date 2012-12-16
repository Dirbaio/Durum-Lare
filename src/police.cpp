#include "police.h"
#include "graphics_engine.h"

#include "defines.h"

void Police::Init() {

    GraphEng* graphics = GraphEng::getInstance();

    mySpr.setTexture(*graphics->getTexture("img/police.png"));
    mySpr.setOrigin(mySpr.getTextureRect().width*0.5f,
                      mySpr.getTextureRect().height*0.5f);

}

void Police::Update() {

    Npc::Update();

    //TO DO

}
