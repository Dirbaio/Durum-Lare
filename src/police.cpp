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
	m_state = STATE_PATROL_WATCHING;

	AnimationData* ad = new AnimationData();
	ad->Load("anim/poli.anim");
	if (m_anim == NULL) m_anim = new Animation();
	m_anim->setAnimData(ad);
	m_anim->SelectAnim("IdleDown");
}

void Police::Update() {

	Npc::Update();
	InputEng* input = InputEng::getInstance();
	City &city = *GameReg::getInstance()->city;

	std::list<Person>* personList = GameReg::getInstance()->personList;
	for (std::list<Person>::iterator it = personList->begin(); it != personList->end(); it++) {
		Person &person = *it;
		vec2 pos = person.getPosition();
		vec2 dir_corpse = pos - m_position;
		vec2 dir_facing (dirInc[m_faceDir].x,dirInc[m_faceDir].y);
		Utils::normalize(dir_corpse);
		Utils::normalize(dir_facing);
		if (Utils::dot2(dir_corpse,dir_facing) >= 0.5f && city.visible(pos,m_position)) {
			switch(person.getState())
			{
			case Person::STATE_PANIC:
			{
				if(m_state == STATE_PATROL_MOVING ||
				   m_state == STATE_PATROL_WATCHING) {
					m_state = STATE_CONFUSE;
					setGoal(person.getPosition());
				}
				break;
			}
			case Person::STATE_DEAD:
				if(m_state == STATE_PATROL_MOVING ||
				   m_state == STATE_PATROL_WATCHING ||
				   m_state == STATE_CONFUSE) {
					m_state = STATE_ALERT;
					setGoal(person.getPosition());
				}
				break;
			}
		}
	}

	switch(m_state)
	{
	case STATE_PATROL_MOVING:
		m_mark = MARK_NONE;
		m_vel = 20.0f;
		if (!m_hasGoal) {
			m_state = STATE_PATROL_WATCHING;
			m_watchingTime = Utils::randomInt(2, 6);
			lookAtRandomPlace();
			m_watchingTimeFacing = Utils::randomInt(1, 2);
			setGoal(getNewGoal());
		}
		else moveTowardsGoal();
		break;
	case STATE_PATROL_WATCHING:
		m_mark = MARK_NONE;
		m_vel = 20.0f;
		m_watchingTime -= input->getFrameTime().asSeconds();
		m_watchingTimeFacing -= input->getFrameTime().asSeconds();

		if (m_watchingTime < 0) {
			setGoal(getNewGoal());
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
                m_mark = MARK_RED_EXCLAMATION;
		if (!m_hasGoal) {
			setGoal(getNewGoal());
		}
		else moveTowardsGoal();

		Player* p = GameReg::getInstance()->player;
		vec2 pos = p->getPosition();
		vec2 dir_player = pos - m_position;
		vec2 dir_facing (dirInc[m_faceDir].x,dirInc[m_faceDir].y);
		Utils::normalize(dir_player);
		Utils::normalize(dir_facing);

		if(Utils::dot2(dir_player,dir_facing) >= 0.5f && city.visible(p->getPosition(),m_position)) {
			m_lastPosSawPlayer = p->getPosition();
			m_lastDirSawPlayer = m_lastPosSawPlayer - m_position;
			m_state = STATE_CHASING_PLAYER;
		}
		break;
	}
	case STATE_CONFUSE:
	{
		m_vel = 30.0f;
		m_mark = MARK_QUESTION;
		if (!m_hasGoal) {
			setGoal(getNewGoal());
			m_state = STATE_ALERT;
		}
		else moveTowardsGoal();
		break;
	}
	case STATE_CHASING_PLAYER:
	{
		m_vel = 75.0f;
                m_mark = MARK_RED_EXCLAMATION;
		Player* p = GameReg::getInstance()->player;
		if(city.visible(p->getPosition(), m_position)) {
			m_lastDirSawPlayer = p->getPosition()-m_lastPosSawPlayer;
			m_lastPosSawPlayer = p->getPosition();
		}

		moveInDir(m_lastPosSawPlayer-m_position);
		sf::FloatRect rect = getBoundBox();
		if (rect.contains(m_lastPosSawPlayer))
		{
			if (rect.intersects(p->getBoundBox()))
			{
				GameReg::getInstance()->eventQueue.push(new EventGameOver());
			}
			else
			{
				m_state = STATE_PLAYER_LOST;
			}
		}

		break;
	}
	case STATE_PLAYER_LOST:
		m_vel = 60.0f;
		m_mark = MARK_QUESTION;
		Player* p = GameReg::getInstance()->player;
		if(city.visible(p->getPosition(), m_position)) {
			m_lastDirSawPlayer = m_lastPosSawPlayer-p->getPosition();
			m_lastPosSawPlayer = p->getPosition();
			m_state = STATE_CHASING_PLAYER;
		}
		else
		{
			moveInDir(m_lastDirSawPlayer);
		}
		break;
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

sf::Vector2f Police::getNewGoal()
{
	int distGoal = 5;
	std::vector<vec2i> goals;

	City &city = *GameReg::getInstance()->city;
	vec2i from = city.absoluteToTilePos(m_position);

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

