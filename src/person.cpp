#include "person.h"
#include "graphics_engine.h"
#include "npc.h"
#include "defines.h"
#include "input_engine.h"
#include "game_reg.h"
#include "utils.h"
#include "animation.h"
#include "game_scene.h"
#include <SFML/Audio.hpp>

void Person::Init() {

    GraphEng* graphics = GraphEng::getInstance();
    //mySpr.setTexture(*graphics->getTexture("img/person.png"));
    bloodSpr.setTexture(*graphics->getTexture("img/blood.png"));
    bloodSpr.setOrigin(bloodSpr.getTextureRect().width*0.5f,
                      bloodSpr.getTextureRect().height*0.5f);

    DISSAPPEAR_TIME = 12.0f;
    m_walkingTime = 0.0f;

    life = 1;
    m_startPanicTime = 10.0f;
    m_state = STATE_WALKING;

    knows_player = false;

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
    ix = Utils::randomInt(8, 56);
    iy = Utils::randomInt(8, 56);
}


float Person::getClosestMenace(vec2 pos, vec2& menacePos)
{
    menacePos = m_lastSawPlayer;
    float d = Utils::distance(pos, menacePos)/2;

    vector<Person*> v = GameReg::getInstance()->scene->getPeopleSeen(this, SEARCH_DEAD);
	for(int i = 0; i < v.size(); i++)
    {
        float d2 = Utils::distance(pos, v[i]->getPosition());
        if(d2 < d)
        {
            d = d2;
            menacePos = v[i]->getPosition();
        }
    }

    return d;
}


void Person::Update() {
    Npc::Update();

    InputEng* input = InputEng::getInstance();
    float delta = input->getFrameTime().asSeconds();

    City &city = *GameReg::getInstance()->city;

    vec2 currPlayerPosition = GameReg::getInstance()->player->getPosition();
    bool seesPlayerNow = canSee(currPlayerPosition);
    if(seesPlayerNow)
    	m_lastSawPlayer = currPlayerPosition;
	
    switch(m_state)
    {
    case STATE_WALKING:
    {
        m_mark = MARK_NONE;

        if(!m_hasGoal)
            setGoal(city.getRandomStreet());
        moveTowardsGoal();

        if (knows_player && seesPlayerNow)
        {
            m_state = STATE_PANIC;
            m_panicTime = m_startPanicTime;
        }

	    vector<Person*> v = GameReg::getInstance()->scene->getPeopleSeen(this, SEARCH_DEAD);
		for(int i = 0; i < v.size(); i++)
        {
            m_state = STATE_PANIC;
            m_panicTime = m_startPanicTime;

            if (Utils::distance(v[i]->m_position, m_lastSawPlayer) < 70) knows_player = true;
        }

	    v = GameReg::getInstance()->scene->getPeopleSeen(this, SEARCH_PANIC);
		cout<<v.size()<<endl;
		for(int i = 0; i < v.size(); i++)
        {
            m_state = STATE_CONFUSED;
            m_confusedTime = Utils::randomInt(5, 10);
            m_confusedTimeFacing = Utils::randomInt(1, 2);
        }
    }
        break;

    case STATE_PANIC:
    {
        if (m_panicTime > 0) m_panicTime -= delta;
        else
        {
            m_state = STATE_WALKING;
            m_hasGoal = false;
        }

        m_mark = MARK_EXCLAMATION;

        if (knows_player && seesPlayerNow)
            m_panicTime = m_startPanicTime;

        vec2i now = city.absoluteToTilePos(m_position);
        vec2i best = now;
        vec2 menacePos;
        float bestd = getClosestMenace(m_position, menacePos);

        float velbak = m_vel;
        m_vel = 70;

        if(bestd < 30)
            moveInDir(vec2(m_position - menacePos));
        else
        {
            for(int i = 0; i < 4; i++)
            {
                vec2i lol = now + dirInc[i];
                if(city.occupedIJ(lol.x, lol.y)) continue;

                float d = getClosestMenace(vec2(lol.x*64+32, lol.y*64+32), menacePos);
                if(d > bestd)
                {
                    bestd = d;
                    best = lol;
                }
            }
	    vec2 togo = vec2(best.x*64+ix, best.y*64+iy);
	    vec2 dir  = togo - m_position;
	    if (Utils::norm(dir) != 0) moveInDir(dir);
        }
        m_vel = velbak;
    }
    break;

    case STATE_CONFUSED:
    {
        m_mark = MARK_QUESTION;
        m_vel = 20.0f;
        m_confusedTime -= delta;
        m_confusedTimeFacing -= delta;

        if (m_confusedTime < 0) {
            setGoal(city.getRandomStreet());
            m_state = STATE_WALKING;
        }

        if (m_confusedTimeFacing < 0) {
            lookAtRandomPlace();
            m_confusedTimeFacing = Utils::randomInt(1, 2);
        }

	    vector<Person*> v = GameReg::getInstance()->scene->getPeopleSeen(this, SEARCH_DEAD);
		cout<<v.size()<<endl;
		for(int i = 0; i < v.size(); i++)
        {
            m_state = STATE_PANIC;
            m_panicTime = m_startPanicTime;

            if (Utils::distance(v[i]->m_position, m_lastSawPlayer) < 70) knows_player = true;
        }

        if (knows_player) {
            Player* p = GameReg::getInstance()->player;
            if(canSee(p->getPosition())) {
                m_state = STATE_PANIC;
            }
        }

        break;
	}
    case STATE_DEAD:
        ensureAnim("Dead");
        m_mark = MARK_NONE;
        m_prio = -1;
        deathTimer -= input->getFrameTime().asSeconds();
        if (deathTimer < 0)
        	markForDelete();

        break;
    }

    if (m_faceDir == FACE_LEFT) m_scale = sf::Vector2f(-1, 1);
    if (m_faceDir == FACE_RIGHT) m_scale = sf::Vector2f(1, 1);
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

    if (m_state == STATE_DEAD) {
        bloodSpr.setPosition(m_position);
        App->draw(bloodSpr);
    }

    if (transHit != NULL) spr->setScale( sf::Vector2f(transHit->getPos(), transHit->getPos()) );
    else spr->setScale(sf::Vector2f(1.0f, 1.0f));
    spr->setPosition(m_position);
    spr->setScale(m_scale);
    App->draw(*spr);


}

void Person::lookAtRandomPlace()
{
    City &city = *GameReg::getInstance()->city;
    vec2i v = city.absoluteToTilePos(m_position);

    int i = 0;
    while(i < 4) {
        m_faceDir = Utils::randomInt(FACE_UP, FACE_RIGHT);
        vec2i v2 = v + dirInc[m_faceDir];
        if(!city.occupedIJ(v2.x, v2.y)) break;
        i++;
    }
}

bool Person::is_alive()
{
    return m_state != STATE_DEAD;
}

bool Person::knowsPlayer()
{
    return knows_player;
}
