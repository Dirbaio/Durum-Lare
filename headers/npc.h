#ifndef NPC_H
#define NPC_H

#include "character.h"

class Npc : public Character {
  public:
    Npc() {alive = true;}

    virtual void Init();
    virtual void Update();
    virtual void Draw();

    virtual void onHit() {};

    bool is_alive() {return alive;}
    void set_alive(bool b) {alive = b;}

    void set_life(int l) {life = l;}
    int get_life() {return life;}

  protected:

    sf::Sprite mySpr;
    bool alive;

    int life;

};

#endif // NPC_H
