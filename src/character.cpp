#include "character.h"

#include <cassert>

#include "utils.h"
#include "input_engine.h"
#include "graphics_engine.h"
#include "game_scene.h"

Character::Character(GameScene* sc) : Object(sc)
{
    m_hasGoal = false;
    m_mark = MARK_NONE;
    m_vel = 16.0f;

    GraphEng* graphics = GraphEng::getInstance();
    spriteExc.setTexture(*graphics->getTexture("img/exc_mark.png"));
    spriteExc.setOrigin(8, 8);
    spriteRedExc.setTexture(*graphics->getTexture("img/exc_rmark.png"));
    spriteRedExc.setOrigin(8, 8);
    spriteQuest.setTexture(*graphics->getTexture("img/int_rmark.png"));
    spriteQuest.setOrigin(8, 8);
    spriteBlueQuest.setTexture(*graphics->getTexture("img/int_mark.png"));
    spriteBlueQuest.setOrigin(8, 8);

}

void Character::move(sf::Vector2f posf)
{
	const sf::Vector2f &pos0 = m_position;

	sf::Vector2f sizs = sf::Vector2f(10, 5);
	sf::Vector2f cens = sf::Vector2f(5, 4);

	sf::Vector2f scen = sizs-cens;
	sf::Vector2f direction = posf - pos0;

	if (direction.y < 0) //Vamos hacia abajo
	{
		//le restamos a la Y la mitad de su tamaño para obtener la Y inferior del sprite
		int yo = scene->city.absoluteToTilePosY(pos0.y - scen.y),
		    yn = scene->city.absoluteToTilePosY(posf.y - scen.y),
		    xl = scene->city.absoluteToTilePosX(pos0.x - cens.x + 2),
		    xr = scene->city.absoluteToTilePosX(pos0.x + scen.x - 2);
		for (int y = yo; y >= yn; y--)
		{
			for (int x = xl; x <= xr; x++)
			{
				if (scene->city.occupedIJ(x,y) && onDownCollision(x, y))
				{
					posf.y = scene->city.tileTopPos(y) + scen.y;
					goto vert_exit;
				}
			}
		}

		noDownCollision();
	}
	else if (direction.y > 0) //Vamos hacia arriba
	{
		//le sumamos a la Y la mitad de su tamaño para obtener la Y superior del sprite
		int yo = scene->city.absoluteToTilePosY(pos0.y + cens.y),
		    yn = scene->city.absoluteToTilePosY(posf.y + cens.y),
		    xl = scene->city.absoluteToTilePosX(pos0.x - cens.x + 2),
		    xr = scene->city.absoluteToTilePosX(pos0.x + scen.x - 2);
		for (int y = yo; y <= yn; y++)
		{
			for (int x = xl; x <= xr; x++)
			{
				if (scene->city.occupedIJ(x,y) && onUpCollision(x, y))
				{
					posf.y = scene->city.tileBottomPos(y) - cens.y;
					goto vert_exit;
				}
			}
		}

		noUpCollision();
	}
vert_exit:

	if (direction.x < 0) //Vamos hacia la izquierda
	{
		int xo = scene->city.absoluteToTilePosX(pos0.x - cens.x),
		    xn = scene->city.absoluteToTilePosX(posf.x - cens.x),
		    yb = scene->city.absoluteToTilePosY(pos0.y - scen.y + 2),
		    yt = scene->city.absoluteToTilePosY(pos0.y + cens.y - 2);
		for (int x = xo; x >= xn; x--)
		{
			for (int y = yb; y <= yt; y++)
			{
				if (scene->city.occupedIJ(x,y) && onLeftCollision(x, y))
				{
					posf.x = scene->city.tileRightPos(x) +cens.x;
					goto horz_exit;
				}
			}
		}

		noLeftCollision();
	}
	else if (direction.x > 0) //Vamos hacia la derecha
	{
		int xo = scene->city.absoluteToTilePosX(pos0.x + scen.x),
				xn = scene->city.absoluteToTilePosX(posf.x + scen.x),
				yb = scene->city.absoluteToTilePosY(pos0.y - scen.y + 2),
				yt = scene->city.absoluteToTilePosY(pos0.y + cens.y - 2);
		for (int x = xo; x <= xn; x++)
		{
			for (int y = yb; y <= yt; y++)
			{
				if (scene->city.occupedIJ(x,y) && onRightCollision(x, y))
				{
					posf.x = scene->city.tileLeftPos(x) - scen.x;
					goto horz_exit;
				}
			}
		}

		noRightCollision();
	}
horz_exit:

	m_position = posf;
}

void Character::DrawMark()
{
    float t = scene_total_time.asSeconds() * 10;
    t = sin(t) * 3;
    if(m_mark == MARK_EXCLAMATION)
    {
        spriteExc.setScale(1+t/10, 1-t/20);
	spriteExc.setPosition(m_position+sf::Vector2f(0, -22+t));
        App->draw(spriteExc);
    }
    else if(m_mark == MARK_QUESTION)
    {
        spriteQuest.setScale(1+t/10, 1-t/20);
	spriteQuest.setPosition(m_position+sf::Vector2f(0, -22+t));
        App->draw(spriteQuest);
    }
    else if(m_mark == MARK_BLUE_QUESTION)
    {
        spriteBlueQuest.setScale(1+t/10, 1-t/20);
	spriteBlueQuest.setPosition(m_position+sf::Vector2f(0, -22+t));
        App->draw(spriteBlueQuest);
    }
    else if(m_mark == MARK_RED_EXCLAMATION)
    {
        spriteRedExc.setScale(1+t/10, 1-t/20);
	spriteRedExc.setPosition(m_position+sf::Vector2f(0, -22+t));
        App->draw(spriteRedExc);
    }
}

void Character::setGoal(sf::Vector2f goal) {
    m_goal = goal;

    sf::Vector2i from = scene->city.absoluteToTilePos(m_position);
    sf::Vector2i to = scene->city.absoluteToTilePos(m_goal);
	
	if(scene->city.occupedIJ(from.x, from.y)) {
                //cout<<"Pathfinding: Current pos is solid. "<<endl;
		return;
	}
	if(scene->city.occupedIJ(to.x, to.y)) {
                //cout<<"Pathfinding: Goal is solid."<<endl;
		return;
	}

    //Calculate the path!
    vector<vector<int> > vis(TILESIZE, vector<int>(TILESIZE, -1));

    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

	bool end = false;
	
    queue<sf::Vector2i> q;
    q.push(to);
    while(!q.empty() && !end)
    {
        int x = q.front().x;
        int y = q.front().y;
        q.pop();

        for(int i = 0; i < 4; i++)
        {
            int x2 = x + dx[i];
            int y2 = y + dy[i];
            if(x2 < 0 || x2 >= TILESIZE) continue;
            if(y2 < 0 || y2 >= TILESIZE) continue;
            if(scene->city.occupedIJ(x2, y2)) continue;
            if(vis[x2][y2] != -1) continue;
            vis[x2][y2] = i;
	    sf::Vector2i v2 (x2, y2);
            if(v2 == from)
            	end = true;
            q.push(v2);
        }
    }

    if(vis[from.x][from.y] == -1) {
        cout<<"Pathfinding: ERROR NO PATH FOUND OMG OMG OMG"<<endl;
        return;
    }
    
	m_hasGoal = true;
	vector<sf::Vector2i> v;

	while(from != to)
	{
		v.push_back(from);
		sf::Vector2i from2 = from;
		from.x -= dx[vis[from2.x][from2.y]];
		from.y -= dy[vis[from2.x][from2.y]];
	}
	v.push_back(from);

	m_path = queue<sf::Vector2f>();

	sf::Vector2i ant(-1, -1);
	sf::Vector2f antf(16, 16);
	for(int i = 0; i < v.size(); i++)
	{
		sf::Vector2f p (v[i].x*64+Utils::randomInt(8, 56), v[i].y*64+Utils::randomInt(8, 56));
		if(v[i].x == ant.x)
			p.x = antf.x;
		if(v[i].y == ant.y)
			p.y = antf.y;
		ant = v[i];
		antf = p;
		m_path.push(p);
	}
	m_path.push(goal);
}

void Character::moveTowardsGoal()
{
    if(!m_hasGoal) return;

    while(!m_path.empty() && Utils::distance(m_path.front(), m_position) < 2)
        m_path.pop();

    if(m_path.empty())
    {
        m_hasGoal = false;
        return;
    }

    sf::Vector2f to = m_path.front();
    moveInDir(to-m_position);
}

void Character::moveInDir(sf::Vector2f dir)
{
    if (Utils::norm(dir) == 0) return;
    Utils::normalize(dir);

    if(dir.x < -0.5f) m_faceDir = FACE_LEFT;
    if(dir.y < -0.5f) m_faceDir = FACE_UP;
    if(dir.x >  0.5f) m_faceDir = FACE_RIGHT;
    if(dir.y >  0.5f) m_faceDir = FACE_DOWN;

    float delta = scene->input.getFrameTime().asSeconds();
    move(m_position + dir*delta*m_vel);
}

bool Character::canSee(const sf::Vector2f& pos)
{

    sf::Vector2f dir_corpse = pos-m_position;
    sf::Vector2f dir_facing((float) dirInc[m_faceDir].x, (float) dirInc[m_faceDir].y);
    Utils::normalize(dir_corpse);
    Utils::normalize(dir_facing);

    return Utils::dot2(dir_corpse,dir_facing) >= 0.0f && scene->city.visible(m_position, pos);
}


bool Character::onLeftCollision(int x, int j) {return true;}
bool Character::onRightCollision(int x, int j){return true;}
bool Character::onUpCollision(int x, int j)   {return true;}
bool Character::onDownCollision(int x, int j) {return true;}
