#include "player.h"
#include <SFML/Graphics.hpp>
#include "graphics_engine.h"
#include "defines.h"
#include "input_engine.h"
#include "game_reg.h"

#include <GL/gl.h>


void Player::Init() {

	GraphEng* graphics = GraphEng::getInstance();

	mySpr.setTexture(*graphics->getTexture("img/player.png"));

	m_position.x = 0;
	m_position.y = 0;

	myVel.x = 64*3;
	myVel.y = 64*3;

	m_boundbox.height = 16;
	m_boundbox.width = 16;

}

void Player::Update() {
	updateBBox();

	GameReg* gameReg = GameReg::getInstance();
	InputEng* input = InputEng::getInstance();

	const sf::Vector2f &pos0 = m_position;
	sf::Vector2f posf = m_position;

	if (input->getKeyState(InputEng::PLAYER_UP))
		posf.y -= myVel.y*input->getFrameTime().asSeconds();
	if (input->getKeyState(InputEng::PLAYER_DOWN))
		posf.y += myVel.y*input->getFrameTime().asSeconds();
	if (input->getKeyState(InputEng::PLAYER_LEFT))
		posf.x -= myVel.x*input->getFrameTime().asSeconds();
	if (input->getKeyState(InputEng::PLAYER_RIGHT))
		posf.x += myVel.x*input->getFrameTime().asSeconds();

	sf::Vector2f sizs = sf::Vector2f(8, 8);
	sf::Vector2f cens = sf::Vector2f(4, 4);

	sf::Vector2f scen = sizs-cens;
	sf::Vector2f direction = posf - m_position;

	City &city = *gameReg->city;

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

	if (input->getKeyDown(InputEng::PLAYER_ACTION)) {
		gameReg->eventQueue.push(new EventPlayerAction());
	}
}

void Player::Draw() {

	mySpr.setPosition(m_position);
	App->draw(mySpr);

}
