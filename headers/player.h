#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"

class Player : public Character {
public:
	Player() {}

	void Init();
	void Update();
	void Draw();

    void LoadAnims();
	void hitAction();

private:
	sf::Sprite mySpr;
	sf::Vector2f myVel;

    std::string currentAnim;
	float m_actionDelay;

    int myScore;
};

#endif // PLAYER_H
