#ifndef PERSON_H
#define PERSON_H

#include "npc.h"
#include "transition.h"

#include <SFML/Audio.hpp>

class Person : public Npc {
  public:
    Person() {}

    void Init();
    void Update();

    void Draw();

    void doDeath();

    void onHit();

  private:

    float DISSAPPEAR_TIME;

    float deathTimer;

    sf::SoundBuffer dieSoundBuff;
    sf::Sound dieSound;

    sf::Sprite deadSpr;
    float m_walkingTime;

    TransitionLinear* transHit;


};

#endif // PERSON_H
