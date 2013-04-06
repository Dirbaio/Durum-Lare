#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include <vector>
#include <queue>
#include <list>

#include "game_reg.h"
#include "scene.h"
#include "event.h"
#include "map.h"
#include "city.h"

#include "player.h"
#include "person.h"
#include "police.h"
#include "shop.h"

#include "hud.h"
#include "item.h"


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

    vector<Person*> getPeopleAround(sf::Vector2f pos, float r, SearchType st);
    vector<Person*> getPeopleSeen(Character* c, SearchType st);

    void sendInputToServer();
    void receiveServerInfo();

    vector<Player> players;
    int playerNum;

    void initThread();
    bool initThreadDone;
    sf::Text loadingText;
    void HandleEvents();
    void HandleCamInput();

    //Map map;
    void spawnNewPerson();
    void spawnNewPolice();
    void spawnNewShop();
    void spawnNewMoney(sf::Vector2f pos);

private:

    int m_killedPeople;
    float m_timerSpawnPolice;

    void collide(Character* a);
    vector<vector<vector<Person*> > > estructuraPepinoPeople;
    vector<vector<vector<Police*> > > estructuraPepinoPolice;

    //Registry
    GameReg* gameReg;

    //Statics
    sf::View camera;

    City city;
    Hud hud;

    //Managers
    std::list<Person> personList;
    std::list<Police> policeList;
    std::list<Item> itemList;
    std::list<Shop> shopList;

    sf::Music bg_music;

    TransitionLinear m_camZoomTrans;
    float m_camZoom;
    sf::Vector2f m_camViewportOrg; //Original viewport


    sf::TcpSocket* connSocket;
};

#endif // GAMESCENE_H
