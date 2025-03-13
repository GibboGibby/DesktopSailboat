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
	std::unique_ptr<ParticleSystem> particleSystem = std::make_unique<ParticleSystem>(renderer, window);
	//ParticleSystem* particleSystem = new ParticleSystem(renderer, window);
	GTimer fpsTimer;
	GTimer capTimer;
	fpsTimer.Start();

	float frameAverage = 0.0f;
	int countedFrames = 0;
	bool countingFrames = false;

	float deltaTime = 0.0f;
	float testPos1 = 0.0f;
	testPos1 = window->GetWindowSize().x / 2.0f;
	float testPos2 = 0.0f;

	float xOffset = 0.0f;
	float yOffset = 0.0f;

	Vector2 startPos(0.0f, 0.0f);

	bool movingWindow = false;

	bool f1Check = false;
	int tabNumber = 0;
	int sizeX, sizeY;
	SDL_GetWindowSize(window->GetSDLWindow(), &sizeX, &sizeY);
	particleSystem->Position({ sizeX / 2.0f, sizeY / 2.0f });
	particleSystem->ResetBox();
	particleSystem->Start();
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
			//std::cout << "Setting active window" << std::endl;
			//SetActiveWindow(window->GetWindowHandle());
			SetForegroundWindow(window->GetWindowHandle());
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

		if (state[SDL_SCANCODE_LCTRL])
		{
			//std::cout << "Control is pressed!!!!!!!" << std::endl;
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

		if (movingWindow)
		{
			Vector2 winSize = window->GetWindowSize();
			SDL_Rect rect = { mx + xOffset, my + yOffset, winSize.x, winSize.y };
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
			ImGui::SameLine();
			if (ImGui::Button("SPH Stuff"))
			{
				tabNumber = 2;
			}
			ImGui::NewLine();

			switch (tabNumber)
			{
				case 0:
					ImGui::Checkbox("RandomTestCheckbox", &randomCheckbox);

					ImGui::SliderFloat("Float Slider", &randomValue, 0.0f, 10.0f);

					ImGui::Button("Move Window");
					if (ImGui::IsItemActive())
					{
						if (!movingWindow)
						{
							movingWindow = true;
							int wx, wy;
							SDL_GetWindowPosition(window->GetSDLWindow(), &wx, &wy);
							float mx, my;
							SDL_GetGlobalMouseState(&mx, &my);

							xOffset = wx - mx;
							yOffset = wy - my;
							startPos = Vector2(wx, wy);
						}
					}
					else
					{
						if (movingWindow)
						{
							movingWindow = false;
							int wx, wy;
							SDL_GetWindowPosition(window->GetSDLWindow(), &wx, &wy);
							Vector2 movedAmt(wx - startPos.x, my - startPos.y);
							std::cout << "Moved amt = " << movedAmt.x << ", " << movedAmt.y << std::endl;
							particleSystem->AddForceToAllParticles(movedAmt* 3.0f);
						}
					}

					if (ImGui::Button("Button"))
					{
						counter++;
					}
					ImGui::SameLine();
					ImGui::Text("Counter = %d", counter);

					ImGui::Text("Number of Particles = %lli", particleSystem->GetNumberOfParticles());

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
					ImGui::Checkbox("Limit FPS: ", &g_Settings.limitFPS);
					ImGui::SameLine();
					if (ImGui::InputInt("##FpsLimit", &g_Settings.fpsLimit))
					{
						if (g_Settings.fpsLimit < 15) g_Settings.fpsLimit = 15;
					}
					


					ImGui::Checkbox("Limit Simulation FPS: ", &g_Settings.limitSimFPS);
					ImGui::SameLine();
					ImGui::InputInt("##simFpsLimit", &g_Settings.simFpsLimit);
					if (ImGui::Button("Reset to Default"))
					{
						DesktopSailboat::ResetSettings();
						DesktopSailboat::SaveSettings();
					}

					if (ImGui::IsAnyItemActive)
					{
						DesktopSailboat::SaveSettings();
					}

					if (ImGui::Button("Spawn Cricle at Center"))
					{
						int sizeX, sizeY;
						SDL_GetWindowSize(window->GetSDLWindow(), &sizeX, &sizeY);
						particleSystem->SpawnCircle(sizeX / 2, sizeY / 2, 20);
					}


					if (ImGui::InputFloat("Box Width", &g_Settings.boxWidth) || ImGui::InputFloat("Box Height", &g_Settings.boxHeight))
					{
						particleSystem->ResetBox();
					}
					break;

				case 2:
					if (ImGui::Button("Spawn Another Particle"))
					{
						particleSystem->SpawnParticle();
					}
					if (ImGui::Button("Reset SPH"))
					{
						particleSystem->ResetParticles();
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
					SetForegroundWindow(window->GetWindowHandle());
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

		//renderer->DrawCircle(SDL_Point{ (int)testPos1,(int)testPos2 }, 50, SDL_Color{0, 0, 255, 255});
		

		guiRenderer->Render(renderer);

		renderer->FinishRender();
		
		clickInput->UpdatePrevInput();

		int frameTicks = capTimer.GetTicks();
		float screenTicksPerFrame = 1000.0f / g_Settings.fpsLimit;
		if (frameTicks < screenTicksPerFrame && g_Settings.limitFPS)
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
