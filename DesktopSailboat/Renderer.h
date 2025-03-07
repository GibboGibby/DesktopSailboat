#pragma once
#include "Window.h"
#include "Camera.h"

#include "SDL3/SDL.h"
#include <memory>
#include<vector>

class Renderer
{

public:
	Renderer(std::shared_ptr<Window> window);

	void StartRender();
	void FinishRender();

	SDL_Renderer* GetSDLRenderer();

	void SetCamera(std::shared_ptr<Camera> camera);

	void DrawCircle(const SDL_Point& pos, const float radius, const SDL_Color& color);
	void DrawLine(const Vector2& start, const Vector2& end, const SDL_Color& color);
private:
	SDL_Renderer* renderer;
	std::shared_ptr<Camera> camera;
};