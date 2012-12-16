#include "defines.h"
#include <SFML/Graphics.hpp>
#include "graphics_engine.h"
#include <SFML/Window.hpp>
#include <map>
#include <string>
#include <iostream>
#include <algorithm>

#include "game_reg.h"

GraphEng* GraphEng::m_pInstance = 0;
bool GraphEng::m_bInstanced = false;

GraphEng:: GraphEng() {
        if (!m_bInstanced) {
                atexit(GraphEng::deleteInstance);
                m_bInstanced = true;
        }
}


GraphEng* GraphEng::getInstance() {
        if (GraphEng::m_pInstance == 0) {
                m_pInstance = new GraphEng();
        }

        return m_pInstance;
}

void GraphEng::deleteInstance()
{
        if (m_pInstance) delete m_pInstance;
}

void GraphEng::init() {

        //sf::Texture* img = getTexture("img/logo.png");
        //App->SetIcon(img->getWidth(), img->getHeight(), img->GetPixelsPtr());

        App->setFramerateLimit(0);

        m_videoModes = sf::VideoMode::getFullscreenModes();
        m_currentVideoMode = 0;

        //App->Create(m_videoModes[0], "Durum dale");
        App->create(m_videoModes[0], "Durum dale", sf::Style::Close);
        //App->create(sf::VideoMode(1024, 768), "Durum dale", sf::Style::Fullscreen);

        //worldRendTex.create(m_videoModes[0].width, m_videoModes[0].height);
       // whiteTex.create(m_videoModes[0].width, m_videoModes[0].height);
       // whiteTex.clear(sf::Color::White);

        //Activate separators
        drawQueue[GRAPH_LAYER_WWSEP].push(NULL);
        drawQueue[GRAPH_LAYER_WWSEP].pop();

        myFont.loadFromFile("data/256BYTES.ttf");

        return;

}

sf::VideoMode GraphEng::getCurrentVideoMode() {
        return m_videoModes[m_currentVideoMode];
}


sf::Texture* GraphEng::getTexture(std::string path) {

        std::map<std::string, sf::Texture*>::iterator it;
        it = imgMap.find(path);
        if (it == imgMap.end()) {
                std::cerr << "* Loading image: " << path << std::endl;
                imgMap[path] = new sf::Texture();
                imgMap[path]->loadFromFile(path);
                imgMap[path]->setSmooth(false);

                it = imgMap.find(path);
        }
        return it->second;

}

void GraphEng::Draw(sf::Sprite* drawable) {
    Draw(drawable, GraphEng::GRAPH_LAYER_STANDARD);
}

void GraphEng::Draw(sf::Text text) {
        textQueue[GRAPH_LAYER_POPUP].push(text);
}

void GraphEng::Draw(sf::Text text, int layer) {
        textQueue[layer].push(text);
}

void GraphEng::Draw(sf::Sprite* drawable, int layer) {

        if (layer == GRAPH_LAYER_TILE) instaDraw(drawable);

        drawQueue[layer].push(drawable);
}

void GraphEng::instaDraw(sf::Sprite* spr) {
    App->draw(*spr);
        //worldRendTex.setView(App->getView());
        //worldRendTex.draw(*spr);
}

bool sortfuncspr(sf::Sprite* i, sf::Sprite* j) {
        sf::Vector2f posI = sf::Vector2f(i->getPosition().x + i->getOrigin().x, i->getPosition().y + i->getOrigin().y);
        sf::Vector2f posJ = sf::Vector2f(j->getPosition().x + j->getOrigin().x, j->getPosition().y + j->getOrigin().y);

        if (posI.y == posJ.y) {
                return (posI.x < posJ.x);
        }
        return (posI.y < posJ.y);
}

void GraphEng::DrawAll() {
        std::map<int, std::queue<sf::Sprite*> >::iterator sprIt;

        //Draw sprites
        for (sprIt = drawQueue.begin(); sprIt != drawQueue.end(); ++sprIt) {

                std::vector<sf::Sprite*> drawVec;

                while (!sprIt->second.empty()) {
                        drawVec.push_back(sprIt->second.front());
                        sprIt->second.pop();
                }

                sort(drawVec.begin(), drawVec.end(), sortfuncspr);

                //Display effects
                if (sprIt->first == GRAPH_LAYER_WWSEP) {
                        //drawFogOfWar();
                        //drawDayEffect();
                }

                //Hud rendering
                else if (sprIt->first > GRAPH_LAYER_WWSEP) {
                        sf::View old_view = App->getView();
                        App->setView(App->getDefaultView());

                        for (int i = 0; i < drawVec.size(); ++i)
                                App->draw(*drawVec[i]);

                        App->setView(old_view);
                }

                //World map rendering
                else {
                      //  worldRendTex.setView(App->getView());
                        for (int i = 0; i < drawVec.size(); ++i) {
                                   //     worldRendTex.draw(*drawVec[i]);
                        }
                }

        }

        //Draw texts
        std::map<int, std::queue<sf::Text> >::iterator txtIt;
        for (txtIt = textQueue.begin(); txtIt != textQueue.end(); ++txtIt) {
                if (txtIt->first > GRAPH_LAYER_WWSEP) {
                        sf::View old_view = App->getView();
                        App->setView(App->getDefaultView());

                        while (!txtIt->second.empty()) {
                                //txtIt->second.front().setFont(myFont);
                                App->draw(txtIt->second.front());
                                txtIt->second.pop();
                        }

                        App->setView(old_view);
                }

                else while (!txtIt->second.empty()) {
                        //txtIt->second.front().setFont(myFont);
                        App->draw(txtIt->second.front());
                        txtIt->second.pop();
                }
        }

}
/*
void GraphEng::drawDayEffect() {
        //if (dayCycleFX.IsAvailable())

        worldRendTex.display();

        sf::RenderStates rs;
                rs.shader = &dayCycleFX;
                if (ambientLight < 0.3f) ambientLight = 0.3f;
                dayCycleFX.setParameter("light", ambientLight);
                dayCycleFX.setParameter("albedo", sf::Shader::CurrentTexture);

        //App->draw(sf::Sprite(worldRendTex.GetTexture()), rs);
        sf::Sprite spr(worldRendTex.getTexture());
        spr.setPosition(0, 0);

        //std::cerr << spr.getLocalBounds().width << "x" << spr.getLocalBounds().height << std::endl;

        sf::View old_view = App->getView();
        App->setView(App->getDefaultView());
        App->draw(spr, rs);
        App->setView(old_view);

        worldRendTex.clear();
}
*/

/*
void GraphEng::drawFogOfWar() {
        return;
        //if (dayCycleFX.IsAvailable())

        worldRendTex.display();

        GameReg* gameReg = GameReg::getInstance();

        sf::RenderStates rs;
                rs.shader = &fogOfWarFX;

                fogOfWarFX.setParameter("pos_x", gameReg->player->getPosition().x);
                fogOfWarFX.setParameter("pos_y", gameReg->player->getPosition().y);
                fogOfWarFX.setParameter("albedo", sf::Shader::CurrentTexture);

        //App->draw(sf::Sprite(worldRendTex.GetTexture()), rs);
        sf::Sprite spr(worldRendTex.getTexture());
        spr.setPosition(0, 0);

        //std::cerr << spr.getLocalBounds().width << "x" << spr.getLocalBounds().height << std::endl;

        sf::View old_view = App->getView();
        App->setView(App->getDefaultView());
        App->draw(spr, rs);
        App->setView(old_view);

        worldRendTex.clear();
}*/
