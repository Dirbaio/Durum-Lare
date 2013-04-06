#ifndef GAME_H
#define GAME_H

#include "scene.h"

#include "input_engine.h"
#include "graphics_engine.h"

class Game {

public:
    Game() {}

    bool Init();
    void Go();

private:
    Scene* scene;

    GraphEng* graphics;

    int frames; //For framerate calc purpose

};

#endif // GAME_H
