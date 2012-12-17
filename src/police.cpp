#include "police.h"
#include "graphics_engine.h"

#include "game_reg.h"
#include "input_engine.h"
#include "defines.h"

void Police::Init() {

	GraphEng* graphics = GraphEng::getInstance();

	mySpr.setTexture(*graphics->getTexture("img/police.png"));
	mySpr.setOrigin(mySpr.getTextureRect().width*0.5f,
			mySpr.getTextureRect().height*0.5f);

	m_vel = 20.0f;
	m_watchingTime = 0;
	m_knowPlayer = false;
	m_state = STATE_PATROL_WATCHING;

	AnimationData* ad = new AnimationData();
	ad->Load("anim/poli.anim");
	if (m_anim == NULL) m_anim = new Animation();
	m_anim->setAnimData(ad);
	m_anim->SelectAnim("IdleDown");
}

void Police::Update() {

	Npc::Update();
	m_collided = false;
	InputEng* input = InputEng::getInstance();
	float delta = input->getFrameTime().asSeconds();
	City &city = *GameReg::getInstance()->city;
	Player* p = GameReg::getInstance()->player;

	std::list<Person>* personList = GameReg::getInstance()->personList;
	for (std::list<Person>::iterator it = personList->begin(); it != personList->end(); it++) {
		Person &person = *it;
		if (canSee(person.getPosition())) {
			switch(person.getState())
			{
			case Person::STATE_PANIC:
			{
				if(m_state == STATE_PATROL_MOVING ||
				   m_state == STATE_PATROL_WATCHING) {
					if (person.knowsPlayer())
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
				m_knowPlayer = m_knowPlayer || (canSee(p->m_position) &&
				    Utils::distance(person.m_position, p->m_position) < 70);
				if (m_knowPlayer)
				{
					if(m_state == STATE_PATROL_MOVING ||
					   m_state == STATE_PATROL_WATCHING ||
					   m_state == STATE_CONFUSE) {
						m_state = STATE_ALERT;
						m_alertTime = 20;
						m_lastAlertPos = person.getPosition();
						setGoal(m_lastAlertPos);
					}
				}
				else
				{
					if(m_state == STATE_PATROL_MOVING ||
					   m_state == STATE_PATROL_WATCHING ||
					   m_state == STATE_CONFUSE) {
						m_state = STATE_CONFUSE;
						m_alertTime = 20;
						m_lastAlertPos = person.getPosition();
						setGoal(m_lastAlertPos);
					}
				}

				break;
			}
			}
		}
	}

	std::list<Police>* policeList = GameReg::getInstance()->policeList;
	for (std::list<Police>::iterator it = policeList->begin(); it != policeList->end(); it++) {
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
					setGoal(m_lastAlertPos);
				}
				break;
			case Police::STATE_CONFUSE:
				if(m_state == STATE_PATROL_MOVING ||
				   m_state == STATE_PATROL_WATCHING) {
					m_state = STATE_CONFUSE;
					m_alertTime = police.m_alertTime;
					m_lastAlertPos = police.m_lastAlertPos;
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
					m_lastAlertPos = m_lastPosSawPlayer;
					setGoal(m_lastPosSawPlayer);
				}
				break;
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

		if(canSee(p->getPosition())) {
			if (p->isDoingAction()) m_knowPlayer = true;
			if (m_knowPlayer) {
				m_lastPosSawPlayer = p->getPosition();
				m_lastDirSawPlayer = m_lastPosSawPlayer - m_position;
				m_lastPosSawTime = 5;
				m_state = STATE_CHASING_PLAYER;
			}
		}

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

		if (m_knowPlayer) {
			Player* p = GameReg::getInstance()->player;
			if(canSee(p->getPosition())) {
				m_lastPosSawPlayer = p->getPosition();
				m_lastDirSawPlayer = m_lastPosSawPlayer - m_position;
				m_lastPosSawTime = 5;
				m_state = STATE_CHASING_PLAYER;
			}
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
			m_knowPlayer = true;
		}

		Player* p = GameReg::getInstance()->player;
		if(canSee(p->getPosition())) {
			m_lastPosSawPlayer = p->getPosition();
			m_lastDirSawPlayer = m_lastPosSawPlayer - m_position;
			m_lastPosSawTime = 5;
			m_state = STATE_CHASING_PLAYER;
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
		m_lastPosSawTime -= delta;
		Player* p = GameReg::getInstance()->player;
		if(city.visible(m_position, p->getPosition())) {
			m_lastDirSawPlayer = p->getPosition()-m_lastPosSawPlayer;
			m_lastPosSawPlayer = p->getPosition();
			m_lastPosSawTime = 5;
		}

		moveInDir(m_lastPosSawPlayer-m_position);
		if (Utils::distance(m_position, p->m_position) <= 12)
		{
			GameReg::getInstance()->eventQueue.push(new EventGameOver());
		}

		if (Utils::distance(m_position, m_lastPosSawPlayer) <= 12)
		{
			m_state = STATE_PLAYER_LOST;
		}

		if (m_lastPosSawTime < 0)
		{
			m_state = STATE_ALERT;
			m_alertTime = 20;
			m_lastAlertPos = m_lastPosSawPlayer;
		}

		break;
	}
	case STATE_PLAYER_LOST:
	{
		m_vel = 60.0f;
		m_mark = MARK_QUESTION;
		m_lastPosSawTime -= delta;
		Player* p = GameReg::getInstance()->player;
		if(canSee(p->getPosition())) {
			m_lastDirSawPlayer = m_lastPosSawPlayer-p->getPosition();
			m_lastPosSawPlayer = p->getPosition();
			m_lastPosSawTime = 5;
			m_state = STATE_CHASING_PLAYER;
		}
		else
		{
			moveInDir(m_lastDirSawPlayer);
		}

		if (m_lastPosSawTime < 0 || m_collided)
		{
			m_state = STATE_ALERT;
			m_alertTime = 20;
			m_lastAlertPos = m_lastPosSawPlayer;
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
	std::vector<vec2i> goals;

	City &city = *GameReg::getInstance()->city;
	vec2i from = city.absoluteToTilePos(pos);

	vector<vector<int> > vis(city.getTW(), vector<int>(city.getTH(), -1));

	queue<vec2i> q;
	q.push(from);
	vis[from.x][from.y] = 0;
	while(!q.empty())
	{
		vec2i v = q.front();
		int dist = vis[v.x][v.y];
		q.pop();

		if (dist > distGoal) continue;
		else goals.push_back(v);

		for(int i = 0; i < 4; i++)
		{
			vec2i v2 = v + dirInc[i];
			if(v2.x < 0 || v2.x >= city.getTW()) continue;
			if(v2.y < 0 || v2.y >= city.getTH()) continue;
			if(city.occupedIJ(v2.x, v2.y)) continue;
			if(vis[v2.x][v2.y] != -1) continue;
			vis[v2.x][v2.y] = dist+1;
			q.push(v2);
		}
	}

	vec2i goal = goals[Utils::randomInt(0, goals.size()-2)];
	return vec2(goal.x*64+Utils::randomInt(8, 56), goal.y*64+Utils::randomInt(8, 56));

}

void Police::lookAtRandomPlace()
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
