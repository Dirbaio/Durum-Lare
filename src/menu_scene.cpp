#include "menu_scene.h"
#include "defines.h"
#include "utils.h"
#include "input_engine.h"
#include "game_scene.h"
#include "animation.h"

#include <sstream>

int MenuScene::max_points = 0;

bool MenuScene::Init() {

	std::stringstream points;
	points << "MAX MONEY: " << max_points;

	font.loadFromFile("fonts/BitDarling.TTF");
	last_points.setFont(font);
	last_points.setString(points.str().c_str());
	last_points.setColor(sf::Color::Green);
	last_points.setPosition(App->getSize().x/2 - last_points.getLocalBounds().width/2, App->getSize().y*0.85f);

	press_start.setFont(font);
	press_start.setString("PRESS INTRO TO START");
	press_start.setColor(sf::Color(Utils::randomInt(0, 255), Utils::randomInt(0, 255), Utils::randomInt(0, 255)));
	press_start.setPosition(App->getSize().x/2 - press_start.getLocalBounds().width/2, App->getSize().y*0.75f);

	textTimer = 0.25f;

	AnimationData* ad = new AnimationData();
	ad->Load("anim/takena.anim");
	anim_takena.setAnimData(ad);
	anim_takena.SelectAnim("WalkingDown");

	return true;
}



void MenuScene::Update() {

    float delta = input.getFrameTime().asSeconds();

    textTimer -= delta;
    if (textTimer <= 0) {
        textTimer = 0.25f;
        press_start.setColor(sf::Color(Utils::randomInt(0, 255), Utils::randomInt(0, 255), Utils::randomInt(0, 255)));
    }

    if (input.getKeyState(InputEng::MENU_START)) {
	    nextScene = new GameScene();
    }

    anim_takena.Update(delta);
}

void MenuScene::Draw() {

    //Takena walking
    sf::Sprite* spr = anim_takena.getCurrentFrame();
    spr->setOrigin(16, 16);
    spr->setPosition(App->getSize().x*0.5f, App->getSize().y*0.5f);
    spr->setScale(10, 10);
    App->draw(*spr);


    //Start letters
    App->draw(press_start);
    if (max_points > 0) App->draw(last_points);
}

void MenuScene::setNewScore(int score)
{
	if (score > max_points) max_points = score;
}



