#include "game_scene.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>


#include <iostream>
#include <vector>
#include <list>
#include <sstream>

#include "defines.h"
#include "utils.h"
#include "input_engine.h"
#include "graphics_engine.h"

#include "generator.h"

#include "player.h"
#include "person.h"
#include "police.h"

#include "hud.h"
#include "item.h"
#include "item_factory.h"
#include "menu_scene.h"

#include "shop.h"

GameScene::GameScene() {

}

GameScene::~GameScene() {
    bg_music.stop();
}

void GameScene::initThread() {

}

bool GameScene::Init() {

    cout<<"Connecting..."<<endl;
    connSocket = new sf::TcpSocket();
    if(connSocket->connect("127.0.0.1", 6174) != sf::Socket::Done)
    {
        cerr<<"Can't connect shit"<<endl;
        exit(1);
    }

    int playerCount = 1;

    sf::Packet packet = receivePacket();
    int mapSize, personCount, policeCount, seed;
    packet >> playerNum >> playerCount >> mapSize >> personCount >> policeCount >> seed;
    srand(seed); //VERRRY IMPORRRRRTANT

    /*
    GraphEng* graphics = graphics->getInstance();

    sf::Sprite loadScene;
    loadScene.setTexture(*graphics->getTexture("img/loading.png"));
    loadScene.setPosition(App->getView().getSize().x*0.5f - loadScene.getTexture()->getSize().x*0.5f,
                  App->getView().getSize().y*0.5f - loadScene.getTexture()->getSize().y*0.5f);
    loadingText = sf::Text("Loading...");*/

    sf::Text titleText;
    titleText.setColor(sf::Color::Yellow);
    titleText.setStyle(sf::Text::Bold);
    titleText.setString("GiTA");
    titleText.setScale(3, 3);
    titleText.setPosition(
                (float) App->getSize().x*0.25f,
                (float) App->getSize().y*0.1f);

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

    
    GraphEng* graphics = GraphEng::getInstance();

    //Init map
    city.init(mapSize, mapSize);

    //Init player
    players = vector<Player> (playerCount, Player(this));
    for(int i = 0; i < playerCount; i++)
    {
        players[i].Init();
        players[i].setPosition(city.getRandomStreet());
    }


    //Init camera
    camera.reset(sf::FloatRect(0, 0,
                               (float) graphics->getCurrentVideoMode().width,
                               (float) graphics->getCurrentVideoMode().height));


    m_camViewportOrg.x =  (float) graphics->getCurrentVideoMode().width;
    m_camViewportOrg.y = (float) graphics->getCurrentVideoMode().height;

    //Init NPCS
    for (int i = 0; i < personCount; ++i) spawnNewPerson();
    for (int i = 0; i < policeCount; ++i) spawnNewPolice();

    //Init Shops
//    for (int i = 0; i < 5; ++i) spawnNewShop();

    //Init Camera
    camera.setCenter(sf::Vector2f(0, 0));
    m_camZoom = 0.5f;

    //Init background music
    bg_music.openFromFile("audio/surrounding.ogg");
    bg_music.setLoop(true);
    //	bg_music.play();

    //Init hud
    hud.Init();

    //Init statistics
    m_killedPeople = 0;

    m_timerSpawnPolice = 60.0f;

    return true;
}

void GameScene::spawnNewShop() {

    Shop shop (this);

    shop.Init();
    shop.setPosition(city.getRandomClearing());

    shopList.push_back(shop);

}

void GameScene::spawnNewMoney(sf::Vector2f pos) {
    Item* item;
    if (Utils::randomInt(1, 20) == 1) item =  ItemFactory::MakeNewItem(this, ITEM_BIG_MONEY);
    else item =  ItemFactory::MakeNewItem(this, ITEM_MONEY);
    item->setPosition(pos);
    item->setTransPos(pos, pos + sf::Vector2f(
                          (float) Utils::randomInt(-16, 16),
                          (float) Utils::randomInt(-16, 16)));
    itemList.push_back(*item);
    delete item;
}


void GameScene::spawnNewPerson() {
    Person p(this);
    p.Init();
    p.setPosition(city.getRandomStreet());
    personList.push_back(p);
}

void GameScene::spawnNewPolice() {
    Police p(this);
    p.Init();
    p.setPosition(city.getRandomStreet());
    policeList.push_back(p);
}

vector<Person*> GameScene::getPeopleAround(sf::Vector2f pos, float r, SearchType st)
{
    sf::Vector2f min = pos - sf::Vector2f(r, r);
    sf::Vector2f max = pos + sf::Vector2f(r, r);
    int xmin = (int) (min.x / 64.0f);
    int ymin = (int) (min.y / 64.0f);
    int xmax = (int) (max.x / 64.0f);
    int ymax = (int) (max.y / 64.0f);
    if(xmin < 0) xmin = 0;
    if(ymin < 0) ymin = 0;
    if(xmax >=TILESIZE) xmax =TILESIZE-1;
    if(ymax >=TILESIZE) ymax =TILESIZE-1;

    vector<Person*> res;
    for(int x = xmin; x <= xmax; x++) {
        for(int y = ymin; y <= ymax; y++)
        {
            vector<Person*>& v = estructuraPepinoPeople[x][y];
            for(int i = 0; i < (int) v.size(); i++)
                if( (st == SEARCH_ANY ||
                     (st == SEARCH_DEAD && !v[i]->is_alive()) ||
                     (st == SEARCH_PANIC && v[i]->getState() == Person::STATE_PANIC))
                        && Utils::distance(pos, v[i]->m_position) <= r)
                    res.push_back(v[i]);
        }
    }

    return res;
}

void GameScene::collide(Character* a)
{
    sf::Vector2f pos = a->m_position;
    float r = 10;

    sf::Vector2f min = pos - sf::Vector2f(r, r);
    sf::Vector2f max = pos + sf::Vector2f(r, r);
    int xmin = (int) (min.x / 64.0f);
    int ymin = (int) (min.y / 64.0f);
    int xmax = (int) (max.x / 64.0f);
    int ymax = (int) (max.y / 64.0f);
    if(xmin < 0) xmin = 0;
    if(ymin < 0) ymin = 0;
    if(xmax >=TILESIZE) xmax =TILESIZE-1;
    if(ymax >=TILESIZE) ymax =TILESIZE-1;

    for(int x = xmin; x <= xmax; x++) {
        for(int y = ymin; y <= ymax; y++)
        {
            {
                vector<Person*>& v = estructuraPepinoPeople[x][y];
                for(int i = 0; i < (int) v.size(); i++)
                {
                    Character* b = v[i];

                    if(Utils::distance(pos, b->m_position) < 10)
                    {
                        sf::Vector2f l = b->m_position - a->m_position;
                        if(Utils::norm(l) == 0) continue;

                        Utils::normalize(l);
                        sf::Vector2f m = (b->m_position + a->m_position) / 2.0f;
                        a->move(m-l*5.0f);
                        b->move(m+l*5.0f);
                    }
                }
            }

            {
                vector<Police*>& v = estructuraPepinoPolice[x][y];
                for(int i = 0; i < (int) v.size(); i++)
                {
                    Character* b = v[i];

                    if(Utils::distance(pos, b->m_position) < 10)
                    {
                        sf::Vector2f l = b->m_position - a->m_position;
                        if(Utils::norm(l) == 0) continue;

                        Utils::normalize(l);
                        sf::Vector2f m = (b->m_position + a->m_position) / 2.0f;
                        a->move(m-l*5.0f);
                        b->move(m+l*5.0f);
                    }
                }
            }
        }
    }
}

vector<Person*> GameScene::getPeopleSeen(Character* c, SearchType st)
{
    float r = 180;

    sf::Vector2f min = c->m_position - sf::Vector2f(r, r);
    sf::Vector2f max = c->m_position + sf::Vector2f(r, r);
    int xmin = (int) (min.x / 64.0f);
    int ymin = (int) (min.y / 64.0f);
    int xmax = (int) (max.x / 64.0f);
    int ymax = (int) (max.y / 64.0f);

    if(xmin < 0) xmin = 0;
    if(ymin < 0) ymin = 0;
    if(xmax >=TILESIZE) xmax =TILESIZE-1;
    if(ymax >=TILESIZE) ymax =TILESIZE-1;

    vector<Person*> res;
    for(int x = xmin; x <= xmax; x++) {
        for(int y = ymin; y <= ymax; y++)
        {
            vector<Person*>& v = estructuraPepinoPeople[x][y];
            for(int i = 0; i < (int) v.size(); i++)
                if( (st == SEARCH_ANY ||
                     (st == SEARCH_DEAD && !v[i]->is_alive()) ||
                     (st == SEARCH_PANIC && v[i]->getState() == Person::STATE_PANIC))
                        && c->canSee(v[i]->m_position))
                    res.push_back(v[i]);
        }
    }

    return res;
}

void GameScene::sendInputToServer() {
    sf::Packet packet;
    packet << input.encodeToString();
    connSocket->send(packet);
}

sf::Packet GameScene::receivePacket() {
    sf::Packet packet;
    if (connSocket->receive(packet) != sf::Socket::Done) {
        cout << "[ERROR] PACKET COSAS CHUNGAS" << endl;
        nextScene = new MenuScene();
    }
    return packet;
}

void GameScene::receiveServerInfo() {

    sf::Packet packet = receivePacket();

    for(int i = 0; i < (int)players.size(); i++)
    {
        std::string str;
        packet >> str;
        players[i].playerInput.decodeFromString(str);
    }
}

void GameScene::Update() {

    float delta = input.getFrameTime().asSeconds();

    //Timers
    m_timerSpawnPolice -= delta;
    if (m_timerSpawnPolice <= 0) {
        m_timerSpawnPolice = 60.0f;
        spawnNewPolice();
    }

    //	if (input.getKeyDown(InputEng::NEW_SCENE))
    //		this->nextScene = new GameScene();
    input.setGlobalMousePos(
                App->convertCoords(sf::Vector2i(
                                       (int) input.getMousePos().x,
                                       (int) input.getMousePos().y), camera));


    //Camera Zoom
    if (input.getKeyDown(InputEng::CAM_ZOOM_IN)) {
        m_camZoomTrans.setPos(m_camZoom);
        m_camZoomTrans.goPos(0.4f);
        m_camZoomTrans.setTime(0.2f);
    }
    if (input.getKeyDown(InputEng::CAM_ZOOM_OUT)) {
        m_camZoomTrans.setPos(m_camZoom);
        m_camZoomTrans.goPos(0.5f);
        m_camZoomTrans.setTime(0.2f);
    }
    if (!m_camZoomTrans.reached()) {
        m_camZoomTrans.update(delta);
        m_camZoom = m_camZoomTrans.getPos();
    }

    if(players[playerNum].m_jailed)
    {
        float speed = 200;

        sf::Vector2f center = camera.getCenter();
        if(input.getKeyState(InputEng::PLAYER_UP)) center.y -= delta*speed;
        if(input.getKeyState(InputEng::PLAYER_DOWN)) center.y += delta*speed;
        if(input.getKeyState(InputEng::PLAYER_LEFT)) center.x -= delta*speed;
        if(input.getKeyState(InputEng::PLAYER_RIGHT)) center.x += delta*speed;
        camera.setCenter(center);

        if(input.getKeyDown(InputEng::MENU_START))
            gameOver();
    }
    else
        camera.setCenter(players[playerNum].getPosition());


    estructuraPepinoPeople = vector<vector<vector<Person*> > > (TILESIZE, vector<vector<Person*> >(TILESIZE));
    estructuraPepinoPolice = vector<vector<vector<Police*> > > (TILESIZE, vector<vector<Police*> >(TILESIZE));

    for (std::list<Person>::iterator it = personList.begin(); it != personList.end(); ++it) {
        sf::Vector2i p = city.absoluteToTilePos(it->m_position);
        if(!city.validTile(p.x, p.y)) continue;
        estructuraPepinoPeople[p.x][p.y].push_back(&*it);
    }
    for (std::list<Police>::iterator it = policeList.begin(); it != policeList.end(); ++it) {
        sf::Vector2i p = city.absoluteToTilePos(it->m_position);
        if(!city.validTile(p.x, p.y)) continue;
        estructuraPepinoPolice[p.x][p.y].push_back(&*it);
    }

    //Player update

    sendInputToServer();
    receiveServerInfo();

    for(int i = 0; i < (int) players.size(); i++)
       players[i].Update();



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

    //Shops update
    for (std::list<Shop>::iterator it = shopList.begin(); it != shopList.end(); ++it) {
        it->Update();
    }

    //COLLISIONS !!!!
    for(int i = 0; i < (int) players.size(); i++)
        collide(&players[i]);
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

}

bool comp(Object* a, Object* b)
{
    if(a->m_prio == b->m_prio) {
        int ya = int(a->getPosition().y+0.1);
        int yb = int(b->getPosition().y+0.1);
        if(ya == yb) return a->getUniqueID() < b->getUniqueID();
        return ya < yb;
    }
    else return a->m_prio < b->m_prio;
}

void GameScene::Draw() {
    GraphEng* graphics = GraphEng::getInstance();

    camera.setSize(m_camViewportOrg.x*m_camZoom, m_camViewportOrg.y*m_camZoom);

    App->setView(camera);

    //Map draw
    city.render();

    vector<Object*> v;
    for(int i = 0; i < (int)players.size(); i++)
        v.push_back(&players[i]);

    bool seeAll = true; //Sino, es una mierda como una casa.

    Player& player = players[playerNum];
    for (std::list<Person>::iterator it = personList.begin(); it != personList.end(); ++it)
        if(seeAll || player.canSee(it->getPosition()))
            v.push_back(&*it);
    for (std::list<Police>::iterator it = policeList.begin(); it != policeList.end(); ++it)
        if(seeAll || player.canSee(it->getPosition()))
            v.push_back(&*it);
    for (std::list<Item>::iterator it = itemList.begin(); it != itemList.end(); ++it)
        if(seeAll || player.canSee(it->getPosition()))
            v.push_back(&*it);
    for (std::list<Shop>::iterator it = shopList.begin(); it != shopList.end(); ++it)
        if(seeAll || player.canSee(it->getPosition()))
            v.push_back(&*it);

    sort(v.begin(), v.end(), comp);
    for(int i = 0; i < (int) v.size(); i++)
    {
        v[i]->Draw();
        v[i]->DrawMark();
    }

    //Map draw
    city.renderTop();
    graphics->DrawAll();
    App->setView(App->getDefaultView());
    hud.Draw();
}

void GameScene::Destroy() {
    cout<<"Disconnecting..."<<endl;
    connSocket->disconnect();
}

void GameScene::gameOver() {
    MenuScene::setNewScore(1337); //player.getMoney());
    nextScene = new MenuScene();
}
