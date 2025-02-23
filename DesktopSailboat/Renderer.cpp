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

SDL_Renderer* Renderer::GetSDLRenderer()
{
	return renderer;
}
