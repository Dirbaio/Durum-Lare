#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include <scene.h>
#include "utils.h"
#include "animation.h"

class ScoreScene : public Scene {
public:
    ScoreScene() {}
    ~ScoreScene() {}

    bool Init();
    void Update();
    void Draw();
    void Destroy() {}

private:
    sf::Sprite takena_van;

};


#endif // MENU_SCENE_H
