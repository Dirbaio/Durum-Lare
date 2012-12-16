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

protected:



};

#endif // ITEM_H
