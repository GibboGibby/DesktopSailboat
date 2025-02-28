#include "Renderer.h"

Renderer::Renderer(std::shared_ptr<Window> window)
{
	renderer = SDL_CreateRenderer(window->GetSDLWindow(), "direct3d11");
	if (renderer == nullptr)
	{
		std::cerr << "Failed to create Renderer" << std::endl;
		exit(1);
	}
}

void Renderer::StartRender()
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(renderer);
}

void Renderer::FinishRender()
{
	SDL_RenderPresent(renderer);
}

SDL_Renderer* Renderer::GetSDLRenderer()
{
	return renderer;
}

void Renderer::SetCamera(std::shared_ptr<Camera> _camera)
{
	camera = _camera;
}

int roundUpToMultipleOfEight(int v)
{
	return (v + (8 - 1)) & -8;
}

void Renderer::DrawCircle(const SDL_Point& center, const float radius, const SDL_Color& color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	const int arrSize = roundUpToMultipleOfEight(radius * 8 * 35 / 49);
	std::vector<SDL_FPoint> points;
	points.reserve(arrSize);

	const float diameter = (radius * 2);

	float x = (radius - 1);
	float y = 0;
	float tx = 1;
	float ty = 1;
	float error = (tx - diameter);

	while (x >= y)
	{
		// Each of the following renders an octant of the circle
		points.push_back(SDL_FPoint{ center.x + x, center.y - y });
		points.push_back(SDL_FPoint{ center.x + x, center.y + y });
		points.push_back(SDL_FPoint{ center.x - x, center.y - y });
		points.push_back(SDL_FPoint{ center.x - x, center.y + y });
		points.push_back(SDL_FPoint{ center.x + y, center.y - x });
		points.push_back(SDL_FPoint{ center.x + y, center.y + x });
		points.push_back(SDL_FPoint{ center.x - y, center.y - x });
		points.push_back(SDL_FPoint{ center.x - y, center.y + x });

		if (error <= 0)
		{
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0)
		{
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}

	SDL_RenderPoints(renderer, points.data(), points.size());
}
