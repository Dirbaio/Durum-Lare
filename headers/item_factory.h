#ifndef ITEM_FACTORY_H
#define ITEM_FACTORY_H

#include "item.h"

namespace ItemFactory {

    Item* MakeNewItem(GameScene* sc, int type);


};

#endif // ITEM_FACTORY_H
