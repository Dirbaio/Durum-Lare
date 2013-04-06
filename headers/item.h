#ifndef ITEM_H
#define ITEM_H

#include "object.h"
#include "transition.h"

enum eItemType {
    ITEM_NONE = 0,
    ITEM_MONEY,
    ITEM_BIG_MONEY,

    ITEM_SHOE,
    ITEM_GLASSES,

    ITEM_BURGER,

    ITEM_TYPE_SIZE
};

class Item : public Object {
public:

    Item(GameScene* sc, int type) : Object(sc),  myItemType(type) {}
    virtual ~Item() {}

    virtual void Init();
    virtual void Update();
    virtual void Draw();

    void setSprite(sf::Sprite spr);
    void setValue(int n) {myValue = n;}
    int getValue() {return myValue;}

    void setTransPos(sf::Vector2f from, sf::Vector2f to);
    void setTransScale(float from, float to, float time);

    void takeAction();

    bool isSpawning() {return flagSpawning;}

    int getItemType() {return myItemType;}

protected:

    int myValue;

    int myItemType;

    TransitionLinear* transPosX;
    TransitionLinear* transPosY;
    TransitionLinear* transScale;

    float dieTimer;
    bool flagDie;

    float spawnTimer;
    bool flagSpawning;

};



#endif // ITEM_H
