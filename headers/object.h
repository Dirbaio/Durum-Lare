#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics.hpp>
#include <queue>
#include "event.h"
#include "animation.h"

class GameScene;
//#include "animation.h"
//#include "Transition.h"

enum eObjDir {
    FACE_UP = 0, FACE_DOWN, FACE_LEFT, FACE_RIGHT, FACE_SIZE
};

extern const sf::Vector2i dirInc[];

enum eObjType {

    OBJ_PROP,
    //Total size
    OBJ_TOTAL_TYPE_SIZE
};

class Object
{
public:
    GameScene* scene;
    Object(GameScene* sc);

    int getUniqueID() {return uniqueID;}

    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void DrawMark() {}

    void updateBBox();

    sf::FloatRect getBoundBox() { return m_boundbox; }
    virtual void setPosition(const sf::Vector2f& pos) {
        m_position = pos;
    }
    sf::Vector2f getPosition() { return m_position; }

    sf::Vector2f getScale() { return m_scale; }
    void setScale(sf::Vector2f sca) { m_scale = sca; }

    sf::Vector2f getOrigin() { return m_origin; }

    sf::Vector2f getCenter() {return m_center;}

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
    void setTakeable (bool _isTakeable) { m_isTakeable = _isTakeable; }
    bool isUsable() { return m_isUsable; }
    bool isToBeDeleted() { return m_toBeDeleted; }
    void markForDelete() { m_toBeDeleted = true; }

    //Transitions
    /*
    void setDrawTransition(Transition* tx, Transition* ty);
    void checkTransition();
    void cancelTransition();
*/

    //Events
    virtual void onUse() {}
    virtual void onBuy() {}
    virtual void onGameTick() {}
    virtual void onCollide() {}

    int m_prio;

    void ensureAnim(std::string anim);

    sf::Vector2f m_position;

protected:
    static int idCount;
    int uniqueID;

    bool m_toBeDeleted;

    int m_type;

    Animation* m_anim;
    sf::Sprite mySpr;


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

    std::string currentAnim;

    //Transition* myTransitionX;
    //Transition* myTransitionY;
};

#endif // OBJECT_H
