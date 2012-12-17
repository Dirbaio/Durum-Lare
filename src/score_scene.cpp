#include "score_scene.h"
#include "defines.h"
#include "utils.h"
#include "input_engine.h"
#include "graphics_engine.h"
#include "game_scene.h"
#include "animation.h"
#include "menu_scene.h"

bool ScoreScene::Init() {

    GraphEng* graphics = GraphEng::getInstance();

    takena_van.setTexture(*graphics->getTexture("img/takena_van.png"));
    takena_van.setPosition(0, 0);

    takena_van.setScale(App->getSize().x/takena_van.getTextureRect().width,
                        App->getSize().y/takena_van.getTextureRect().height);

    return true;
}



void ScoreScene::Update() {

    InputEng* input = InputEng::getInstance();
    float delta = input->getFrameTime().asSeconds();

    if (input->getKeyDown(InputEng::MENU_START)) nextScene = new GameScene();
}

void ScoreScene::Draw() {

    App->draw(takena_van);


}




