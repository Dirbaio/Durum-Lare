#include "person.h"
#include "graphics_engine.h"
#include "npc.h"
#include "defines.h"
#include "input_engine.h"
#include "utils.h"
#include "animation.h"
#include "game_scene.h"
#include <SFML/Audio.hpp>

#define NUM_ANIMS_DATA 6
AnimationData* s_person_data[NUM_ANIMS_DATA] = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

const char* s_person_dataFilenames[NUM_ANIMS_DATA] = {
    "anim/calvo.anim",
    "anim/tupe.anim",
    "anim/gordo.anim",
    "anim/rubiaca.anim",
    "anim/morenaca.anim",
    "anim/moderno.anim"
};

void Person::Init() {

    GraphEng* graphics = GraphEng::getInstance();
    //mySpr.setTexture(*graphics->getTexture("img/person.png"));
    m_bloodSpr.setTexture(*graphics->getTexture("img/blood.png"));
    m_bloodSpr.setOrigin((float) m_bloodSpr.getTextureRect().width *0.5f,
                         (float) m_bloodSpr.getTextureRect().height*0.5f);

    m_dissappearTime = 30.0f;
    m_walkingTime = 0.0f;

    life = 1;
    m_startPanicTime = 10.0f;
    m_state = STATE_WALKING;

    m_knowsPlayer = vector<bool>(scene->players.size(), false);
    m_lastSawPlayer = vector<sf::Vector2f>(scene->players.size());
    m_playerActionTime = vector<float>(scene->players.size(), 10000);

    m_transHit = NULL;
    m_confuseCooldown = 0.0f;

    int rand = Utils::randomInt(0, NUM_ANIMS_DATA-1);
    if (s_person_data[rand] == NULL) {
        s_person_data[rand] = new AnimationData();
        s_person_data[rand]->Load(s_person_dataFilenames[rand]);
    }

    AnimationData* ad = s_person_data[rand];
    if (m_anim == NULL) m_anim = new Animation();
    m_anim->setAnimData(ad);
    m_anim->SelectAnim("WalkingDown");

    //m_dieSoundBuff.loadFromFile("audio/wilhelmscream.ogg");
    //m_dieSound.setBuffer(m_dieSoundBuff);
    //m_dieSound.setLoop(false);
    //m_dieSound.setPitch(1.5f);
    //m_dieSound.setVolume(10000.0f);

    m_vel = 16.0f * 1.25f * (float) Utils::randomInt(750, 2000) / 1000.0f;
    ix = Utils::randomInt(8, 56);
    iy = Utils::randomInt(8, 56);
}


float Person::getClosestMenace(sf::Vector2f pos, sf::Vector2f& menacePos)
{
    int menaceCount = 1;
    menacePos.x = 0;
    menacePos.y = 0;
    menacePos += m_panicSource;
    for(int i = 0; i < (int)scene->players.size(); i++)
    {
        if(scene->players[i].m_jailed) continue;
        if(m_knowsPlayer[i] || m_playerActionTime[i] < m_startPanicTime)
        {
            menacePos = m_lastSawPlayer[i]*6.0f;
            menaceCount += 6;
        }
    }
    vector<Person*> v = scene->getPeopleSeen(this, SEARCH_DEAD);
    for(int i = 0; i < (int) v.size(); i++) {
        menacePos += v[i]->getPosition();
        menaceCount++;
    }

    menacePos /= float(menaceCount);
    return Utils::distance(pos, menacePos);
}


void Person::Update() {
    Npc::Update();

    float delta = scene->input.getFrameTime().asSeconds();

    //Player* p = &scene->players[0];

    for(int i = 0; i < (int)scene->players.size(); i++)
    {
        if(scene->players[i].m_jailed) continue;
        m_playerActionTime[i] += delta;
        sf::Vector2f currPlayerPosition = scene->players[i].getPosition();
        bool seesPlayerNow = canSee(currPlayerPosition);
        if(seesPlayerNow)
        {
            if(scene->players[i].isDoingAction())
                m_playerActionTime[i] = 0;
            m_lastSawPlayer[i] = currPlayerPosition;
        }
    }

    switch(m_state)
    {
    case STATE_WALKING:
    {
        m_mark = MARK_NONE;

        if(!m_hasGoal)
            setGoal(scene->city.getRandomStreet());
        moveTowardsGoal();

        for(int i = 0; i < (int)scene->players.size(); i++)
        {
            if(scene->players[i].m_jailed) continue;
            if ((m_knowsPlayer[i] || scene->players[i].isDoingAction()) && canSee(scene->players[i].getPosition()))
            {
                m_state = STATE_PANIC;
                m_panicTime = m_startPanicTime;
                m_panicSource = scene->players[i].getPosition();
            }
        }

        vector<Person*> v = scene->getPeopleSeen(this, SEARCH_DEAD);
        for(int j = 0; j < (int) v.size(); j++)
        {
            m_state = STATE_PANIC;
            m_panicTime = m_startPanicTime;
            m_panicSource = v[j]->getPosition();

            for(int i = 0; i < (int)scene->players.size(); i++)
            {
                if(scene->players[i].m_jailed) continue;
                if (Utils::distance(v[j]->m_position, m_lastSawPlayer[i]) < 70) m_knowsPlayer[i] = true;
            }
        }

        v = scene->getPeopleSeen(this, SEARCH_PANIC);

        if (m_confuseCooldown <= 0.0f) {
            for(int i = 0; i < (int) v.size(); i++)
            {
                m_state              = STATE_CONFUSED;
                m_confusedTime       = (float) Utils::randomInt(2,4);
                m_confusedTimeFacing = (float) Utils::randomInt(1, 3)/4.0;
                m_confuseCooldown    = (float) Utils::randomInt(6,12);
            }
        }
        else {
            m_confuseCooldown -= delta;
        }

        break;
    }
    case STATE_PANIC:
    {
        if (m_panicTime > 0) m_panicTime -= delta;
        else {
            m_state = STATE_WALKING;
            m_hasGoal = false;
        }

        m_mark = MARK_EXCLAMATION;

        for(int i = 0; i < (int)scene->players.size(); i++)
        {
            if(scene->players[i].m_jailed) continue;

            if (m_knowsPlayer[i] && canSee(scene->players[i].getPosition()))
                m_panicTime = m_startPanicTime;
        }

        sf::Vector2i now = scene->city.absoluteToTilePos(m_position);
        sf::Vector2i best = now;
        sf::Vector2f menacePos;
        float bestd = getClosestMenace(m_position, menacePos);

        float velbak = m_vel;
        m_vel = 70;

        if(bestd < 50)
            moveInDir(sf::Vector2f(m_position - menacePos));
        else
        {
            for(int i = 0; i < 4; i++)
            {
                sf::Vector2i lol = now + dirInc[i];
                if(scene->city.occupedIJ(lol.x, lol.y)) continue;

                float d = getClosestMenace(sf::Vector2f(lol.x * 64.0f + 32.0f,
                                                        lol.y * 64.0f + 32.0f),
                                           menacePos);
                if(d > bestd) {
                    bestd = d;
                    best = lol;
                }
            }

            sf::Vector2f togo = sf::Vector2f(best.x*64+ix, best.y*64+iy);
            sf::Vector2f dir  = togo - m_position;
            if (Utils::norm(dir) != 0) moveInDir(dir);
        }
        m_vel = velbak;
        break;
    }
    case STATE_CONFUSED: {
        m_mark = MARK_BLUE_QUESTION;

        m_vel = 20.0f;
        m_confusedTime -= delta;
        m_confusedTimeFacing -= delta;

        if (m_confusedTime < 0) {
            setGoal(scene->city.getRandomStreet());
            m_state = STATE_WALKING;
        }

        if (m_confusedTimeFacing < 0) {
            lookAtRandomPlace();
            m_confusedTimeFacing = Utils::randomInt(1, 3)/4.0f;
        }

        vector<Person*> v = scene->getPeopleSeen(this, SEARCH_DEAD);
        for(int j = 0; j < v.size(); j++) {
            m_state = STATE_PANIC;
            m_panicTime = m_startPanicTime;
            m_panicSource = v[j]->getPosition();
            for(int i = 0; i < (int)scene->players.size(); i++)
                if (Utils::distance(v[j]->m_position, m_lastSawPlayer[i]) < 70) m_knowsPlayer[i] = true;
        }


        for(int i = 0; i < (int)scene->players.size(); i++)
        {
            if(scene->players[i].m_jailed) continue;
            if ((m_knowsPlayer[i] || scene->players[i].isDoingAction()) && canSee(scene->players[i].getPosition()))
            {
                m_state = STATE_PANIC;
                m_panicTime = m_startPanicTime;
                m_panicSource = scene->players[i].getPosition();
            }
        }

        break;
    }
    case STATE_DEAD: {
        m_mark = MARK_NONE;
        m_prio = -1;
        m_deathTimer -= scene->input.getFrameTime().asSeconds();
        if (m_deathTimer < 0) markForDelete();
        break;
    }
    }

    if (m_state == STATE_DEAD) {
        if      (m_faceDir == FACE_UP)    ensureAnim("DeadUp");
        else if (m_faceDir == FACE_DOWN)  ensureAnim("DeadDown");
        else if (m_faceDir == FACE_LEFT)  ensureAnim("DeadLeft");
        else if (m_faceDir == FACE_RIGHT) ensureAnim("DeadRight");
    }
    else if (m_state == STATE_CONFUSED) {
        if      (m_faceDir == FACE_UP)    ensureAnim("IdleUp");
        else if (m_faceDir == FACE_DOWN)  ensureAnim("IdleDown");
        else if (m_faceDir == FACE_LEFT)  ensureAnim("IdleLeft");
        else if (m_faceDir == FACE_RIGHT) ensureAnim("IdleRight");
    }
    else {
        if      (m_faceDir == FACE_UP)    ensureAnim("WalkingUp");
        else if (m_faceDir == FACE_DOWN)  ensureAnim("WalkingDown");
        else if (m_faceDir == FACE_LEFT)  ensureAnim("WalkingLeft");
        else if (m_faceDir == FACE_RIGHT) ensureAnim("WalkingRight");
    }
}

void Person::doDeath() {

    m_deathTimer = m_dissappearTime;
    m_state = STATE_DEAD;

    m_boundbox.left = m_boundbox.left - m_boundbox.width ;
    m_boundbox.width = (m_boundbox.width *2);
    m_boundbox.top = m_boundbox.top - m_boundbox.height ;
    m_boundbox.height = (m_boundbox.height *2);

}

void Person::onHit() {
    //m_dieSound.play();
    life--;

    if (life <= 0) doDeath();
    if (m_transHit != NULL) delete m_transHit;
    m_transHit = new TransitionLinear();

    m_transHit->setPos(m_scale.x);
    m_transHit->goPos(m_scale.x*1.5f);
    m_transHit->setTime(0.05f);
}

void Person::Draw() {

    sf::Sprite* spr = m_anim->getCurrentFrame();

    if (m_state == STATE_DEAD) {
        m_bloodSpr.setPosition(m_position);
        App->draw(m_bloodSpr);
    }

    //if (transHit != NULL) spr->setScale( sf::Vector2f(transHit->getPos(), transHit->getPos()) );
    //else spr->setScale(sf::Vector2f(1.0f, 1.0f));

    // spr->setOrigin(sf::Vector2f(16, 32));
    spr->setPosition(m_position);
    spr->setScale(m_scale);
    App->draw(*spr);


}

void Person::lookAtRandomPlace() {
    sf::Vector2i v = scene->city.absoluteToTilePos(m_position);

    int lastFaceDir = m_faceDir;
    int i = 0;
    while(i < 8) {
        m_faceDir = Utils::randomInt(FACE_UP, FACE_RIGHT);
        sf::Vector2i v2 = v + dirInc[m_faceDir];
        if(!scene->city.occupedIJ(v2.x, v2.y) && m_faceDir != lastFaceDir) break;
        i++;
    }
}

bool Person::is_alive() {
    return m_state != STATE_DEAD;
}

bool Person::knowsPlayer(int i)
{
    return m_knowsPlayer[i];
}
