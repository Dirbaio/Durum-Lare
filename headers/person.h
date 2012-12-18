#ifndef PERSON_H
#define PERSON_H

#include "npc.h"
#include "transition.h"
#include "game_reg.h"
#include "utils.h"

#include <SFML/Audio.hpp>
#include <list>

class Person : public Npc {
public:
	Person() {}

	void Init();
	void Update();
	void Draw();

	void doDeath();
	void onHit();
	bool is_alive();

	enum State {
		STATE_WALKING,
		STATE_PANIC,
		STATE_DEAD,
		STATE_CONFUSED
	};

	int getState() { return m_state; }
	bool knowsPlayer();

private:
	float getClosestMenace(sf::Vector2f pos, sf::Vector2f& menacePos);
	void lookAtRandomPlace();

	int ix, iy;
	bool m_knowsPlayer;
	float m_dissappearTime;
	float m_deathTimer;
	float m_panicTime, m_startPanicTime;
	float m_walkingTime;
	float m_confuseCooldown;
	float m_confusedTime;
	float m_confusedTimeFacing;

	sf::Vector2f m_lastSawPlayer;
	State m_state;

	TransitionLinear* m_transHit;
	sf::Sprite m_bloodSpr;
	//sf::SoundBuffer m_dieSoundBuff;
	//sf::Sound m_dieSound;
};

#endif // PERSON_H
