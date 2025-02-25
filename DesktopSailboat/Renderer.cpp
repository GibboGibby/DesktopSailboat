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
