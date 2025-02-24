#pragma once
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl3.h"
#include "ImGUI/imgui_impl_sdlrenderer3.h"
#include "SDL3/SDL_opengl.h"

#include "Window.h"
#include "Renderer.h"

class GuiRenderer
{
	void Init(const std::shared_ptr<Window> window, const std::shared_ptr<Renderer> renderer);

	void NewFrame();
	void Render(const std::shared_ptr<Renderer> renderer);
};