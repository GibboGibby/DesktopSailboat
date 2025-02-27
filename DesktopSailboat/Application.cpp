#include "Application.h"

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


	if (window != nullptr && renderer != nullptr && eventHandler != nullptr)
		return true;
	else
		return false;
}

void Application::Run()
{
	// Game Loop
	while (isRunning)
	{
		eventHandler->HandleEvents(isRunning);
		float mx, my;
		SDL_MouseButtonFlags flags = SDL_GetGlobalMouseState(&mx, &my);
		int wx, wy;
		SDL_GetWindowPosition(window->GetSDLWindow(), &wx, &wy);
		clickInput->UpdateCurrentInput(flags);

		if (clickInput->IsMouseButtonDown(LeftMouseButton))
		{
			std::cout << "Left Mouse Button Down" << std::endl;
		}

		if (clickInput->IsMouseButton(LeftMouseButton))
		{
			std::cout << "Left Mouse Button" << std::endl;
		}

		if (clickInput->IsMouseButtonUp(LeftMouseButton))
		{
			std::cout << "Left Mouse Button Up" << std::endl;
		}

		int sizeX, sizeY;
		SDL_GetWindowSize(window->GetSDLWindow(), &sizeX, &sizeY);

		if (IsPointInsideObject(mx, my, { wx, wy, sizeX, sizeY }))
		{
			SetActiveWindow(window->GetWindowHandle());
		}


		// RENDER
		renderer->StartRender();

		guiRenderer->NewFrame();

		bool randomCheckbox = false;
		int counter = 0;
		float randomValue = 0.0f;

		{

			ImGuiIO& io = ImGui::GetIO();
			ImGui::Begin("Hello World!");
			ImGui::Checkbox("RandomTestCheckbox", &randomCheckbox);

			ImGui::SliderFloat("Float Slider", &randomValue, 0.0f, 10.0f);

			if (ImGui::Button("Button"))
			{
				counter++;
			}
			ImGui::SameLine();
			ImGui::Text("Counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
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
		

		

		guiRenderer->Render(renderer);

		renderer->FinishRender();
		
		clickInput->UpdatePrevInput();
	}
}

void Application::Shutdown()
{
	// Appropriate cleanup. Should mostly be done on the side of the particle system as I am using shared pointers

}
