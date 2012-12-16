#include "character.h"

#include "utils.h"
#include "game_reg.h"
#include "input_engine.h"
#include "graphics_engine.h"
Character::Character()
{
    m_hasGoal = false;
    m_mark = MARK_NONE;
    m_vel = 16.0f;

    GraphEng* graphics = GraphEng::getInstance();
    spriteExc.setTexture(*graphics->getTexture("img/exc_mark.png"));
    spriteExc.setOrigin(8, 8);
    spriteQuest.setTexture(*graphics->getTexture("img/int_mark.png"));
    spriteQuest.setOrigin(8, 8);
}

void Character::move(sf::Vector2f posf)
{
	const sf::Vector2f &pos0 = m_position;
	City &city = *GameReg::getInstance()->city;
	sf::Vector2f sizs = sf::Vector2f(10, 5);
	sf::Vector2f cens = sf::Vector2f(5, 4);

	sf::Vector2f scen = sizs-cens;
	sf::Vector2f direction = posf - pos0;

	if (direction.y < 0) //Vamos hacia abajo
	{
		//le restamos a la Y la mitad de su tamaño para obtener la Y inferior del sprite
		int yo = city.absoluteToTilePosY(pos0.y - scen.y),
		    yn = city.absoluteToTilePosY(posf.y - scen.y),
		    xl = city.absoluteToTilePosX(pos0.x - cens.x + 2),
		    xr = city.absoluteToTilePosX(pos0.x + scen.x - 2);
		for (int y = yo; y >= yn; y--)
		{
			for (int x = xl; x <= xr; x++)
			{
				if (city.occupedIJ(x,y) && onDownCollision(x, y))
				{
					posf.y = city.tileTopPos(y) + scen.y;
					goto vert_exit;
				}
			}
		}

		noDownCollision();
	}
	else if (direction.y > 0) //Vamos hacia arriba
	{
		//le sumamos a la Y la mitad de su tamaño para obtener la Y superior del sprite
		int yo = city.absoluteToTilePosY(pos0.y + cens.y),
		    yn = city.absoluteToTilePosY(posf.y + cens.y),
		    xl = city.absoluteToTilePosX(pos0.x - cens.x + 2),
		    xr = city.absoluteToTilePosX(pos0.x + scen.x - 2);
		for (int y = yo; y <= yn; y++)
		{
			for (int x = xl; x <= xr; x++)
			{
				if (city.occupedIJ(x,y) && onUpCollision(x, y))
				{
					posf.y = city.tileBottomPos(y) - cens.y;
					goto vert_exit;
				}
			}
		}

		noUpCollision();
	}
vert_exit:

	if (direction.x < 0) //Vamos hacia la izquierda
	{
		int xo = city.absoluteToTilePosX(pos0.x - cens.x),
		    xn = city.absoluteToTilePosX(posf.x - cens.x),
		    yb = city.absoluteToTilePosY(pos0.y - scen.y + 2),
		    yt = city.absoluteToTilePosY(pos0.y + cens.y - 2);
		for (int x = xo; x >= xn; x--)
		{
			for (int y = yb; y <= yt; y++)
			{
				if (city.occupedIJ(x,y) && onLeftCollision(x, y))
				{
					posf.x = city.tileRightPos(x) +cens.x;
					goto horz_exit;
				}
			}
		}

		noLeftCollision();
	}
	else if (direction.x > 0) //Vamos hacia la derecha
	{
		int xo = city.absoluteToTilePosX(pos0.x + scen.x),
				xn = city.absoluteToTilePosX(posf.x + scen.x),
				yb = city.absoluteToTilePosY(pos0.y - scen.y + 2),
				yt = city.absoluteToTilePosY(pos0.y + cens.y - 2);
		for (int x = xo; x <= xn; x++)
		{
			for (int y = yb; y <= yt; y++)
			{
				if (city.occupedIJ(x,y) && onRightCollision(x, y))
				{
					posf.x = city.tileLeftPos(x) - scen.x;
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
        spriteExc.setPosition(m_position+vec2(0, -22+t));
        App->draw(spriteExc);
    }
    else if(m_mark == MARK_QUESTION)
    {
        spriteQuest.setScale(1+t/10, 1-t/20);
        spriteQuest.setPosition(m_position+vec2(0, -22+t));
        App->draw(spriteQuest);
    }
}

void Character::setGoal(vec2 goal) {
    m_goal = goal;

    City &city = *GameReg::getInstance()->city;

    vec2i from = city.absoluteToTilePos(m_position);
    vec2i to = city.absoluteToTilePos(m_goal);

    //Calculate the path!
    vector<vector<int> > vis(city.getTW(), vector<int>(city.getTH(), -1));

    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    queue<vec2i> q;
    q.push(to);
    while(!q.empty())
    {
        int x = q.front().x;
        int y = q.front().y;
        q.pop();

        for(int i = 0; i < 4; i++)
        {
            int x2 = x + dx[i];
            int y2 = y + dy[i];
            if(x2 < 0 || x2 >= city.getTW()) continue;
            if(y2 < 0 || y2 >= city.getTH()) continue;
            if(city.occupedIJ(x2, y2)) continue;
            if(vis[x2][y2] != -1) continue;
            vis[x2][y2] = i;
            q.push(vec2i(x2, y2));
        }
    }

    if(vis[from.x][from.y] == -1)
        cerr<<"ERROR NO PATH FOUND OMG OMG OMG"<<endl;
    else
    {
        m_hasGoal = true;
        vector<vec2i> v;

        while(from != to)
        {
            v.push_back(from);
            vec2i from2 = from;
            from.x -= dx[vis[from2.x][from2.y]];
            from.y -= dy[vis[from2.x][from2.y]];
        }
        v.push_back(from);

        m_path = queue<vec2>();

        vec2i ant(-1, -1);
        vec2 antf(16, 16);
        for(int i = 0; i < v.size(); i++)
        {
            vec2 p (v[i].x*64+Utils::randomInt(8, 56), v[i].y*64+Utils::randomInt(8, 56));
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
}

void Character::moveTowardsGoal()
{
    if(!m_hasGoal) return;

    InputEng* input = InputEng::getInstance();

    while(!m_path.empty() && Utils::distance(m_path.front(), m_position) < 2)
        m_path.pop();

    if(m_path.empty())
    {
        m_hasGoal = false;
        return;
    }

    vec2 to = m_path.front();

    vec2 dir = to-m_position;
    Utils::normalize(dir);

    if(dir.x < -0.5f) m_faceDir = FACE_LEFT;
    if(dir.y < -0.5f) m_faceDir = FACE_UP;
    if(dir.x >  0.5f) m_faceDir = FACE_RIGHT;
    if(dir.y >  0.5f) m_faceDir = FACE_DOWN;

    float delta = input->getFrameTime().asSeconds();
    move(m_position + dir*delta*m_vel);
}
