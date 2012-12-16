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

    enum State  {
	    STATE_PATROL_MOVING,
	    STATE_PATROL_WATCHING,
	    STATE_ALERT,
	    STATE_CONFUSE,
	    STATE_CHASING_PLAYER
    };

    State m_state;
    float m_watchingTime;
    float m_watchingTimeFacing;
};

#endif // POLICE_H
