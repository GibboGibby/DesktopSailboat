#include "Grid.h"
#include "GridSquare.h"

void Grid::InitGrid(Vector2b _gridSize, Vector2 _screenSize)
{
	gridSize = _gridSize;
	screenSize = _screenSize;

	int xLoop = static_cast<int>(ceil(screenSize.x) / static_cast<float>(gridSize.x));
	int yLoop = static_cast<int>(ceil(screenSize.y) / static_cast<float>(gridSize.y));

	for (int x = 0; x < xLoop; x++)
	{
		for (int y = 0; y < yLoop; y++)
		{
			grid.emplace(std::make_pair(static_cast<uint8_t>(x), static_cast<uint8_t>(y)), std::make_shared<GridSquare>());
		}
	}
}

GridSquare* Grid::At(Vector2b position)
{
	// TODO: insert return statement here
	int x = static_cast<int>(ceil(screenSize.x) / static_cast<float>(gridSize.x));
	int y = static_cast<int>(ceil(screenSize.y) / static_cast<float>(gridSize.y));
	if (position.x < 0 || position.x > x || position.y < 0 || position.y > y) return nullptr;
	return grid[std::make_pair(position.x, position.y)].get();
}

GridSquare* Grid::At(uint8_t x, uint8_t y)
{
	// TODO: insert return statement here
	return At(Vector2b{ x,y });
}

GridSquare* Grid::At(int x, int y)
{
	// TODO: insert return statement here
	return At(Vector2b{ static_cast<uint8_t>(x), static_cast<uint8_t>(y) });
}

GridSquare* Grid::AtFromParticleSystemPosition(Vector2 position)
{
	int x = position.x / gridSize.x;
	int y = position.y / gridSize.y;

	return At(Vector2b{ static_cast<uint8_t>(x), static_cast<uint8_t>(y) });
}

std::vector<GridSquare*> Grid::GetGridSquaresAroundPosition(Vector2b pos)
{
	std::vector<GridSquare*> gridSquares;
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			auto t = At((int)pos.x + i, (int)pos.y + i);
			if (t == nullptr) continue;
			gridSquares.push_back(t);
		}
	}
	return gridSquares;
}

Vector2b Grid::WorldToGridPos(Vector2 pos)
{
	Vector2b gridPos;
	gridPos.x = static_cast<uint8_t>(pos.x / gridSize.x);
	gridPos.y = static_cast<uint8_t>(pos.y / gridSize.y);
	return gridPos;
}
	
Vector2b Grid::WorldToGridPos(Vector2d pos)
{
	Vector2b gridPos;
	gridPos.x = static_cast<uint8_t>(pos.x / gridSize.x);
	gridPos.y = static_cast<uint8_t>(pos.y / gridSize.y);
	return gridPos;
}