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
#include "item.h"
#include <SFML/Audio.hpp>

enum SearchType {
    SEARCH_ANY,
    SEARCH_DEAD,
    SEARCH_PANIC
};

class GameScene : public Scene {
public:
	GameScene();
	~GameScene();

	bool Init();
	void Update();
	void Draw();
	void Destroy();

	vector<Person*> getPeopleAround(vec2 pos, float r, SearchType st);
	vector<Person*> getPeopleSeen(Character* c, SearchType st);

private:
	void initThread();
	bool initThreadDone;
	sf::Text loadingText;
	void HandleEvents();
	void HandleCamInput();

	//Map map;
	void spawnNewPerson();
	void spawnNewPolice();
    void spawnNewMoney(sf::Vector2f pos);

	void collide(Character* a);
	vector<vector<vector<Person*> > > estructuraPepinoPeople;
	vector<vector<vector<Police*> > > estructuraPepinoPolice;

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
    std::list<Item> itemList;

    sf::Music bg_music;
};

#endif // GAMESCENE_H
