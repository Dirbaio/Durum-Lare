#ifndef ITEM_H
#define ITEM_H

#include "object.h"
#include "transition.h"

enum eItemType {
    ITEM_MONEY,

    ITEM_TYPE_SIZEs
};

class Item : public Object {
public:

    Item() {}
    virtual ~Item() {}

    virtual void Init();
    virtual void Update();
    virtual void Draw();

    void setSprite(sf::Sprite spr);
    void setScore(int n) {myScore = n;}
    int getScore() {return myScore;}

    void setTransPos(sf::Vector2f from, sf::Vector2f to);
    void setTransScale(float from, float to, float time);

    void takeAction();

    bool isSpawning() {return flagSpawning;}

protected:

    int myScore;

    TransitionLinear* transPosX;
    TransitionLinear* transPosY;
    TransitionLinear* transScale;

    float dieTimer;
    bool flagDie;

    float spawnTimer;
    bool flagSpawning;

};



#endif // ITEM_H
