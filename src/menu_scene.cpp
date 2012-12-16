#include "menu_scene.h"
#include "defines.h"
#include "utils.h"
#include "input_engine.h"
#include "game_scene.h"

bool MenuScene::Init() {

    font.loadFromFile("fonts/BitDarling.TTF");


    press_start.setFont(font);
    press_start.setString("PRESS INTRO  A LOT OF TIMES TO START");

    press_start.setColor(sf::Color(Utils::randomInt(0, 255), Utils::randomInt(0, 255), Utils::randomInt(0, 255)));
    //press_start.setColor(sf::Color::Green);
    press_start.setPosition(App->getSize().x/2 - press_start.getLocalBounds().width/2, App->getSize().y/2);

    textTimer = 0.25f;

    return true;
}



void MenuScene::Update() {

    InputEng* input = InputEng::getInstance();

    float delta = input->getFrameTime().asSeconds();

    textTimer -= delta;
    if (textTimer <= 0) {
        textTimer = 0.25f;
        press_start.setColor(sf::Color(Utils::randomInt(0, 255), Utils::randomInt(0, 255), Utils::randomInt(0, 255)));
    }

    if (input->getKeyDown(InputEng::MENU_START)) nextScene = new GameScene();


}

void MenuScene::Draw() {

    App->draw(press_start);



}



