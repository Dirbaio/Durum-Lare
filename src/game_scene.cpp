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
#include <SFML/Audio.hpp>
#include "player.h"
#include "person.h"
#include "police.h"
#include <list>
#include "hud.h"
#include "item.h"
#include "item_factory.h"
#include "score_scene.h"

GameScene::GameScene() {

}

GameScene::~GameScene() {
	bg_music.stop();
}

void GameScene::initThread() {
	gameReg = GameReg::getInstance();
	gameReg->city = &city;
	gameReg->personList = &personList;
	gameReg->policeList = &policeList;
	gameReg->itemList = &itemList;
	gameReg->player = &player;
	gameReg->scene = this;

	GraphEng* graphics = GraphEng::getInstance();

	//Init map
	city.init(99,99, 64,64);
	//Init player
	player.Init();
	player.setPosition(city.getRandomStreet());


	//Init camera
	camera.reset(sf::FloatRect(0, 0,
				   graphics->getCurrentVideoMode().width,
				   graphics->getCurrentVideoMode().height));

	//Init NPCS

    for (int i = 0; i < 200; ++i) spawnNewPerson();
    for (int i = 0; i < 30; ++i) spawnNewPolice();

	//Init Camera
	camera.setCenter(sf::Vector2f(0, 0));
        camera.zoom(0.4f);

	//Init background music
	bg_music.openFromFile("audio/surrounding.ogg");
	bg_music.setLoop(true);;
	bg_music.play();

	//Init hud
	hud.Init();

	//Le oc
	initThreadDone = true;
}

bool GameScene::Init() {
	initThreadDone = false;

	GraphEng* graphics = graphics->getInstance();
       /*
	sf::Sprite loadScene;
	loadScene.setTexture(*graphics->getTexture("img/loading.png"));
	loadScene.setPosition(App->getView().getSize().x*0.5f - loadScene.getTexture()->getSize().x*0.5f,
			      App->getView().getSize().y*0.5f - loadScene.getTexture()->getSize().y*0.5f);
	loadingText = sf::Text("Loading...");
*/
	sf::Text titleText;
	titleText.setColor(sf::Color::Yellow);
	titleText.setStyle(sf::Text::Bold);
	titleText.setString("DURUM, DALE");
	titleText.setScale(3, 3);
	titleText.setPosition(App->getSize().x*0.25f, App->getSize().y*0.1f);

	sf::Clock timer;
	timer.restart();


	/*
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
    }*/

	initThread();

	return true;
}

void GameScene::spawnNewMoney(sf::Vector2f pos) {
	Item* item = ItemFactory::MakeNewItem(ItemFactory::ITEM_MONEY);
	item->setPosition(pos);
	item->setTransPos(pos, pos + sf::Vector2f(Utils::randomInt(-16, 16), Utils::randomInt(-16, 16)));
	itemList.push_back(*item);
	delete item;
}


void GameScene::spawnNewPerson() {

	Person p;
	p.Init();
	p.setPosition(city.getRandomStreet());

	personList.push_back(p);

}

void GameScene::spawnNewPolice() {

	Police p;
	p.Init();
	p.setPosition(city.getRandomStreet());

	policeList.push_back(p);
}

vector<Person*> GameScene::getPeopleAround(vec2 pos, float r, SearchType st)
{
	vec2 min = pos - vec2(r, r);
	vec2 max = pos + vec2(r, r);
	int xmin = min.x / 64;
	int ymin = min.y / 64;
	int xmax = max.x / 64;
	int ymax = max.y / 64;
	if(xmin < 0) xmin = 0;
	if(ymin < 0) ymin = 0;
	if(xmax >= city.getTW()) xmax = city.getTW()-1;
	if(ymax >= city.getTH()) ymax = city.getTH()-1;

	vector<Person*> res;
	for(int x = xmin; x <= xmax; x++)
		for(int y = ymin; y <= ymax; y++)
		{
			vector<Person*>& v = estructuraPepinoPeople[x][y];
			for(int i = 0; i < v.size(); i++)
				if( (st == SEARCH_ANY || 
					(st == SEARCH_DEAD && !v[i]->is_alive()) || 
					(st == SEARCH_PANIC && v[i]->getState() == Person::STATE_PANIC))
						 && Utils::distance(pos, v[i]->m_position) <= r)
					res.push_back(v[i]);
		}
	cout << "ASDF "<<res.size()<<endl;
	return res;
}

void GameScene::collide(Character* a)
{
	vec2 pos = a->m_position;
	float r = 10;
	
	vec2 min = pos - vec2(r, r);
	vec2 max = pos + vec2(r, r);
	int xmin = min.x / 64;
	int ymin = min.y / 64;
	int xmax = max.x / 64;
	int ymax = max.y / 64;
	if(xmin < 0) xmin = 0;
	if(ymin < 0) ymin = 0;
	if(xmax >= city.getTW()) xmax = city.getTW()-1;
	if(ymax >= city.getTH()) ymax = city.getTH()-1;

	for(int x = xmin; x <= xmax; x++)
		for(int y = ymin; y <= ymax; y++)
		{
			{
				vector<Person*>& v = estructuraPepinoPeople[x][y];
				for(int i = 0; i < v.size(); i++)
				{
					Character* b = v[i];
				
					if(Utils::distance(pos, b->m_position) < 10)
					{
						vec2 l = b->m_position - a->m_position;
						if(Utils::norm(l) == 0) continue;
				
						Utils::normalize(l);
						vec2 m = (b->m_position + a->m_position) / 2.0f;
						a->move(m-l*5.0f);
						b->move(m+l*5.0f);
					}
				}
			}
			
			{
				vector<Police*>& v = estructuraPepinoPolice[x][y];
				for(int i = 0; i < v.size(); i++)
				{
					Character* b = v[i];
				
					if(Utils::distance(pos, b->m_position) < 10)
					{
						vec2 l = b->m_position - a->m_position;
						if(Utils::norm(l) == 0) continue;
				
						Utils::normalize(l);
						vec2 m = (b->m_position + a->m_position) / 2.0f;
						a->move(m-l*5.0f);
						b->move(m+l*5.0f);
					}
				}
			}
		}
}

vector<Person*> GameScene::getPeopleSeen(Character* c, SearchType st)
{
	float r = 180;
	
	vec2 min = c->m_position - vec2(r, r);
	vec2 max = c->m_position + vec2(r, r);
	int xmin = min.x / 64;
	int ymin = min.y / 64;
	int xmax = max.x / 64;
	int ymax = max.y / 64;
//	cout<<xmin<<" "<<xmax<<" "<<ymin<<" "<<ymax<<endl;
	if(xmin < 0) xmin = 0;
        if(ymin < 0) ymin = 0;
	if(xmax >= city.getTW()) xmax = city.getTW()-1;
	if(ymax >= city.getTH()) ymax = city.getTH()-1;
	
	vector<Person*> res;
	for(int x = xmin; x <= xmax; x++)
		for(int y = ymin; y <= ymax; y++)
		{
			vector<Person*>& v = estructuraPepinoPeople[x][y];
			for(int i = 0; i < v.size(); i++)
				if( (st == SEARCH_ANY || 
					(st == SEARCH_DEAD && !v[i]->is_alive()) || 
					(st == SEARCH_PANIC && v[i]->getState() == Person::STATE_PANIC))
						 && c->canSee(v[i]->m_position))
					res.push_back(v[i]);
		}
	return res;
}

void GameScene::Update() {

	InputEng* input = InputEng::getInstance();


	if (input->getKeyDown(InputEng::NEW_SCENE))
		this->nextScene = new GameScene();

	input->setGlobalMousePos(
				App->convertCoords(sf::Vector2i(
							   input->getMousePos().x,
							   input->getMousePos().y),camera));

	estructuraPepinoPeople = vector<vector<vector<Person*> > > (city.getTW(), vector<vector<Person*> >(city.getTH()));
	estructuraPepinoPolice = vector<vector<vector<Police*> > > (city.getTW(), vector<vector<Police*> >(city.getTH()));

	for (std::list<Person>::iterator it = personList.begin(); it != personList.end(); ++it) {
		vec2i p = city.absoluteToTilePos(it->m_position);
		if(!city.validTile(p.x, p.y)) continue;
		estructuraPepinoPeople[p.x][p.y].push_back(&*it);
	}
	for (std::list<Police>::iterator it = policeList.begin(); it != policeList.end(); ++it) {
		vec2i p = city.absoluteToTilePos(it->m_position);
		if(!city.validTile(p.x, p.y)) continue;
		estructuraPepinoPolice[p.x][p.y].push_back(&*it);
	}
	
	//Player update
	player.Update();

	//Persons update
	for (std::list<Person>::iterator it = personList.begin(); it != personList.end(); ++it) {
		it->Update();
	}

	//Police update
	for (std::list<Police>::iterator it = policeList.begin(); it != policeList.end(); ++it) {
		it->Update();
	}

	//Items update
	for (std::list<Item>::iterator it = itemList.begin(); it != itemList.end(); ++it) {
		it->Update();
	}

	//COLLISIONS !!!!
	collide(&player);
	for (std::list<Person>::iterator it = personList.begin(); it != personList.end(); ++it)
		collide(&*it);
	for (std::list<Police>::iterator it = policeList.begin(); it != policeList.end(); ++it)
		collide(&*it);
		
	//Delete persons
	for (std::list<Person>::iterator it = personList.begin(); it != personList.end();) {
		if(it->isToBeDeleted())
		{
			std::list<Person>::iterator it2 = it;
			it2++;
			personList.erase(it);
			it = it2;
			spawnNewPerson();
		}
		else ++it;
	}

	//Delete items
	for (std::list<Item>::iterator it = itemList.begin(); it != itemList.end();) {
		if(it->isToBeDeleted())
		{
			std::list<Item	>::iterator it2 = it;
			it2++;
			itemList.erase(it);
			it = it2;
		}
		else ++it;
	}

	HandleCamInput();
	HandleEvents();
}

bool comp(Object* a, Object* b)
{
	if(a->m_prio == b->m_prio)
	{
		int ya = int(a->getPosition().y+0.1);
		int yb = int(b->getPosition().y+0.1);
		if(ya == yb)
			return a->getUniqueID() < b->getUniqueID();
		return ya < yb;
	}
	else
		return a->m_prio < b->m_prio;
}

void GameScene::Draw() {
	InputEng* input = InputEng::getInstance();
	GraphEng* graphics = GraphEng::getInstance();

	camera.setCenter(player.getPosition());
	App->setView(camera);

	//Map draw
	city.render();
	graphics->DrawAll();

	vector<Object*> v;

	v.push_back(&player);

	for (std::list<Person>::iterator it = personList.begin(); it != personList.end(); ++it)
		v.push_back(&*it);
	for (std::list<Police>::iterator it = policeList.begin(); it != policeList.end(); ++it)
		v.push_back(&*it);
	for (std::list<Item>::iterator it = itemList.begin(); it != itemList.end(); ++it)
		v.push_back(&*it);

	sort(v.begin(), v.end(), comp);
	for(int i = 0; i < v.size(); i++)
	{
		v[i]->Draw();
		v[i]->DrawMark();
	}

	//Map draw
	city.renderTop();

	graphics->DrawAll();

	App->setView(App->getDefaultView());

	hud.Draw();


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

		case EVENT_PLAYER_ACTION:
		{
			EventPlayerAction* ev = (EventPlayerAction*)e;

			player.hitAction();

			std::vector<Person*> persons = getPeopleAround(player.getPosition(), 13, SEARCH_ANY);
//			std::vector<Person*> persons = getPeopleSeen(&player, SEARCH_ANY);
			for (std::vector<Person*>::iterator it = persons.begin(); it != persons.end(); ++it) {
				if (!(*it)->is_alive()) continue;
				player.setKills(player.getKills()+1);
				(*it)->onHit();
				int n_moneys = Utils::randomInt(1, 3);
				for (int i = 0; i < n_moneys; ++i) spawnNewMoney((*it)->getPosition());
			}

			break;
		}

/*
		case EVENT_DELETE_PERSON: {
			EventDeletePerson* ev = (EventDeletePerson*)e;

			for (std::list<Person>::iterator it = personList.begin(); it != personList.end(); ++it) {
				if (&(*it) == ev->person) {
					it = personList.erase(it);
					break;
				}
			}

			spawnNewPerson();
			break;
		}

		case EVENT_DELETE_ITEM: {
			EventDeleteItem* ev = (EventDeleteItem*)e;

			for (std::list<Item>::iterator it = itemList.begin(); it != itemList.end(); ++it) {
				if ((*it) == ev->item) {
					delete (*it);
					it = itemList.erase(it);
					break;
				}
			}

			break;
		}*/

		case EVENT_GAME_OVER: {
			EventGameOver* ev = (EventGameOver*)e;
                        nextScene = new ScoreScene();
			break;
		}

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
