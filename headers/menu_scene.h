#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include <scene.h>
#include "utils.h"

class MenuScene : public Scene {
public:
    MenuScene() {}
    ~MenuScene() {}

    bool Init();
    void Update();
    void Draw();
    void Destroy() {}

private:

    sf::Font font;
    sf::Text press_start;

    float textTimer;
};


#endif // MENU_SCENE_H
