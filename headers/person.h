#ifndef PERSON_H
#define PERSON_H

#include "npc.h"

#define DISSAPPEAR_TIME 2.0f

class Person : public Npc {
  public:
    Person() {}

    void Init();
    void Update();

    void Draw();

    void doDeath();

  private:


    float deathTimer;

    sf::Sprite deadSpr;
};

#endif // PERSON_H
