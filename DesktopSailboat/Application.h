#pragma once
#include "Window.h"
#include "Renderer.h"
#include "EventHandler.h"
#include "ClickInput.h"
#include "GuiRenderer.h"
#include "Camera.h"
#include "ParticleSystem.h"
#include <memory>
#include "GTimer.h"

const int SCREEN_FPS = 60.0f;
const int SCREEN_TICKS_PER_FRAME = 1000.0f / SCREEN_FPS;

class Application
{
public:
	Application();
	~Application();

	bool Init();
	void Run();
	void Shutdown();
public:


private:
	bool IsPointInsideObject(int x, int y, SDL_Rect object);
private:
	bool isRunning;
	std::shared_ptr<Window> window;
	std::shared_ptr<Renderer> renderer;
	std::shared_ptr<GuiRenderer> guiRenderer;
	std::shared_ptr<EventHandler> eventHandler;
	std::shared_ptr<ClickInput> clickInput;

	std::shared_ptr<Camera> camera;
	std::shared_ptr<ParticleSystem> particleSystem;

	bool showSettings = false;

};