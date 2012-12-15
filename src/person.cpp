#include "person.h"
#include "graphics_engine.h"
#include "npc.h"
#include "defines.h"
#include "input_engine.h"
#include "game_reg.h"
#include "utils.h"

void Person::Init() {

    GraphEng* graphics = GraphEng::getInstance();

    mySpr.setTexture(*graphics->getTexture("img/person.png"));
    deadSpr.setTexture(*graphics->getTexture("img/person_dead.png"));

    DISSAPPEAR_TIME = 3.0f;
}

void Person::Update() {

    InputEng* input = InputEng::getInstance();

    Npc::Update();

    if (!alive) {
        deathTimer -= input->getFrameTime().asSeconds();
        if (deathTimer < 0) {
            GameReg* gameReg = GameReg::getInstance();
            gameReg->eventQueue.push(new EventDeletePerson(this));
        }
        return;
    }

    m_position.x += input->getFrameTime().asSeconds()*400.0f*Utils::randomInt(-1, 1);
    m_position.y += input->getFrameTime().asSeconds()*400.0f*Utils::randomInt(-1, 1);

}

void Person::doDeath() {

    deathTimer = DISSAPPEAR_TIME;
    alive = false;
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
