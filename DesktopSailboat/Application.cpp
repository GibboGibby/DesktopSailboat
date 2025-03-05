#include "Application.h"
#include "Settings.h"

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
	GTimer fpsTimer;
	GTimer capTimer;
	fpsTimer.Start();

	float deltaTime = 0.0f;
	float testPos1 = 0.0f;
	testPos1 = window->GetWindowSize().x / 2.0f;
	float testPos2 = 0.0f;

	bool f1Check = false;
	int tabNumber = 0;
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
			//std::cout << "Left Mouse Button Down" << std::endl;
		}

		if (clickInput->IsMouseButton(LeftMouseButton))
		{
			//std::cout << "Left Mouse Button" << std::endl;
		}

		if (clickInput->IsMouseButtonUp(LeftMouseButton))
		{
			//std::cout << "Left Mouse Button Up" << std::endl;
		}

		int sizeX, sizeY;
		SDL_GetWindowSize(window->GetSDLWindow(), &sizeX, &sizeY);

		if (IsPointInsideObject(mx, my, { wx, wy, sizeX, sizeY }))
		{
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

		//ImGui::SetNextWindowPos(ImVec2(wx, wy), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		
		// RENDER
		renderer->StartRender();

		guiRenderer->NewFrame();

		bool randomCheckbox = false;
		int counter = 0;
		float randomValue = 0.0f;

		if (showSettings)
		{
			

			ImGuiIO& io = ImGui::GetIO();
			//ImGui::Begin("Desktop Sailboat Settings", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
			ImGui::Begin("Desktop Sailboat Settings", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);
			ImGui::SetWindowPos(ImVec2(0, 0), 0);

			if (ImGui::Button("Test Stuff"))
			{
				tabNumber = 0;
			}
			ImGui::SameLine();
			if (ImGui::Button("Sim Settings"))
			{
				tabNumber = 1;
			}
			ImGui::NewLine();

			switch (tabNumber)
			{
				case 0:
					ImGui::Checkbox("RandomTestCheckbox", &randomCheckbox);

					ImGui::SliderFloat("Float Slider", &randomValue, 0.0f, 10.0f);

					if (ImGui::Button("Button"))
					{
						counter++;
					}
					ImGui::SameLine();
					ImGui::Text("Counter = %d", counter);

					ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
					break;
				case 1:
					if (ImGui::SliderInt("Particle Count", &g_Settings.particleCount, 1, 100000))
					{
						// Done every frame this is changed. Could do a system to callback once it returns false after returning true
						// This would indicate that the value has been changed but has also stopped being changed (for the particle count)
						// As changing every frame might be a bad idea
						DesktopSailboat::SaveSettings();
					}
					ImGui::SliderFloat("Viscosity", &g_Settings.viscosity, 0.0f, 10.0f);
					ImGui::Checkbox("Limit FPS to 60", &g_Settings.limitFPS);
					if (ImGui::Button("Reset to Default"))
					{
						DesktopSailboat::ResetSettings();
						DesktopSailboat::SaveSettings();
					}


			}
			

			



			ImGui::End();
		}

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
					window->PassClickThrough(mx, my);
				}
				//std::cout << "Not Clicking Object" << std::endl;
				//PassClickThrough(mx, my);
			}

		}
		
		float localX, localY;
		SDL_GetMouseState(&localX, &localY);

		testPos2 += 100.0f * deltaTime;

		if (testPos2 > window->GetWindowSize().y)
		{
			testPos2 = 0.0f;
		}

		renderer->DrawCircle(SDL_Point{ (int)testPos1,(int)testPos2 }, 50, SDL_Color{0, 0, 255, 255});
		

		guiRenderer->Render(renderer);

		renderer->FinishRender();
		
		clickInput->UpdatePrevInput();

		int frameTicks = capTimer.GetTicks();
		if (frameTicks < SCREEN_TICKS_PER_FRAME && g_Settings.limitFPS)
		{
			//Wait remaining time
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
		deltaTime = (float)capTimer.GetTicks() / 1000.0f;
	}
}

void Application::Shutdown()
{
	// Appropriate cleanup. Should mostly be done on the side of the particle system as I am using shared pointers

}
