#include "game.h"
#include "defines.h"
#include <SFML/Window.hpp>

#include "input_engine.h"
#include "graphics_engine.h"

#include "scene.h"
#include "game_scene.h"
#include "menu_scene.h"

#include <iostream>
#include <sstream>

sf::RenderWindow* App;
sf::Time global_frametime;
sf::Time scene_total_time;

bool Game::Init() {

	App = new sf::RenderWindow();

	input = InputEng::getInstance();

	graphics = GraphEng::getInstance();
	graphics->init();

    scene = new MenuScene();

	frames = 0;

	return true;

}

void Game::Go() {

	sf::Clock clock1;
	sf::Clock clock2;

	float framerate = 0.0f;

	while (App->isOpen() && scene != NULL && scene != EXIT_SCN) {
		scene->Init();

		sf::Clock sceneTimeElapsed;

		clock1.restart();

		while(App->isOpen() && scene->nextScene == NULL) {

            global_frametime = clock1.getElapsedTime();
            scene_total_time = sceneTimeElapsed.getElapsedTime();
            clock1.restart();

			//Framerate
			if (clock2.getElapsedTime().asMilliseconds() > 100) {
				framerate = (float(frames) / clock2.getElapsedTime().asMilliseconds())*1000;
				clock2.restart();
				frames = 0;
			}
			std::stringstream ss(std::stringstream::in | std::stringstream::out);
			framerate*=100;
			framerate=(int)framerate;
			framerate/=100;
			ss << "FPS: " << framerate;
			sf::Text fpsText(ss.str());
			fpsText.setPosition(10, 10);
			fpsText.setColor(sf::Color::Red);

			std::stringstream ss2(std::stringstream::in | std::stringstream::out);
			ss2 << static_cast<int>(sceneTimeElapsed.getElapsedTime().asSeconds()/60) << " : " <<
			       (((int)sceneTimeElapsed.getElapsedTime().asSeconds())%60) << " . " <<
			       sceneTimeElapsed.getElapsedTime().asMilliseconds()%1000;
			sf::Text timeText(ss2.str());
			timeText.setPosition(App->getSize().x*0.8f, 10);
			timeText.setColor(sf::Color::White);


			//Input
			input->Update();
			if (input->getKeyDown(InputEng::EXIT)) scene->nextScene = EXIT_SCN;

			//Logic
			scene->Update();

			//Draw
			App->clear(sf::Color(12, 12, 12));

			scene->Draw();

            App->draw(fpsText);
            //App->draw(timeText);
			App->display();

			frames++;
		}

		Scene* aux_scn = scene;
		scene = scene->nextScene;
		aux_scn->Destroy();
		delete aux_scn;
	}
}
