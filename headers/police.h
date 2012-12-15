#ifndef POLICE_H
#define POLICE_H

#include "object.h"

class Police : public Object {
  public:
    Police() {}

    void Init();
    void Update();
    void Draw();

  private:

    sf::Sprite mySpr;

};

#endif // POLICE_H
