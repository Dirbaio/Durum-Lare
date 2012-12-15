#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"

class Player : public Object {
public:
    Player() {}

    void Init();
    void Update();
    void Draw();


private:
    sf::Sprite mySpr;


};

#endif // PLAYER_H
