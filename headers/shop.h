#ifndef SHOP_H
#define SHOP_H

#include "object.h"
#include <SFML/Graphics.hpp>
#include "item.h"

struct ShopStats {
    std::string message;
    std::vector<Item> items;
};

class Shop : public Object {
public:
    Shop() {}
    ~Shop() {}

    void Init();
    void Update();
    void Draw();

    bool isPlayerNear() {return m_isPlayerNear;}
    ShopStats& getStats() {return m_shopStats;}

private:
    sf::Sprite m_shopSprite;
    bool m_isPlayerNear;
    ShopStats m_shopStats;

    void setItem(int n, int type);
};

#endif // SHOP_H
