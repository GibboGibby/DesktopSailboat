#include "Application.h"
#include "Settings.h"
#include "ParticleSystem.h"

bool Application::IsPointInsideObject(int x, int y, SDL_Rect object)
{
	return (x >= object.x && x <= object.x + object.w &&
		y >= object.y && y <= object.y + object.h);
}


Application::Application() : window(nullptr), renderer(nullptr), eventHandler(nullptr), guiRenderer(nullptr), clickInput(nullptr), isRunning(true)
{
}

Application::~Application()
{
}

bool Application::Init()
{
	window = std::make_shared<Window>();
	renderer = std::make_shared<Renderer>(window);
	guiRenderer = std::make_shared<GuiRenderer>(window, renderer);
	eventHandler = std::make_shared<EventHandler>(window);
	clickInput = std::make_shared<ClickInput>();

	camera = std::make_shared<Camera>();
	particleSystem = std::make_shared<ParticleSystem>();

	renderer->SetCamera(camera);

	DesktopSailboat::LoadSettings();

	if (window != nullptr && renderer != nullptr && eventHandler != nullptr)
		return true;
	else
		return false;
}

void Application::Run()
{
	std::shared_ptr<ParticleSystem> particleSystem = std::make_shared<ParticleSystem>(renderer, window);
	guiRenderer->SetParticleSystem(particleSystem);
	//ParticleSystem* particleSystem = new ParticleSystem(renderer, window);
	GTimer fpsTimer;
	GTimer capTimer;
	fpsTimer.Start();

	float frameAverage = 0.0f;
	int countedFrames = 0;
	bool countingFrames = false;

	float deltaTime = 0.0f;

	bool f1Check = false;
	
	int sizeX, sizeY;
	SDL_GetWindowSize(window->GetSDLWindow(), &sizeX, &sizeY);
	particleSystem->Position({ sizeX / 2.0f, sizeY / 2.0f });
	particleSystem->ResetBox();
	particleSystem->Start();

	auto callback = std::bind(&ParticleSystem::SpawnParticle, particleSystem.get());
	guiRenderer->SetCallbacks(callback);
	// Game Loop
	while (isRunning)
	{
		capTimer.Start();
		eventHandler->HandleEvents(isRunning);
		float mx, my;
		SDL_MouseButtonFlags flags = SDL_GetGlobalMouseState(&mx, &my);
		int wx, wy;
		SDL_GetWindowPosition(window->GetSDLWindow(), &wx, &wy);
		clickInput->UpdateCurrentInput(flags);


		if (clickInput->IsMouseButtonDown(LeftMouseButton))
		{


			SDL_Rect temp_rect{ 1000,600,100,100 };
			SetActiveWindow(window->GetWindowHandle());
			if (IsPointInsideObject(mx, my, temp_rect))
			{
				//std::cout << "Clicking object" << std::endl;
				//moving = true;
				//xOffset = temp_rect.x - mx;
				//yOffset = temp_rect.y - my;
			}
			else
			{
				int sizeX, sizeY;
				SDL_GetWindowSize(window->GetSDLWindow(), &sizeX, &sizeY);
				SDL_Rect screenRect{ wx, wy, sizeX, sizeY };
				if (IsPointInsideObject(mx, my, screenRect))
				{
					SetActiveWindow(window->GetWindowHandle());
					//SetForegroundWindow(window->GetWindowHandle());
					window->EnableClickThrough();
					//window->PassClickThrough(mx, my);
				}
				//std::cout << "Not Clicking Object" << std::endl;
				//PassClickThrough(mx, my);
			}

		}

		if (clickInput->IsMouseButtonUp(LeftMouseButton))
		{
			//std::cout << "Mouse Button Up" << std::endl;
			int sizeX, sizeY;
			SDL_GetWindowSize(window->GetSDLWindow(), &sizeX, &sizeY);
			SDL_Rect screenRect{ wx, wy, sizeX, sizeY };
			if (IsPointInsideObject(mx, my, screenRect))
			{
				SetActiveWindow(window->GetWindowHandle());
				//SetForegroundWindow(window->GetWindowHandle());
				window->DisableClickThrough();
			}
		}


		int sizeX, sizeY;
		SDL_GetWindowSize(window->GetSDLWindow(), &sizeX, &sizeY);
		 
		if (IsPointInsideObject(mx, my, { wx, wy, sizeX, sizeY }))
		{
			//std::cout << "Setting active window" << std::endl;
			//SetActiveWindow(window->GetWindowHandle());
			//SetForegroundWindow(window->GetWindowHandle());
			SetActiveWindow(window->GetWindowHandle());
		}
		
		const bool* state = SDL_GetKeyboardState(nullptr);
		

		if (state[SDL_SCANCODE_F1] && !f1Check)
		{
			showSettings = !showSettings;
			f1Check = true;
		}

		if (!state[SDL_SCANCODE_F1])
		{
			f1Check = false;
		}

		camera->Update();

		particleSystem->Update();

		//ImGui::SetNextWindowPos(ImVec2(wx, wy), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		
		// RENDER
		renderer->StartRender();

		guiRenderer->NewFrame();

		particleSystem->Render();

		bool randomCheckbox = false;
		int counter = 0;
		float randomValue = 0.0f;

		if (guiRenderer->MovingWindow())
		{
			Vector2 winSize = window->GetWindowSize();
			SDL_Rect rect = { mx + guiRenderer->xOffset, my + guiRenderer->yOffset, winSize.x, winSize.y };
			SDL_SetWindowPosition(window->GetSDLWindow(), rect.x, rect.y);
		}

		if (state[SDL_SCANCODE_P])
		{
			ImGuiIO& io = ImGui::GetIO();
			//std::cout << "Framerate for " << particleSystem->GetNumberOfParticles() << " particles " << " is " << io.Framerate << " fps" << std::endl;
			countedFrames++;
			frameAverage += io.Framerate;
			if (!countingFrames)
				countingFrames = true;
		}

		if (!state[SDL_SCANCODE_P])
		{
			if (countingFrames)
			{
				countingFrames = false;
				std::cout << "Frame Average over " << countedFrames << " frames is " << frameAverage / countedFrames << " fps" << std::endl;
			}
		}

		// TODO: Move this to a function in the GUI Renderer or just somewhere else
		if (showSettings)
		{
			
			guiRenderer->DrawFrame();
			
		}

	
		
		float localX, localY;
		SDL_GetMouseState(&localX, &localY);

		//renderer->DrawCircle(SDL_Point{ (int)testPos1,(int)testPos2 }, 50, SDL_Color{0, 0, 255, 255});
		

		guiRenderer->Render(renderer);

		renderer->FinishRender();
		
		clickInput->UpdatePrevInput();

		int frameTicks = capTimer.GetTicks();
		float screenTicksPerFrame = 1000.0f / g_Settings.app.fpsLimit;
		if (frameTicks < screenTicksPerFrame && g_Settings.app.limitFPS)
		{
			//Wait remaining time
			SDL_Delay(screenTicksPerFrame - frameTicks);
		}
		deltaTime = (float)capTimer.GetTicks() / 1000.0f;
	}
}

void Application::Shutdown()
{
	// Appropriate cleanup. Should mostly be done on the side of the particle system as I am using shared pointers
	// Nevermind the particle system is also using a shared_ptr

}
