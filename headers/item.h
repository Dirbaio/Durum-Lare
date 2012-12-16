#ifndef ITEM_H
#define ITEM_H

#include "object.h"

enum eItemType {
    ITEM_MONEY,

    ITEM_TYPE_SIZEs
};

class Item : public Object {
public:

    Item() {}

    virtual void Init();
    virtual void Update();
    virtual void Draw();

    void setSprite(sf::Sprite spr);
    void setScore(int n) {myScore = n;}

protected:

    int myScore;

};



#endif // ITEM_H
