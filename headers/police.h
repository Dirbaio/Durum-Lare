#ifndef POLICE_H
#define POLICE_H

#include "npc.h"

class Police : public Npc {
  public:
    Police() {}


    void Init();
    void Update();
    void Draw();

  private:

    sf::Vector2f getNewGoal();
    void lookAtRandomPlace();

    bool onCollision(int x, int j);
    bool onLeftCollision(int x, int j);
    bool onRightCollision(int x, int j);
    bool onUpCollision(int x, int j);
    bool onDownCollision(int x, int j);

    enum State  {
	    STATE_PATROL_MOVING,
	    STATE_PATROL_WATCHING,
	    STATE_ALERT,
	    STATE_CONFUSE,
	    STATE_CHASING_PLAYER,
	    STATE_PLAYER_LOST
    };

    State m_state;
    float m_watchingTime;
    float m_watchingTimeFacing;
    sf::Vector2f m_lastPosSawPlayer;
    sf::Vector2f m_lastDirSawPlayer;
};

#endif // POLICE_H
