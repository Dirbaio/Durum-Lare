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


    enum State {
	STATE_WALKING,
	STATE_PANIC,
	STATE_DEAD
    };

    int getState() { return m_state; }
    bool knowsPlayer();

  private:
    int ix, iy;

    float DISSAPPEAR_TIME;

    float deathTimer;

    sf::SoundBuffer dieSoundBuff;
    sf::Sound dieSound;

    float m_panicTime, m_startPanicTime;
	vec2 m_lastSawPlayer;
    State m_state;

    sf::Sprite bloodSpr;
    float m_walkingTime;

    bool knows_player;
    TransitionLinear* transHit;

};

#endif // PERSON_H
