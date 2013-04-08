#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics.hpp>

class GameScene;

class Hud {
public:

    Hud() {}

    GameScene* scene;
    void Init(GameScene* scene);
    void Draw();


private:

    sf::Sprite moneyHud;
    sf::Sprite killsHud;

    sf::Sprite shopHud;

    sf::Font hudFont;

};

#endif // HUD_H
