#ifndef PERSON_H
#define PERSON_H

#include "npc.h"
#include "transition.h"

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

    sf::Sprite deadSpr;
    float m_walkingTime;
    float m_vel;

    TransitionLinear* transHit;

};

#endif // PERSON_H
