
#include "game.h"
#include "game_states.h"
#include "input.h"
#include "graphic.h"

Game::Game() {
    input = Input::pGetInstance();
    graphic = Graphic::pGetInstance();
    audio = Audio::pGetInstance();
    language_manager = LanguageManager::pGetInstance();
}

Game::~Game() {
    graphic->quit();
}

bool Game::init() {
    if (!graphic->init()) {std::cerr << "[ERROR] En graphics\n";return false;}
    if (!input->init()) {std::cerr << "[ERROR] En input\n";return false;}
    m_state = GAMESTATE_GAME;
    input->clear_keys();
    return true;
}

void Game::update() {
    float now = input->get_global_time();
    bool exit = false;
    while (!exit) {
        input->update();
        if (input->exit()) {
            save_state();
            Settings* sett = Settings::pGetInstance();
            sett->restore = true;
            m_state = GAMESTATE_EXIT;
        }

        float interval = now;
        now = input->get_global_time();
        interval = now - interval;

        graphic->clear();

        switch (m_state) {
        case GAMESTATE_GAME:
            m_state = state_splash_primary.update(interval);
                      state_splash_primary.draw();
            break;
        case GAMESTATE_EXIT:
            exit = true;
            break;
        default:
            std::cerr << "No State" << std::endl;
            exit = true;
        }


        //CInputLibrary::waitvsync();
        graphic->flip();
    }
    //EXIT
    Settings* sett = Settings::pGetInstance();
    sett->save("data/settings.sv");
}

