#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"

class Player : public Character {
public:
	Player() {}

	void Init();
	void Update();
	void Draw();

        void ensureAnim(std::string anim);

        void LoadAnims();

private:


	sf::Sprite mySpr;
	sf::Vector2f myVel;

        std::string currentAnim;
};

#endif // PLAYER_H
