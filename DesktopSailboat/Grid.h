#pragma once
#include <map>
#include <memory>
#include <vector>
#include "Maths.h"

class GridSquare;

class Grid
{
public:
	void InitGrid(Vector2b _gridSize, Vector2 _screenSize);
	GridSquare* At(Vector2b position);
	GridSquare* At(uint8_t x, uint8_t y);
	GridSquare* At(int x, int y);

	GridSquare* AtFromParticleSystemPosition(Vector2 position);

	std::vector<GridSquare*> GetGridSquaresAroundPosition(Vector2b pos);

	Vector2b WorldToGridPos(Vector2 pos);
	Vector2b WorldToGridPos(Vector2d pos);

private:
	std::map<std::pair<uint8_t, uint8_t>, std::shared_ptr<GridSquare>> grid;
	Vector2b gridSize;
	Vector2 screenSize;
};