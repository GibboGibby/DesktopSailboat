#pragma once
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl3.h"
#include "ImGUI/imgui_impl_sdlrenderer3.h"
#include "SDL3/SDL_opengl.h"

#include "Window.h"
#include "Renderer.h"
#include <functional>

#define m_assert(expr, msg) assert(((void)(msg), (expr)))

typedef std::function<void()> VoidCallback;

class ParticleSystem;

class GuiRenderer
{
public:
	GuiRenderer(const std::shared_ptr<Window> window, const std::shared_ptr<Renderer> renderer);
	~GuiRenderer();


	void NewFrame();

	void SetCallbacks(VoidCallback spawnParticle);

	void SetParticleSystem(std::shared_ptr<ParticleSystem> ps) { particleSystem = ps; }

	void DrawFrame();

	void Render(const std::shared_ptr<Renderer> renderer);

	const bool MovingWindow() const { return movingWindow; }



public:

	float xOffset = 0;
	float yOffset = 0;

private:
	void ResetToDefaultButton(size_t offset, size_t size);
	template<typename T>
	void ResetToDefaultButton(T* settingsPointer);
private:
	int tabNumber = 0;

	// Callbacks
	VoidCallback SpawnParticle;

	Vector2 startPos;

	bool movingWindow = false;

	std::shared_ptr<Window> window;

	std::shared_ptr<ParticleSystem> particleSystem;
	int buttonId = 0;
};

