#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics.hpp>

class Hud {
public:

    Hud() {}

    void Init();
    void Draw();


private:

    sf::Sprite moneyHud;

};

#endif // HUD_H
