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
#include "city.h"

#include <list>
#include "person.h"
#include "police.h"

#include "hud.h"

#include <SFML/Audio.hpp>

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

	//Map map;
	void spawnNewPerson();
	void spawnNewPolice();


	//Registry
	GameReg* gameReg;

	//Statics
	sf::View camera;
	Player player;
	City city;
        Hud hud;

	//Managers
	std::list<Person> personList;
	std::list<Police> policeList;


        sf::Music bg_music;
};

#endif // GAMESCENE_H
