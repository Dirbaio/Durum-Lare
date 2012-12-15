#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"

class Player : public Object {
public:
	Player() {}

	void Init();
	void Update();
	void Draw();

        void ensureAnim(std::string anim);

        void LoadAnims();

private:

	void noLeftCollision() {}
	void noRightCollision(){}
	void noUpCollision()   {}
	void noDownCollision() {}

	bool onLeftCollision(int x, int j) {return true;}
	bool onRightCollision(int x, int j){return true;}
	bool onUpCollision(int x, int j)   {return true;}
	bool onDownCollision(int x, int j) {return true;}



	sf::Sprite mySpr;
	sf::Vector2f myVel;

        std::string currentAnim;
};

#endif // PLAYER_H
