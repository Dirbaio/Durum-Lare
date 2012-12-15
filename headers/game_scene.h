#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <vector>
#include <queue>
#include "scene.h"
#include <SFML/Graphics.hpp>
#include "event.h"
#include "game_reg.h"
#include "map.h"
#include "player.h"

class GameScene : public Scene {
public:
    GameScene();
    ~GameScene();

    bool Init();
    void Update();
    void Draw();
    void Destroy();

private:
    void initThread();
    bool initThreadDone;
    sf::Text loadingText;
    void HandleEvents();
    void HandleCamInput();

    Map map;

    //Registry
    GameReg* gameReg;

    //Statics
    sf::View camera;
    Player player;


    //Managers

};

#endif // GAMESCENE_H
