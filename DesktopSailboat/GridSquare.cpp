#include "GridSquare.h"

GridSquare::GridSquare()
{

}

std::map<short, std::weak_ptr<Particle>>& GridSquare::GetMap()
{
	return particles;
}

std::weak_ptr<Particle> GridSquare::GetParticle(short id)
{
	return particles[id];
}

bool GridSquare::ContainsParticle(short id)
{
	return particles.count(id);
}

void GridSquare::RemoveParticle(short id)
{
	particles.erase(id);
}

void GridSquare::AddParticle(short id, std::weak_ptr<Particle> particle)
{
	particles.emplace(id, particle);
}

void GridSquare::AddParticle(std::weak_ptr<Particle> particle)
{
	particles.emplace(particle.lock()->id, particle);
}
