#ifndef PERSON_H
#define PERSON_H

#include "npc.h"

class Person : public Npc {
  public:
    Person() {}

    void Init();
    void Update();

  private:


};

#endif // PERSON_H
