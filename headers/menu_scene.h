#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include <scene.h>
#include "utils.h"
#include "animation.h"

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

    Animation anim_takena;

    float textTimer;
};


#endif // MENU_SCENE_H
