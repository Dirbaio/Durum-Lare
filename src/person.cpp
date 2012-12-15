#include "person.h"
#include "graphics_engine.h"

#include "defines.h"

void Person::Init() {

    GraphEng* graphics = GraphEng::getInstance();

    mySpr.setTexture(*graphics->getTexture("img/person.png"));
    deadSpr.setTexture(*graphics->getTexture("img/person_dead.png"));
}

void Person::Update() {

    Npc::Update();

    //TO DO

}

void Person::Draw() {

    if (alive) {
        Npc::Draw();
    }
    else {
        deadSpr.setPosition(m_position);
        App->draw(deadSpr);
    }
}
