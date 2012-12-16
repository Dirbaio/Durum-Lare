#include "particle.h"
#include "particleemitter.h"

#include "defines.h"

sf::Color interpolate(sf::Color a, sf::Color b, float weight)
{
	float wa = weight, wb = 1-weight;
	return sf::Color(a.r*wa + b.r*wb, a.g*wa + b.g*wb, a.b*wa + b.b*wb, a.a*wa + b.a*wb);
}

Particle::Particle(const sf::Vector2f &_pos, const sf::Vector2f &_vel, float live):
	m_pos0(_pos), m_pos (_pos), m_vel0(_vel), m_live(live), m_time(0)
{

}

bool Particle::update(float deltaTime, ParticleEmitter &parent)
{
	if(m_live == m_time) return false;

	m_time += deltaTime;
	if (m_live < m_time) m_time = m_live;

	sf::Vector2f acc = parent.m_grav;
	m_pos = m_pos0 + m_vel0*m_time + acc*m_time*m_time*0.5f;

	return true;
}

void Particle::draw(ParticleEmitter &parent)
{
	float weight = (m_live-m_time)/m_live;
	sf::Color current = interpolate(parent.m_c0, parent.m_c1, weight);
	float size = ((parent.m_z0*weight) + (parent.m_z1*(1.0f-weight)))*0.5f;

	sf::Sprite &spr = parent.m_sprite;
	spr.setScale(size, size);
	spr.setPosition(m_pos);
	spr.setColor(current);
	App->draw(spr);
}
