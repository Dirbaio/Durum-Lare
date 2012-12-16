#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

class ParticleEmitter;
class Particle : public sf::Sprite
{
public:
	Particle(const sf::Vector2f &_pos, const sf::Vector2f &_vel, float m_live);
	bool update(float deltaTime, ParticleEmitter &parent);
	void draw(ParticleEmitter &parent);
protected:
	sf::Vector2f m_pos0, m_pos, m_vel0;
	float m_live, m_time;



	friend class ParticleEmitter;
};
