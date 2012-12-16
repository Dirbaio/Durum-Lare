#include "person.h"
#include "graphics_engine.h"
#include "npc.h"
#include "defines.h"
#include "input_engine.h"
#include "game_reg.h"
#include "utils.h"
#include "animation.h"

#include <SFML/Audio.hpp>

void Person::Init() {

    GraphEng* graphics = GraphEng::getInstance();


    //mySpr.setTexture(*graphics->getTexture("img/person.png"));
    deadSpr.setTexture(*graphics->getTexture("img/person_dead.png"));
    deadSpr.setOrigin(deadSpr.getTextureRect().width*0.5f,
                      deadSpr.getTextureRect().height*0.5f);


    DISSAPPEAR_TIME = 12.0f;
    m_walkingTime = 0.0f;

    life = 1;
    m_startPanicTime = 10.0f;

    m_state = STATE_WALKING;

    transHit = NULL;

    AnimationData* ad = new AnimationData();

    if (Utils::randomInt(0, 1)) ad->Load("anim/calvo.anim");
    else ad->Load("anim/tupe.anim");

    if (m_anim == NULL) m_anim = new Animation();
    m_anim->setAnimData(ad);
    m_anim->SelectAnim("Walking");

    dieSoundBuff.loadFromFile("audio/wilhelmscream.ogg");
    dieSound.setBuffer(dieSoundBuff);
    dieSound.setLoop(false);
    //dieSound.setPitch(1.5f);
    dieSound.setVolume(10000.0f);
    m_vel = 16.0f*1.25f * Utils::randomInt(750, 2000)/1000.0f;
}

void Person::Update() {

    InputEng* input = InputEng::getInstance();

    Npc::Update();

    if (!alive) {
        m_prio = -1;
        deathTimer -= input->getFrameTime().asSeconds();
        m_state = STATE_DEAD;
        if (deathTimer < 0) {
            GameReg* gameReg = GameReg::getInstance();
            gameReg->eventQueue.push(new EventDeletePerson(this));
        }
        return;
    }

    float delta = input->getFrameTime().asSeconds();

    std::list<Person>* personList = GameReg::getInstance()->personList;


    if (m_panicTime > 0) m_panicTime -= delta;
    else m_state = STATE_WALKING;

    vec2 position = m_position;
    City &city = *GameReg::getInstance()->city;

    if (m_state == STATE_WALKING) {
        for (std::list<Person>::iterator it = personList->begin(); it != personList->end() && m_state != STATE_PANIC; it++) {
            Person &person = *it;
            if (person.alive) continue;
            else {
                vec2 pos = person.m_position;
                vec2 dir_corpse = pos-position;
                vec2 dir_facing (dirInc[m_faceDir].x,dirInc[m_faceDir].y);
                Utils::normalize(dir_corpse);
                Utils::normalize(dir_facing);
                if (city.visible(pos,position) && Utils::dot2(dir_corpse,dir_facing) >= 0.5f) {
                    m_state = STATE_PANIC;
                    m_panicTime = m_startPanicTime;
                }
            }
        }
    }

    //LE OLD CODE
    /*

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
            flag_draw_mirror = true;
	    break;
    case FACE_RIGHT:
	    pos.x += delta * m_vel;
            flag_draw_mirror = false;
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
    */

    if (m_state == STATE_WALKING) {
        City &city = *GameReg::getInstance()->city;

        if(!m_hasGoal)
            setGoal(city.getRandomStreet());
        moveTowardsGoal();

        if (m_faceDir == FACE_LEFT) m_scale = sf::Vector2f(-1, 1);
        if (m_faceDir == FACE_RIGHT) m_scale = sf::Vector2f(1, 1);
    }

}

void Person::doDeath() {

    deathTimer = DISSAPPEAR_TIME;
    alive = false;
}

void Person::onHit() {

    dieSound.play();

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
        spr->setScale(m_scale);

        App->draw(*spr);
    }
    else {
        deadSpr.setPosition(m_position);
        App->draw(deadSpr);
    }
}
