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

void Renderer::DrawLine(const Vector2& start, const Vector2& end, const SDL_Color& color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderLine(renderer, start.x, start.y, end.x, end.y);
}

void Renderer::DrawFilledCircle(const SDL_Point& pos, const float radius, const SDL_Color& color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	for (int w = 0; w < radius * 2; w++)
	{
		for (int h = 0; h < radius * 2; h++)
		{
			int dx = radius - w; // horizontal offset
			int dy = radius - h; // vertical offset
			if ((dx * dx + dy * dy) <= (radius * radius))
			{
				SDL_RenderPoint(renderer, pos.x + dx, pos.y +dy);
			}
		}
	}
}