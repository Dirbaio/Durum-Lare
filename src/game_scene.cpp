#include "game_scene.h"
#include "defines.h"
#include "input_engine.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include "utils.h"
#include "game_reg.h"
#include "graphics_engine.h"
#include "generator.h"

#include "player.h"

GameScene::GameScene() {

}

GameScene::~GameScene() {

}

void GameScene::initThread() {
    gameReg = GameReg::getInstance();
    GraphEng* graphics = GraphEng::getInstance();

    //Init map
    vector<vector<bool> > v = generateMap();
    map.load(v);

    //Init player
    player.Init();

    //Init camera
    camera.reset(sf::FloatRect(0, 0,
        graphics->getCurrentVideoMode().width,
        graphics->getCurrentVideoMode().height));

    camera.setCenter(sf::Vector2f(0, 0));

    camera.zoom(0.5f);

    initThreadDone = true;
}

bool GameScene::Init() {
    initThreadDone = false;

    GraphEng* graphics = graphics->getInstance();
    sf::Sprite loadScene;
    loadScene.setTexture(*graphics->getTexture("img/loading.png"));
    loadScene.setPosition(App->getView().getSize().x*0.5f - loadScene.getTexture()->getSize().x*0.5f,
                          App->getView().getSize().y*0.5f - loadScene.getTexture()->getSize().y*0.5f);
    loadingText = sf::Text("Loading...");

    sf::Text titleText;
    titleText.setColor(sf::Color::Yellow);
    titleText.setStyle(sf::Text::Bold);
    titleText.setString("DURUM, DALE");
    titleText.setScale(3, 3);
    titleText.setPosition(App->getSize().x*0.25f, App->getSize().y*0.1f);

    sf::Clock timer;
    timer.restart();

    sf::Thread thr_init(&GameScene::initThread, this);
    thr_init.launch();
    while (!initThreadDone) {
        loadingText.setPosition(App->getSize().x*0.35f, App->getSize().y*0.75f);
        loadingText.setColor(sf::Color::Red);
        if (timer.getElapsedTime().asSeconds() > 2) {
            loadingText.setString(loadingText.getString()+".");
            timer.restart();
        }

        App->clear(sf::Color(255,255,255));
        graphics->Draw(&loadScene, GraphEng::GRAPH_LAYER_HUD);
        graphics->Draw(titleText);
        graphics->Draw(loadingText);
        graphics->DrawAll();
        App->display();
    }

    return true;
}

void GameScene::Update() {

    InputEng* input = InputEng::getInstance();


    input->setGlobalMousePos(App->convertCoords(sf::Vector2i(
                                                    input->getMousePos().x,
                                                    input->getMousePos().y),
                                                camera));


    player.Update();

     HandleCamInput();


    HandleEvents();

}



void GameScene::Draw() {
    InputEng* input = InputEng::getInstance();
    GraphEng* graphics = GraphEng::getInstance();

    camera.setCenter(player.getPosition());

    App->setView(camera);
    map.render();

    player.Draw();


    graphics->DrawAll();

    App->setView(App->getDefaultView());

    /*

        //Display mouse position

        std::stringstream ss2(std::stringstream::in | std::stringstream::out);
                ss2 << "MousePos: ";
                ss2 << input->getGlobalMousePos().x;
                ss2 << ", ";
                ss2 << input->getGlobalMousePos().y;
        sf::Text str_mousePos(ss2.str());
        str_mousePos.setColor(sf::Color::Red);
        str_mousePos.setPosition(10, 80);
        //App->draw(str_mousePos);

        */

}

void GameScene::Destroy() {

}


void GameScene::HandleCamInput() {
        InputEng* input = InputEng::getInstance();

        if (input->getKeyState(InputEng::CAM_UP))
            camera.move(0, -input->getFrameTime().asSeconds()*2000);
        if (input->getKeyState(InputEng::CAM_DOWN))
            camera.move(0, input->getFrameTime().asSeconds()*2000);
        if (input->getKeyState(InputEng::CAM_LEFT))
            camera.move(-input->getFrameTime().asSeconds()*2000, 0);
        if (input->getKeyState(InputEng::CAM_RIGHT))
            camera.move(input->getFrameTime().asSeconds()*2000, 0);

/*
        if (input->getKeyDown(InputEng::F5)) {
                if (camTrans.x != NULL) {
                        camera.setSize(sf::Vector2f(camTrans.x->getGoPos(), camera.getSize().y));
                        delete camTrans.x;
                }
                TransitionLinear* tlx = new TransitionLinear();
                tlx->setPos(camera.getSize().x);
                tlx->goPos(camera.getSize().x*0.5f);
                tlx->setTime(0.2f);
                camTrans.x = tlx;

                if (camTrans.y != NULL) {
                        camera.setSize(sf::Vector2f(camera.getSize().x, camTrans.y->getGoPos()));
                        delete camTrans.y;
                }
                TransitionLinear* tly = new TransitionLinear();
                tly = new TransitionLinear();
                tly->setPos(camera.getSize().y);
                tly->goPos(camera.getSize().y*0.5f);
                tly->setTime(0.2f);
                camTrans.y = tly;
        }
        else if (input->getKeyDown(InputEng::F6)) {
                if (camTrans.x != NULL) {
                        camera.setSize(sf::Vector2f(camTrans.x->getGoPos(), camera.getSize().y));
                        delete camTrans.x;
                }
                TransitionLinear* tlx = new TransitionLinear();
                tlx->setPos(camera.getSize().x);
                tlx->goPos(camera.getSize().x*2.0f);
                tlx->setTime(0.2f);
                camTrans.x = tlx;

                if (camTrans.y != NULL) {
                        camera.setSize(sf::Vector2f(camera.getSize().x, camTrans.y->getGoPos()));
                        delete camTrans.y;
                }
                TransitionLinear* tly = new TransitionLinear();
                tly = new TransitionLinear();
                tly->setPos(camera.getSize().y);
                tly->goPos(camera.getSize().y*2.0f);
                tly->setTime(0.2f);
                camTrans.y = tly;
        }

        if (camTrans.x != NULL) {
                camTrans.x->update(input->getFrameTime().asSeconds());
                camera.setSize(camTrans.x->getPos(), camera.getSize().y);
                if (camTrans.x->reached()) {
                        delete camTrans.x;
                        camTrans.x = NULL;
                }
        }

        if (camTrans.y != NULL) {
                camTrans.y->update(input->getFrameTime().asSeconds());
                camera.setSize(camera.getSize().x, camTrans.y->getPos());
                if (camTrans.y->reached()) {
                        delete camTrans.y;
                        camTrans.y = NULL;
                }
        }*/
}




void GameScene::HandleEvents() {
    InputEng* input = InputEng::getInstance();
    GraphEng* graphics = GraphEng::getInstance();

    while (!gameReg->eventQueue.empty()) {
        Event* e = gameReg->eventQueue.front();
        gameReg->eventQueue.pop();
        switch(e->type) {

        /*

        case EVENT_MOVE: {
            EventMove* ev = (EventMove*)e;

            //Get Position
                        sf::Vector2f oldPos = ev->obj->getPosition();
            sf::Vector2f newPos (
                ev->obj->getPosition().x + ((input->getFrameTime().asSeconds() * ev->velocity.x) * ev->direction.x),
                ev->obj->getPosition().y + ((input->getFrameTime().asSeconds() * ev->velocity.y) * ev->direction.y));

            //Bounding box
            std::vector<sf::Vector2f> boxPoints;
                        boxPoints.push_back(sf::Vector2f(newPos.x - (ev->obj->getLocalBounds().width/2), newPos.y));
            boxPoints.push_back(sf::Vector2f(newPos.x + (ev->obj->getLocalBounds().width/2), newPos.y));
                        boxPoints.push_back(sf::Vector2f(newPos.x, newPos.y - (ev->obj->getLocalBounds().height/2)));
            boxPoints.push_back(sf::Vector2f(newPos.x, newPos.y + (ev->obj->getLocalBounds().height/2)));
            boxPoints.push_back(sf::Vector2f(newPos.x - (ev->obj->getLocalBounds().width/2),
                                             newPos.y - (ev->obj->getLocalBounds().height/2)));
            boxPoints.push_back(sf::Vector2f(newPos.x - (ev->obj->getLocalBounds().width/2),
                                             newPos.y + (ev->obj->getLocalBounds().height/2)));
            boxPoints.push_back(sf::Vector2f(newPos.x + (ev->obj->getLocalBounds().width/2),
                                             newPos.y - (ev->obj->getLocalBounds().height/2)));
            boxPoints.push_back(sf::Vector2f(newPos.x + (ev->obj->getLocalBounds().width/2),
                                             newPos.y + (ev->obj->getLocalBounds().height/2)));

            //Check if points collide
            bool canMove = true;
            //for (int i = 0; i < boxPoints.size(); ++i) {
                const sf::Vector2f& point = newPos;

                //Tile Collision
                if (!gameReg->scenario->isTilePassable(point)) canMove = false;

                //Point vs Object Boxes Collision
                 if (gameReg->objMan->isColliding(point) &&
                     gameReg->objMan->getColliding(point)->canCollide()) {
                        if (gameReg->objMan->isColliding(oldPos) &&
                            gameReg->objMan->getColliding(oldPos)->canCollide()) {
                                canMove = true;
                             }
                              else canMove = false;
                       }
            //}

            //All right, officer?
            if (canMove) ev->obj->setPosition(newPos);

            break;
                }*/


        default:
            //Nothing
            break;
        }
        delete e;
    }
}

