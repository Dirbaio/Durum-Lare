#ifndef CHARACTER_H
#define CHARACTER_H

#include "object.h"

class Character : public Object
{
public:
	Character();
	virtual ~Character() {}

protected:

	void move(sf::Vector2f posf);

	virtual void noLeftCollision() {}
	virtual void noRightCollision(){}
	virtual void noUpCollision()   {}
	virtual void noDownCollision() {}

	virtual bool onLeftCollision(int x, int j) {return true;}
	virtual bool onRightCollision(int x, int j){return true;}
	virtual bool onUpCollision(int x, int j)   {return true;}
	virtual bool onDownCollision(int x, int j) {return true;}


};

#endif // CHARACTER_H
