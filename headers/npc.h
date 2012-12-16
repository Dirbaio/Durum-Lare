#ifndef NPC_H
#define NPC_H

#include "character.h"

class Npc : public Character {
  public:
    Npc() {}
    virtual ~Npc() {}

    virtual void Init();
    virtual void Update();
    virtual void Draw();

    virtual void onHit() {}

    virtual bool is_alive() {return true;}
    void set_life(int l) {life = l;}
    int get_life() {return life;}

  protected:

    sf::Sprite mySpr;
    int life;

};

#endif // NPC_H
