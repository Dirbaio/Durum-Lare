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

<<<<<<< HEAD
    DISSAPPEAR_TIME = 12.0f;
=======
    DISSAPPEAR_TIME = 3.0f;
    m_walkingTime = 0.0f;
>>>>>>> cd6355c5d29861d90381e4466ef0207adcd10899
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

    float delta = input->getFrameTime().asSeconds();

    m_walkingTime -= delta;

    if (m_walkingTime < 0) {
	    m_faceDir = Utils::randomInt(0, FACE_SIZE-1);
	    m_walkingTime = Utils::randomInt(0, 2000)/1000.0f;
    }

    sf::Vector2f pos = m_position;
    m_vel = 16.0f;

    switch (m_faceDir) {
    case FACE_UP:
	    pos.y -= delta * m_vel;
	    break;
    case FACE_DOWN:
	    pos.y += delta * m_vel;
	    break;
    case FACE_LEFT:
	    pos.x -= delta * m_vel;
	    break;
    case FACE_RIGHT:
	    pos.x += delta * m_vel;
	    break;
    }

    Character::move(pos);

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
