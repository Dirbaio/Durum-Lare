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
    Npc::Update();

    InputEng* input = InputEng::getInstance();
    float delta = input->getFrameTime().asSeconds();

    City &city = *GameReg::getInstance()->city;
    std::list<Person>* personList = GameReg::getInstance()->personList;

    switch(m_state)
    {
    case STATE_WALKING:
    {
        m_mark = MARK_NONE;

        if(!m_hasGoal)
            setGoal(city.getRandomStreet());
        moveTowardsGoal();

        for (std::list<Person>::iterator it = personList->begin(); it != personList->end() && m_state != STATE_PANIC; it++)
            if (!it->is_alive() && canSee(it->m_position))
            {
                m_state = STATE_PANIC;
                m_panicTime = m_startPanicTime;
            }

        if (m_faceDir == FACE_LEFT) m_scale = sf::Vector2f(-1, 1);
        if (m_faceDir == FACE_RIGHT) m_scale = sf::Vector2f(1, 1);
    }
        break;
    case STATE_PANIC:
    {
        if (m_panicTime > 0) m_panicTime -= delta;
        else m_state = STATE_WALKING;
        m_mark = MARK_EXCLAMATION;

        vec2i now = city.absoluteToTilePos(m_position);
        vec2i best = now;
        float bestd = 0;
        for(int i = 0; i < 4; i++)
        {
            vec2i lol = now + dirInc[i];
            if(city.occupedIJ(lol.x, lol.y)) continue;

            float d = 1000000;

            for (std::list<Person>::iterator it = personList->begin(); it != personList->end(); it++)
                if (!it->is_alive())
                {
                    float d2 = Utils::distance(vec2(lol.x*64+32, lol.y*64+32), it->m_position);
                    d = min(d, d2);
                }

            cout<<d<<endl;
            if(d > bestd)
            {
                bestd = d;
                best = lol;
            }
        }
        float velbak = m_vel;
        m_vel = 70;
        moveInDir(vec2(best.x*64+32, best.y*64+32) - m_position);
        m_vel = velbak;
    }
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
