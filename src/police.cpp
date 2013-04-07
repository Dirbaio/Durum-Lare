#include "police.h"
#include "graphics_engine.h"

#include "input_engine.h"
#include "game_scene.h"
#include "defines.h"
#include "game_scene.h"

AnimationData* s_police_data = NULL;

void Police::Init() {

    GraphEng* graphics = GraphEng::getInstance();

    mySpr.setTexture(*graphics->getTexture("img/police.png"));

    m_vel = 20.0f;
    m_watchingTime = 0;

    m_lastPosSawTime = vector<float> (scene->players.size());
    m_lastPosSawPlayer = vector<sf::Vector2f> (scene->players.size());
    m_lastDirSawPlayer = vector<sf::Vector2f> (scene->players.size());
    m_knowPlayer = vector<bool> (scene->players.size(), false);

    m_state = STATE_PATROL_WATCHING;

    if (s_police_data == NULL) {
        s_police_data = new AnimationData();
        s_police_data->Load("anim/poli.anim");
    }

    if (m_anim == NULL) m_anim = new Animation();
    m_anim->setAnimData(s_police_data);
    m_anim->SelectAnim("IdleDown");
    m_watchingTimeFacing = 0;
}

void Police::Update() {

    Npc::Update();
    m_collided = false;
    float delta = scene->input.getFrameTime().asSeconds();

    //TODO Multiplayer logic
    Player* p = &scene->players[0];


    std::vector<Person*> personList =scene->getPeopleSeen(this, SEARCH_ANY);
    for (std::vector<Person*>::iterator it = personList.begin(); it != personList.end(); it++) {
        Person &person = *(*it);

        if (canSee(person.getPosition())) {
            switch(person.getState())
            {
            case Person::STATE_PANIC:
            {
                if(m_state == STATE_PATROL_MOVING ||
                        m_state == STATE_PATROL_WATCHING) {
                    if (person.knowsPlayer(0))
                    {
                        m_state = STATE_ALERT;
                        m_alertTime = 20;
                        m_lastAlertPos = person.getPosition();
                        setGoal(m_lastAlertPos);
                    }
                    else
                    {
                        m_state = STATE_CONFUSE;
                        m_alertTime = 15;
                        m_lastAlertPos = person.getPosition();
                        setGoal(m_lastAlertPos);
                    }
                }
                break;
            }
            case Person::STATE_DEAD:
            {
                for(int i = 0; i < (int)scene->players.size(); i++)
                {
                    if(scene->players[i].m_jailed) continue;
                    if(canSee(scene->players[i].getPosition()) &&
                            Utils::distance(person.getPosition(), scene->players[i].getPosition()))
                        m_knowPlayer[i] = true;

                    if (m_knowPlayer[i])
                    {
                        if(m_state == STATE_PATROL_MOVING ||
                                m_state == STATE_PATROL_WATCHING ||
                                m_state == STATE_CONFUSE) {
                            m_state = STATE_ALERT;
                            m_alertTime = 20;
                            m_lastAlertPos = person.getPosition();
                            setGoal(m_lastAlertPos);
                            m_chasingPlayerNum = i;
                        }
                    }
                }

                if(m_state == STATE_PATROL_MOVING ||
                        m_state == STATE_PATROL_WATCHING ||
                        m_state == STATE_CONFUSE) {
                    m_state = STATE_CONFUSE;
                    m_alertTime = 20;
                    m_lastAlertPos = person.getPosition();
                    setGoal(m_lastAlertPos);
                }

                break;
            }
            }
        }
    }

    for (std::list<Police>::iterator it = scene->policeList.begin(); it != scene->policeList.end(); it++) {
        Police &police = *it;
        if (canSee(police.getPosition())) {
            switch(police.m_state)
            {
            case Police::STATE_ALERT:
                if(m_state == STATE_PATROL_MOVING ||
                        m_state == STATE_PATROL_WATCHING ||
                        m_state == STATE_CONFUSE) {
                    m_state = STATE_ALERT;
                    m_alertTime = police.m_alertTime;
                    m_lastAlertPos = police.m_lastAlertPos;
                    m_chasingPlayerNum = police.m_chasingPlayerNum;
                    setGoal(m_lastAlertPos);
                }
                break;
            case Police::STATE_CONFUSE:
                if(m_state == STATE_PATROL_MOVING ||
                        m_state == STATE_PATROL_WATCHING) {
                    m_state = STATE_CONFUSE;
                    m_alertTime = police.m_alertTime;
                    m_lastAlertPos = police.m_lastAlertPos;
                    m_chasingPlayerNum = police.m_chasingPlayerNum;
                    setGoal(m_lastAlertPos);
                }
                break;
            case Police::STATE_CHASING_PLAYER:
            case Police::STATE_PLAYER_LOST:
                if(m_state == STATE_PATROL_MOVING ||
                        m_state == STATE_PATROL_WATCHING ||
                        m_state == STATE_CONFUSE) {
                    m_state = STATE_ALERT;
                    m_alertTime = 30;
                    m_chasingPlayerNum = police.m_chasingPlayerNum;
                    m_lastAlertPos = m_lastPosSawPlayer[m_chasingPlayerNum];
                    setGoal(m_lastAlertPos);
                }
                break;
            }
        }
    }

    if(m_state != STATE_CHASING_PLAYER)
        for(int i = 0; i < (int)scene->players.size(); i++)
        {
            if(scene->players[i].m_jailed) continue;
            Player& p = scene->players[i];
            if(canSee(p.getPosition())) {
                m_knowPlayer[i] = m_knowPlayer[i] || p.isDoingAction();
                if (m_knowPlayer[i]) {
                    m_lastPosSawPlayer[i] = p.getPosition();
                    m_lastDirSawPlayer[i] = m_lastPosSawPlayer[i] - m_position;
                    m_lastPosSawTime[i] = 5;
                    m_chasingPlayerNum = i;
                    m_state = STATE_CHASING_PLAYER;
                }
            }
        }

    switch(m_state)
    {
    case STATE_PATROL_MOVING:
    {
        m_mark = MARK_NONE;
        m_vel = 20.0f;
        if (!m_hasGoal) {
            m_state = STATE_PATROL_WATCHING;
            m_watchingTime = Utils::randomInt(2, 6);
            lookAtRandomPlace();
            m_watchingTimeFacing = Utils::randomInt(1, 2);
            setGoal(getNewGoal(m_position));
        }

        moveTowardsGoal();

        break;
    }
    case STATE_PATROL_WATCHING:
        m_mark = MARK_NONE;
        m_vel = 20.0f;
        m_watchingTime -= delta;
        m_watchingTimeFacing -= delta;

        if (m_watchingTime < 0) {
            setGoal(getNewGoal(m_position));
            m_state = STATE_PATROL_MOVING;
        }

        if (m_watchingTimeFacing < 0) {
            lookAtRandomPlace();
            m_watchingTimeFacing = Utils::randomInt(1, 2);
        }


        break;
    case STATE_ALERT:
    {
        m_vel = 60.0f;
        m_mark = MARK_QUESTION;
        m_alertTime -= delta;

        if (!m_hasGoal) {
            setGoal(getNewGoal(m_lastAlertPos));
        }

        moveTowardsGoal();
        if (m_alertTime < 0) {
            m_state = STATE_PATROL_MOVING;
//            m_knowPlayer = true;
        }

        break;
    }
    case STATE_CONFUSE:
    {
        m_vel = 30.0f;
        m_mark = MARK_QUESTION;
        m_alertTime -= delta;

        if (!m_hasGoal) {
            setGoal(getNewGoal(m_lastAlertPos));
        }

        moveTowardsGoal();
        if (m_alertTime < 0) {
            m_state = STATE_PATROL_MOVING;
        }

        break;
    }
    case STATE_CHASING_PLAYER:
    {
        m_vel = 75.0f;
        m_mark = MARK_RED_EXCLAMATION;

        m_lastPosSawTime[m_chasingPlayerNum] -= delta;

        Player& p = scene->players[m_chasingPlayerNum];
        if(canSee(p.getPosition()))
        {
            m_lastDirSawPlayer[m_chasingPlayerNum] = p.getPosition()-m_lastPosSawPlayer[m_chasingPlayerNum];
            m_lastPosSawPlayer[m_chasingPlayerNum] = p.getPosition();
            m_lastPosSawTime[m_chasingPlayerNum] = 5;
        }

        moveInDir(m_lastPosSawPlayer[m_chasingPlayerNum]-m_position);

        if (Utils::distance(m_position, m_lastPosSawPlayer[m_chasingPlayerNum]) <= 12)
        {
            m_state = STATE_PLAYER_LOST;
        }

        if (m_lastPosSawTime[m_chasingPlayerNum] < 0)
        {
            m_state = STATE_ALERT;
            m_alertTime = 20;
            m_lastAlertPos = m_lastPosSawPlayer[m_chasingPlayerNum];
        }

        if (Utils::distance(m_position, p.getPosition()) <= 12)
        {
            m_state = STATE_PATROL_MOVING;
            p.gotCaught();
        }

        break;
    }
    case STATE_PLAYER_LOST:
    {
        m_vel = 60.0f;
        m_mark = MARK_QUESTION;
        m_lastPosSawTime[m_chasingPlayerNum] -= delta;

        Player& p = scene->players[m_chasingPlayerNum];
        if(canSee(p.getPosition()))
        {
            m_lastDirSawPlayer[m_chasingPlayerNum] = p.getPosition()-m_lastPosSawPlayer[m_chasingPlayerNum];
            m_lastPosSawPlayer[m_chasingPlayerNum] = p.getPosition();
            m_lastPosSawTime[m_chasingPlayerNum] = 5;
            m_state = STATE_CHASING_PLAYER;
        }
        else
        {
            if (m_collided) {
                m_lastDirSawPlayer[m_chasingPlayerNum].x *= -1;
                m_lastDirSawPlayer[m_chasingPlayerNum].y *= -1;
            }
            moveInDir(m_lastDirSawPlayer[m_chasingPlayerNum]);

            if (m_lastPosSawTime[m_chasingPlayerNum] < 0 || m_collided)
            {
                m_state = STATE_ALERT;
                m_alertTime = 20;
                m_lastAlertPos = m_lastPosSawPlayer[m_chasingPlayerNum];
            }
        }

        break;
    }
    }

    if (m_state == STATE_PATROL_WATCHING) {
        if (m_faceDir == FACE_UP)  ensureAnim("IdleUp");
        if (m_faceDir == FACE_DOWN)  ensureAnim("IdleDown");
        if (m_faceDir == FACE_LEFT)  ensureAnim("IdleLeft");
        if (m_faceDir == FACE_RIGHT)  ensureAnim("IdleRight");
    }

    else {
        if (m_faceDir == FACE_UP)  ensureAnim("WalkingUp");
        if (m_faceDir == FACE_DOWN)  ensureAnim("WalkingDown");
        if (m_faceDir == FACE_LEFT)  ensureAnim("WalkingLeft");
        if (m_faceDir == FACE_RIGHT)  ensureAnim("WalkingRight");
    }


}

sf::Vector2f Police::getNewGoal(sf::Vector2f pos)
{
    int distGoal = 5;
    std::vector<sf::Vector2i> goals;

    sf::Vector2i from = scene->city.absoluteToTilePos(pos);

    vector<vector<int> > vis(TILESIZE, vector<int>(TILESIZE, -1));

    queue<sf::Vector2i> q;
    q.push(from);
    vis[from.x][from.y] = 0;
    while(!q.empty())
    {
        sf::Vector2i v = q.front();
        int dist = vis[v.x][v.y];
        q.pop();

        if (dist > distGoal) continue;
        else goals.push_back(v);

        for(int i = 0; i < 4; i++)
        {
            sf::Vector2i v2 = v + dirInc[i];
            if(v2.x < 0 || v2.x >= TILESIZE) continue;
            if(v2.y < 0 || v2.y >= TILESIZE) continue;
            if(scene->city.occupedIJ(v2.x, v2.y)) continue;
            if(vis[v2.x][v2.y] != -1) continue;
            vis[v2.x][v2.y] = dist+1;
            q.push(v2);
        }
    }

    sf::Vector2i goal = goals[Utils::randomInt(0, goals.size()-2)];
    return sf::Vector2f(goal.x*64+Utils::randomInt(8, 56), goal.y*64+Utils::randomInt(8, 56));

}

void Police::lookAtRandomPlace()
{
    sf::Vector2i v = scene->city.absoluteToTilePos(m_position);

    int i = 0;
    while(i < 4) {
        m_faceDir = Utils::randomInt(FACE_UP, FACE_RIGHT);
        sf::Vector2i v2 = v + dirInc[m_faceDir];
        if(!scene->city.occupedIJ(v2.x, v2.y)) break;
        i++;
    }
}

void Police::Draw() {

    sf::Sprite* spr = m_anim->getCurrentFrame();
    spr->setPosition(m_position);
    App->draw(*spr);

}

bool Police::onLeftCollision(int x, int j)
{
    m_collided = true;
    return m_state != STATE_CHASING_PLAYER;
}

bool Police::onRightCollision(int x, int j)
{
    m_collided = true;
    return m_state != STATE_CHASING_PLAYER;
}

bool Police::onUpCollision(int x, int j)
{
    m_collided = true;
    return m_state != STATE_CHASING_PLAYER;
}

bool Police::onDownCollision(int x, int j)
{
    m_collided = true;
    return m_state != STATE_CHASING_PLAYER;
}
