#ifndef ITEM_H
#define ITEM_H

#include "object.h"

class Item : public Object {
public:

    Item() {}

    virtual void Init();
    virtual void Update();
    virtual void Draw();

protected:



};

#endif // ITEM_H
