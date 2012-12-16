#include "person.h"
#include "graphics_engine.h"
#include "npc.h"
#include "defines.h"
#include "input_engine.h"
#include "game_reg.h"
#include "utils.h"
#include "animation.h"

void Person::Init() {

    GraphEng* graphics = GraphEng::getInstance();

    //mySpr.setTexture(*graphics->getTexture("img/person.png"));
    deadSpr.setTexture(*graphics->getTexture("img/person_dead.png"));
    deadSpr.setOrigin(deadSpr.getTextureRect().width*0.5f,
                      deadSpr.getTextureRect().height*0.5f);


    DISSAPPEAR_TIME = 12.0f;
    m_walkingTime = 0.0f;

    life = 1;

    transHit = NULL;

    AnimationData* ad = new AnimationData();
    ad->Load("anim/calvo.anim");
    if (m_anim == NULL) m_anim = new Animation();
    m_anim->setAnimData(ad);
    m_anim->SelectAnim("Walking");

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

    if (transHit != NULL) {
        transHit->update(delta);
        if (transHit->reached()) {
            delete transHit;
            transHit = NULL;
        }
    }
    Character::move(pos);

    m_anim->Update(delta);
}

void Person::doDeath() {

    deathTimer = DISSAPPEAR_TIME;
    alive = false;
}

void Person::onHit() {

    life--;

    if (life <= 0) doDeath();

    if (transHit != NULL) delete transHit;
    transHit = new TransitionLinear();

    transHit->setPos(m_scale.x);
    transHit->goPos(m_scale.x*1.5f);
    transHit->setTime(0.05f);
}

void Person::Draw() {

    sf::Sprite* spr = m_anim->getCurrentFrame();

    if (alive) {
        if (transHit != NULL) spr->setScale( sf::Vector2f(transHit->getPos(), transHit->getPos()) );
        else spr->setScale(sf::Vector2f(1.0f, 1.0f));
        spr->setPosition(m_position);
        App->draw(*spr);
    }
    else {
        deadSpr.setPosition(m_position);
        App->draw(deadSpr);
    }
}
