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


#define NUM_ANIMS_DATA 4

AnimationData* s_person_data[NUM_ANIMS_DATA] = {
	NULL,
	NULL,
	NULL,
        NULL
};

char* s_person_dataFilenames[NUM_ANIMS_DATA] = {
	"anim/calvo.anim",
	"anim/tupe.anim",
	"anim/gordo.anim",
        "anim/rubiaca.anim"
};

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

    m_origin = sf::Vector2f(16, 16);
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


    //dieSoundBuff.loadFromFile("audio/wilhelmscream.ogg");
    //dieSound.setBuffer(dieSoundBuff);
    //dieSound.setLoop(false);
    //dieSound.setPitch(1.5f);
    //dieSound.setVolume(10000.0f);
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
    Player* p = GameReg::getInstance()->player;
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

        if (m_confuseCooldown <= 0.0f) {
            for(int i = 0; i < v.size(); i++)
            {
                m_state = STATE_CONFUSED;
                m_confusedTime = Utils::randomInt(1,2);
                m_confusedTimeFacing = Utils::randomInt(1, 3)/4.0;
		m_confuseCooldown = Utils::randomInt(12,17);
            }
        }
        else {
            m_confuseCooldown -= delta;
        }

        if(canSee(p->getPosition())) {
            if (p->isDoingAction()) {
                m_state = STATE_PANIC;
                m_panicTime = m_startPanicTime;
            }
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

    case STATE_CONFUSED: {
        m_mark = MARK_BLUE_QUESTION;

        m_vel = 20.0f;
        m_confusedTime -= delta;
        m_confusedTimeFacing -= delta;

        if (m_confusedTime < 0) {
            setGoal(city.getRandomStreet());
            m_state = STATE_WALKING;
        }

        if (m_confusedTimeFacing < 0) {
            lookAtRandomPlace();
	    m_confusedTimeFacing = Utils::randomInt(1, 3)/4.0;
        }

	    vector<Person*> v = GameReg::getInstance()->scene->getPeopleSeen(this, SEARCH_DEAD);
		for(int i = 0; i < v.size(); i++)
        {
            m_state = STATE_PANIC;
            m_panicTime = m_startPanicTime;

            if (Utils::distance(v[i]->m_position, m_lastSawPlayer) < 70) knows_player = true;
        }

        if (knows_player) {
            if(canSee(p->getPosition())) {
                m_state = STATE_PANIC;
                m_panicTime = m_startPanicTime;
            }
        }

        if(canSee(p->getPosition())) {
            if (p->isDoingAction()) {
                m_state = STATE_PANIC;
                m_panicTime = m_startPanicTime;
            }
        }

        break;
	}
    case STATE_DEAD: {
        if (m_faceDir == FACE_UP) ensureAnim("DeadUp");
        else if (m_faceDir == FACE_DOWN) ensureAnim("DeadDown");
        else if (m_faceDir == FACE_LEFT) ensureAnim("DeadLeft");
        else if (m_faceDir == FACE_RIGHT) ensureAnim("DeadRight");
        m_mark = MARK_NONE;
        m_prio = -1;
        deathTimer -= input->getFrameTime().asSeconds();
        if (deathTimer < 0)
        	markForDelete();

        break;
    }
    }

    if (m_state != STATE_DEAD) {
        if (m_faceDir == FACE_UP) ensureAnim("WalkingUp");
        else if (m_faceDir == FACE_DOWN) ensureAnim("WalkingDown");
        else if (m_faceDir == FACE_LEFT) ensureAnim("WalkingLeft");
        else if (m_faceDir == FACE_RIGHT) ensureAnim("WalkingRight");
    }

}

void Person::doDeath() {

    deathTimer = DISSAPPEAR_TIME;
    m_state = STATE_DEAD;

    m_boundbox.left = m_boundbox.left - m_boundbox.width ;
    m_boundbox.width = (m_boundbox.width *2);
    m_boundbox.top = m_boundbox.top - m_boundbox.height ;
    m_boundbox.height = (m_boundbox.height *2);

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

    //if (transHit != NULL) spr->setScale( sf::Vector2f(transHit->getPos(), transHit->getPos()) );
    //else spr->setScale(sf::Vector2f(1.0f, 1.0f));

    spr->setOrigin(m_origin);
    spr->setPosition(m_position);
    spr->setScale(m_scale);
    App->draw(*spr);


}

void Person::lookAtRandomPlace()
{
    City &city = *GameReg::getInstance()->city;
    vec2i v = city.absoluteToTilePos(m_position);

    int lastFaceDir = m_faceDir;
    int i = 0;
    while(i < 8) {
        m_faceDir = Utils::randomInt(FACE_UP, FACE_RIGHT);
        vec2i v2 = v + dirInc[m_faceDir];
	if(!city.occupedIJ(v2.x, v2.y) && m_faceDir != lastFaceDir) break;
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
