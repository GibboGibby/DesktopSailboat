#pragma once
#include "Window.h"
#include "SDL3/SDL.h"
#include <memory>

class Renderer
{

public:
	Renderer(std::shared_ptr<Window> window);

	void StartRender();
	void FinishRender();

	SDL_Renderer* GetSDLRenderer();
private:
	SDL_Renderer* renderer;
};