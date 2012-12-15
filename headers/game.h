#ifndef _GAME_H_
#define _GAME_H_

#include "game_states.h"
#include "input.h"
#include "engine.h"
#include "graphic.h"

class Game {
    public:
        Game();
        ~Game();

        bool init();
        void update();

    private:
        LanguageManager* language_manager;

        EGameState m_state;
        Engine state_engine;
};

#endif
