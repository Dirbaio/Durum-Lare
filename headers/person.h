#ifndef PERSON_H
#define PERSON_H

#include "npc.h"

class Person : public Npc {
  public:
    Person() {}

    void Init();
    void Update();

    void Draw();

    void doDeath();

  private:

    float DISSAPPEAR_TIME;

    float deathTimer;

    sf::Sprite deadSpr;
    float m_walkingTime;
    float m_vel;
};

#endif // PERSON_H
