#pragma once
#include <map>
#include <memory>
#include "Particle.h"

class GridSquare
{
private:
	std::map<short, std::weak_ptr<Particle>> particles;

public:
	GridSquare();
	std::map<short, std::weak_ptr<Particle>>& GetMap();

	std::weak_ptr<Particle> GetParticle(short id);

	bool ContainsParticle(short id);

	void RemoveParticle(short id);
	void AddParticle(short id, std::weak_ptr<Particle> particle);
	void AddParticle(std::weak_ptr<Particle> particle);
};