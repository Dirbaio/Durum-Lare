#ifndef NPC_H
#define NPC_H

#include "object.h"

class Npc : public Object {
  public:
    Npc() {alive = true;}

    void Init();
    void Update();
    void Draw();

    bool is_alive() {return alive;}
    void set_alive(bool b) {alive = b;}

  protected:

    sf::Sprite mySpr;
    bool alive;

};

#endif // NPC_H
