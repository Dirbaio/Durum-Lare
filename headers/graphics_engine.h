#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <map>
#include <string>
#include <queue>

class GraphEng
{
public:
                //First-to-last draw
                enum GraphLayer {
                        //Global-position layers
                        GRAPH_LAYER_TILE,
                        GRAPH_LAYER_FLOOR,
                        GRAPH_LAYER_STANDARD,
                        GRAPH_LAYER_OVERSTD,
                        GRAPH_LAYER_DMGNUM,

                        //World / Window separator
                        GRAPH_LAYER_WWSEP,

                        //Window-position layers
                        GRAPH_LAYER_BACKHUD,
                        GRAPH_LAYER_HUD,
                        GRAPH_LAYER_ITEMHUD,
                        GRAPH_LAYER_OVERHUD,
                        GRAPH_LAYER_BACKCURSORHUD,
                        GRAPH_LAYER_CURSORHUD,
                        GRAPH_LAYER_POPUP
                };

        GraphEng();
        static GraphEng* getInstance();

        sf::Texture* getTexture(std::string path);

        void init();

                sf::VideoMode getCurrentVideoMode();

                void DrawAll();

                void Draw(sf::Sprite* drawable);
                void Draw(sf::Sprite* drawable, int layer);

                void Draw(sf::Text text);
                void Draw(sf::Text text, int layer);

                void setAmbientLight(float f) {ambientLight = f;}

private:
                void instaDraw(sf::Sprite* spr);

                void drawDayEffect();
                void drawFogOfWar();

                sf::RenderTexture worldRendTex;

                sf::RenderTexture whiteTex; //To delete

                sf::Shader dayCycleFX;
                sf::Shader fogOfWarFX;
                float ambientLight;

        static void deleteInstance();

        static GraphEng* m_pInstance;
        static bool m_bInstanced;

                std::vector<sf::VideoMode> m_videoModes;
                int m_currentVideoMode;

        std::map<std::string, sf::Texture*> imgMap;

                std::map<int, std::queue<sf::Sprite*> > drawQueue;
                std::map<int, std::queue<sf::Text> > textQueue;

                sf::Font myFont;

};

#endif // GRAPHICSENGINE_H
