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

    int rand = Utils::randomInt(0, 4);

    if (rand == 0) ad->Load("anim/calvo.anim");
    else if (rand == 1) ad->Load("anim/tupe.anim");
    else if (rand == 2) ad->Load("anim/gordo.anim");
    else if (rand == 3) ad->Load("anim/moderno.anim");
    else {
        if (Utils::randomInt(0, 1)) ad->Load("anim/rubiaca.anim");
        else  ad->Load("anim/morenaca.anim");
    }

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
    Npc::Update();

    InputEng* input = InputEng::getInstance();
    float delta = input->getFrameTime().asSeconds();

    switch(m_state)
    {
    case STATE_WALKING:
    {
        m_mark = MARK_NONE;
        vec2 position = m_position;
        City &city = *GameReg::getInstance()->city;

        if(!m_hasGoal)
            setGoal(city.getRandomStreet());
        moveTowardsGoal();

        std::list<Person>* personList = GameReg::getInstance()->personList;
        for (std::list<Person>::iterator it = personList->begin(); it != personList->end() && m_state != STATE_PANIC; it++) {
            Person &person = *it;
            if (person.is_alive()) continue;
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

        if (m_faceDir == FACE_LEFT) m_scale = sf::Vector2f(-1, 1);
        if (m_faceDir == FACE_RIGHT) m_scale = sf::Vector2f(1, 1);
    }
        break;
    case STATE_PANIC:
        if (m_panicTime > 0) m_panicTime -= delta;
        else m_state = STATE_WALKING;
        m_mark = MARK_EXCLAMATION;
        break;
    case STATE_DEAD:
        m_mark = MARK_NONE;
        m_prio = -1;
        deathTimer -= input->getFrameTime().asSeconds();
        if (deathTimer < 0) {
            GameReg* gameReg = GameReg::getInstance();
            gameReg->eventQueue.push(new EventDeletePerson(this));
        }
        break;
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
}

void Person::doDeath() {

    deathTimer = DISSAPPEAR_TIME;
    m_state = STATE_DEAD;
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

    if (m_state != STATE_DEAD) {
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

bool Person::is_alive()
{
    return m_state != STATE_DEAD;
}
