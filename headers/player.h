#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"
#include "item.h"
#include <set>

#include <input_engine.h>

class Player : public Character {
public:
    Player(GameScene* sc) : Character(sc) {}

	void Init();
	void Update();
	void Draw();

    void LoadAnims();
    void hitAction();

    void gotCaught();

    int getMoney(){return myMoney;}
    void setMoney(int sc) {myMoney = sc;}

    int getKills() {return myKills;}
    void setKills(int k) {myKills = k;}

    bool isDoingAction() { return m_actionDelay > 0; }

    void onBuy(Item item);

    const std::set<int>& getSpecialItems() {return m_specialItems;}

    InputEng playerInput;

    bool m_jailed;

	sf::Sprite mySpr;
	sf::Vector2f myVel;

    std::string currentAnim;
	float m_actionDelay;

    int myMoney;
    int myKills;

    std::set<int> m_specialItems;

    InputEng myInputEng;
    sf::Sprite m_jailSpr;

    float m_jailedTime;
};

#endif // PLAYER_H
