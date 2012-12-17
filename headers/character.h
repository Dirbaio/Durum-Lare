#ifndef CHARACTER_H
#define CHARACTER_H

#include "object.h"
#include <queue>
#include "utils.h"

using namespace std;

enum CharacterMark {
    MARK_NONE,
    MARK_EXCLAMATION,
    MARK_RED_EXCLAMATION,
    MARK_BLUE_QUESTION,
    MARK_QUESTION
};

class Character : public Object
{
public:
	Character();
	virtual ~Character() {}

	void move(sf::Vector2f posf);
    bool canSee(const vec2& pos);
protected:


	virtual void noLeftCollision() {}
	virtual void noRightCollision(){}
	virtual void noUpCollision()   {}
	virtual void noDownCollision() {}

	virtual bool onLeftCollision(int x, int j) {return true;}
	virtual bool onRightCollision(int x, int j){return true;}
	virtual bool onUpCollision(int x, int j)   {return true;}
	virtual bool onDownCollision(int x, int j) {return true;}

	queue<vec2> m_path;
	vec2 m_goal;
	bool m_hasGoal;

	float m_vel;
    CharacterMark m_mark;
    sf::Sprite spriteExc, spriteQuest, spriteRedExc, spriteBlueQuest;

    void DrawMark();

	void setGoal(vec2 goal);
	void moveTowardsGoal();
    void moveInDir(vec2 dir);
};

#endif // CHARACTER_H
