#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics.hpp>
#include <queue>
#include "event.h"
#include "animation.h"
//#include "animation.h"
//#include "Transition.h"

enum eObjDir {
	FACE_UP = 0, FACE_DOWN, FACE_LEFT, FACE_RIGHT, FACE_SIZE
};

enum eObjType {

	OBJ_PROP,
	//Total size
	OBJ_TOTAL_TYPE_SIZE
};

class Object
{
public:
	Object();

	int getUniqueID() {return uniqueID;}

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void updateBBox();

	sf::FloatRect getBoundBox() { return m_boundbox; }
	virtual void setPosition(const sf::Vector2f& pos) {
		m_position = pos;
	}
	sf::Vector2f getPosition() { return m_position; }
	sf::Vector2f getOrigin() { return m_origin; }

	sf::FloatRect getLocalBounds() {
		return mySpr.getLocalBounds();
	}

	//Flags
	int getType() { return m_type; }
	bool canCollide() { return m_canCollide; }
	int faceDir() { return m_faceDir; }
	bool isTaken() {return m_isTaken;}
	void setTaken(bool taken) {m_isTaken = taken;}
	bool isTakeable() { return m_isTakeable; }
	void setTakeable(bool isTakeable) { m_isTakeable = isTakeable; }
	bool isUsable() { return m_isUsable; }

	//Transitions
	/*
	void setDrawTransition(Transition* tx, Transition* ty);
	void checkTransition();
	void cancelTransition();
*/

	//Events
	virtual void onUse() {}
	virtual void onTake() {}
	virtual void onGameTick() {}
	virtual void onCollide() {}

protected:
	int uniqueID;

	int m_type;

        Animation* m_anim;
        sf::Sprite mySpr;

	sf::Vector2f m_position;
	sf::Vector2f m_origin;
	sf::Vector2f m_scale;
	sf::Vector2f m_center;
	float m_rotation;

	sf::FloatRect m_boundbox;

	bool m_canCollide;
	int m_faceDir;

	bool m_isTakeable; //Items only
	bool m_isUsable;   //Items and props
	bool m_isTaken;    //Items only

	//Transition* myTransitionX;
	//Transition* myTransitionY;
};

#endif // OBJECT_H
