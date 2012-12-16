#ifndef ITEM_FACTORY_H
#define ITEM_FACTORY_H

#include "item.h"

namespace ItemFactory {

    enum ItemType {

        ITEM_MONEY,

        ITEM_TYPE_SIZE

    };

    Item* MakeNewItem(int type);


};

#endif // ITEM_FACTORY_H
