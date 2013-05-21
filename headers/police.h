#ifndef POLICE_H
#define POLICE_H

#include "npc.h"

class Police : public Npc {
public:
    Police(GameScene* sc) : Npc(sc) {}

    void Init();
    void Update();

private:

    sf::Vector2f getNewGoal(sf::Vector2f pos);
    void lookAtRandomPlace();

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
    float m_alertTime;
    sf::Vector2f m_lastAlertPos;

    vector<float> m_lastPosSawTime;
    vector<sf::Vector2f> m_lastPosSawPlayer;
    vector<sf::Vector2f> m_lastDirSawPlayer;
    vector<bool> m_knowPlayer;
    int m_chasingPlayerNum;
    bool m_collided;
};

#endif // POLICE_H
