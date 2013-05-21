#include "game.h"
#include "utils.h"
#include "input_engine.h"
#include "graphics_engine.h"

#include "scene.h"
#include "game_scene.h"
#include "menu_scene.h"

sf::RenderWindow* App;
sf::Time global_frametime;
sf::Time scene_total_time;

int frameLimit = 30;

bool Game::Init() {

    App = new sf::RenderWindow();

    graphics = GraphEng::getInstance();
    graphics->init();

    scene = new MenuScene();

    frames = 0;
    //    App->setFramerateLimit(frameLimit);

    return true;

}

void Game::Go() {

    sf::Clock clock1;
    sf::Clock clock2;

    float framerate = 0.0f;

    while (App->isOpen() && scene != NULL && scene != EXIT_SCN) {
        if (!scene->Init()) {
            std::cerr << "Scene couldn't init correctly. Game will shut down." << std::endl;
            App->close();
        }

        sf::Clock sceneTimeElapsed;
        clock1.restart();

        while(App->isOpen() && scene->nextScene == NULL)
        {

            sf::Event event;
            while (App->pollEvent(event)) {

                switch(event.type) {
                //Window Closed
                case sf::Event::Closed:
                    App->close();
                    break;
                case sf::Event::Resized:
                {
                    int window_width = event.size.width;
                    int window_height = event.size.height;
                    glViewport(0, 0, window_width, window_height);
                    break;
                }
                default:
                    //Nothing
                    break;
                }
            }

            global_frametime = clock1.getElapsedTime();
            //            global_frametime = sf::microseconds(1000000 / frameLimit); //FIXED framerate.
            scene_total_time = sceneTimeElapsed.getElapsedTime();
            clock1.restart();

            //Framerate
            if (clock2.getElapsedTime().asMilliseconds() > 100) {
                framerate = ((float) frames / (float) clock2.getElapsedTime().asMilliseconds())*1000.0f;
                clock2.restart();
                frames = 0;
            }
            std::stringstream ss(std::stringstream::in | std::stringstream::out);
            framerate*=100;
            framerate= floor(framerate);
            framerate/=100;
            ss << "FPS: " << framerate;

            /*
            sf::Text fpsText; //ss.str());
            fpsText.setPosition(10, 10);
            fpsText.setColor(sf::Color::Red);

            std::stringstream ss2(std::stringstream::in | std::stringstream::out);
            ss2 << static_cast<int>(sceneTimeElapsed.getElapsedTime().asSeconds()/60) << " : " <<
                   (((int)sceneTimeElapsed.getElapsedTime().asSeconds())%60) << " . " <<
                   sceneTimeElapsed.getElapsedTime().asMilliseconds()%1000;
            sf::Text timeText; //ss2.str());
            timeText.setPosition(0.8f * (float) App->getSize().x, 10.0f);
            timeText.setColor(sf::Color::White);
*/
            //Input
            scene->input.Update();
            if (scene->input.getKeyDown(InputEng::EXIT)) scene->nextScene = EXIT_SCN;

            //Logic
            scene->Update();

            //Draw
            App->clear(sf::Color(12, 12, 12));
            App->setActive();
            scene->Draw();

            //            App->draw(fpsText);
            //App->draw(timeText);

            App->display();

            frames++;
        }

        Scene* aux_scn = scene;
        scene = scene->nextScene;
        aux_scn->Destroy();
        delete aux_scn;

        cout << "Scene change. " << endl;
    }
}
