#include "person.h"
#include "graphics_engine.h"

#include "defines.h"

void Person::Init() {

    GraphEng* graphics = GraphEng::getInstance();

    mySpr.setTexture(*graphics->getTexture("img/person.png"));

}

void Person::Update() {

    //TO DO

}
