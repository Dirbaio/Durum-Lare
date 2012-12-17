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

    static void setNewScore(int score);

private:

    sf::Font font;
    sf::Text press_start;
    sf::Text last_points;


    Animation anim_takena;

    float textTimer;

    static int max_points;
};


#endif // MENU_SCENE_H
