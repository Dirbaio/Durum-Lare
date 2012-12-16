#include "police.h"
#include "graphics_engine.h"

#include "game_reg.h"
#include "defines.h"

void Police::Init() {

	GraphEng* graphics = GraphEng::getInstance();

	mySpr.setTexture(*graphics->getTexture("img/police.png"));
	mySpr.setOrigin(mySpr.getTextureRect().width*0.5f,
			mySpr.getTextureRect().height*0.5f);

	m_vel = 16.0f*1.25f;
	m_watchingTime = 0;

}

void Police::Update() {

	Npc::Update();

	switch(m_state)
	{
	case STATE_PATROL_MOVING:
		if (!m_hasGoal) {

		}
		break;

	}
}

sf::Vector2f Police::getNewGoal()
{
	int distGoal = Utils::randomInt(1, 5);
	std::vector<vec2i> goals;

	City &city = *GameReg::getInstance()->city;
	vec2i from = city.absoluteToTilePos(m_position);

	vector<vector<int> > vis(city.getTW(), vector<int>(city.getTH(), -1));

	int dx[] = {0, 0, 1, -1};
	int dy[] = {1, -1, 0, 0};

	queue<vec2i> q;
	q.push(from);
	vis[from.x][from.y] = 0;
	while(!q.empty())
	{
		vec2i& v = q.front();
		int x = v.x;
		int y = v.y;
		int dist = vis[x][x];
		q.pop();

		if (distGoal == dist) goals.push_back(v);
		else if (distGoal < dist) continue;

		dist++;
		for(int i = 0; i < 4; i++)
		{
			int x2 = x + dx[i];
			int y2 = y + dy[i];
			if(x2 < 0 || x2 >= city.getTW()) continue;
			if(y2 < 0 || y2 >= city.getTH()) continue;
			if(city.occupedIJ(x2, y2)) continue;
			if(vis[x2][y2] != -1) continue;
			vis[x2][y2] = dist;
			q.push(vec2i(x2, y2));
		}
	}

	vec2i goal = goals[Utils::randomInt(0, goals.size()-1)];
	return vec2(goal.x*64+Utils::randomInt(8, 56), goal.y*64+Utils::randomInt(8, 56));

}
