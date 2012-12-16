#ifndef PERSON_H
#define PERSON_H

#include "npc.h"
#include "transition.h"
#include "game_reg.h"
#include "utils.h"

#include <SFML/Audio.hpp>
#include <list>

class Person : public Npc {
  public:
    Person() {}

    void Init();
    void Update();
    void Draw();

    void doDeath();
    void onHit();

    bool is_alive();

  private:

    float DISSAPPEAR_TIME;

    float deathTimer;

    sf::SoundBuffer dieSoundBuff;
    sf::Sound dieSound;

    float m_panicTime, m_startPanicTime;

    enum State {
        STATE_WALKING,
        STATE_PANIC,
        STATE_DEAD
    };

    State m_state;

    sf::Sprite deadSpr;
    float m_walkingTime;


    TransitionLinear* transHit;


};

#endif // PERSON_H
