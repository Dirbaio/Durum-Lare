#pragma once

#include <list>
#include <fstream>
#include <SFML/Graphics.hpp>

#include "particle.h"

class ParticleEmitter
{
public:
	ParticleEmitter();
	ParticleEmitter(const ParticleEmitter &oth);

	void update(float deltaTime);
	void draw();
	void restart();

	bool load(std::string path);
	void load();
	void unload();

	// SETTERS

	void setPosition(const sf::Vector2f &pos);
	void setDirection(const sf::Vector2f &dir);
	void setDirection(float dir);
	void setAngle(float angle);

	void setParticleNumber(int num);
	void setFrequency(float freq);
	void setGravity(const sf::Vector2f &grav);

	void setParticleLive(float l);
	void setParticleLive(float l0, float l1);
	void setParticleColor(const sf::Color &c);
	void setParticleColor(const sf::Color &c0, const sf::Color &c1);
	void setParticleSpeed(float s);
	void setParticleSpeed(float s0, float s1);
	void setParticleSize(float z);
	void setParticleSize(float z0, float z1);
	void setParticleAccumulativeColor(bool enable);
	void setParticleMaterial(std::string name);

	//GETTERS

	const sf::Vector2f &position()  { return m_pos; }
	float getDirection() { return m_dir; }
	float getAngle() { return m_angle; }

	int particleCount() { return m_num0;}
	float frequency() { return m_freq; }
	const sf::Vector2f & gravity() { return m_grav; }

	float particleLiveMin() { return m_l0; }
	float particleLiveMax() { return m_l1; }
	const sf::Color &particleColorStart() { return m_c0; }
	const sf::Color &particleColorEnd()   { return m_c1; }
	float particleSpeedMin() { return m_s0; }
	float particleSpeedMax() { return m_s1; }
	float particleSizeStart() { return m_z0; }
	float particleSizeEnd()   { return m_z1; }
	bool particleAccumulativeColor() { return m_accumBlending; }
	const std::string &particleMaterial() { return m_material; }

	bool areParticlesLeft() { return m_num || m_particles.empty();}

	void write(std::ofstream &file);
	void read(std::ifstream &file);

private:
	void newParticle();

	void read (std::ifstream &file, sf::Vector2f &v);
	void write(std::ofstream &file, const sf::Vector2f &v);
	void read (std::ifstream &file, sf::Color &c);
	void write(std::ofstream &file, const sf::Color &c);

protected:

	sf::Vector2f m_pos;
	float m_dir;
	float m_angle;

	int m_num, m_num0;
	float m_freq;
	sf::Vector2f m_grav;

	float m_time;

	float m_l0, m_l1;
	sf::Color  m_c0, m_c1;
	float m_s0, m_s1;
	float m_z0, m_z1;
	bool m_accumBlending;
	std::string m_material;
	sf::Sprite m_sprite;

	friend class Particle;
	std::list<Particle> m_particles;
};
