#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"
#include "item.h"
#include <set>

#include <input_engine.h>

class Player : public Character {
public:
	Player() {}

	void Init();
	void Update();
	void Draw();

    void LoadAnims();
    void hitAction();

    int getMoney(){return myMoney;}
    void setMoney(int sc) {myMoney = sc;}

    int getKills() {return myKills;}
    void setKills(int k) {myKills = k;}

    bool isDoingAction() { return m_actionDelay > 0; }

    void onBuy(Item item);

    const std::set<int>& getSpecialItems() {return m_specialItems;}

    void updateInput(InputEng input);

private:
	sf::Sprite mySpr;
	sf::Vector2f myVel;

    std::string currentAnim;
	float m_actionDelay;

    int myMoney;
    int myKills;

    std::set<int> m_specialItems;

    InputEng myInputEng;
};

#endif // PLAYER_H
