#include <iostream>
#include <SDL3/SDL.h>
#include <string>
#include <thread>
#include <Windows.h>

#include "Maths.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl3.h"
#include "ImGUI/imgui_impl_sdlrenderer3.h"
#include "SDL3/SDL_opengl.h"


void RenderThread(SDL_Window* window, SDL_Renderer* renderer, SDL_Rect* rect);

bool IsPointInsideObject(int x, int y, SDL_Rect object)
{
	return (x >= object.x && x <= object.x + object.w &&
		y >= object.y && y <= object.y + object.h);
}

HWND GetSDLWindowHandle()
{
	return FindWindowA(NULL, "EpicTest");
}



void EnableClickThrough(HWND hwnd) {
	LONG_PTR exStyle = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);
	SetWindowLongPtrW(hwnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED | WS_EX_TRANSPARENT);
	SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_COLORKEY);
}

// Disable click-through mode
void DisableClickThrough(HWND hwnd) {
	LONG_PTR exStyle = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);
	SetWindowLongPtrW(hwnd, GWL_EXSTYLE, exStyle & ~WS_EX_TRANSPARENT);
}

void PassClickThrough(int x, int y) {
	HWND hwnd = GetSDLWindowHandle();
	EnableClickThrough(hwnd);  // Temporarily allow clicks to pass through
	//mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, x, y, 0, 0);
	DisableClickThrough(hwnd); // Restore normal click handling
}

int main()
{

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, SDL_GetError());
	}

	SDL_Window* window;
	SDL_Renderer* renderer;

	//SDL_Window* window = SDL_CreateWindow("EpicTest", 200, 200, SDL_WINDOW_TRANSPARENT | SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP);
	SDL_CreateWindowAndRenderer("EpicTest", 500, 500, SDL_WINDOW_TRANSPARENT | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_BORDERLESS, &window, &renderer);

	if (!window)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, SDL_GetError());
	}

	SDL_ShowWindow(window);

	bool running = true;
	SDL_Surface* screenSurface = SDL_GetWindowSurface(window);


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);

	SDL_Rect rect = { 1000,600,100,100 };

	SDL_SetWindowPosition(window, rect.x - rect.w, rect.y - rect.h);

	SDL_Rect* pRect = &rect;
	std::thread renderThread(RenderThread, window, renderer, pRect);
	renderThread.detach();

	bool moving = false;
	int xOffset = 0;
	int yOffset = 0;

	float randomValue = 0.0f;
	uint8_t counter = 0;
	bool randomCheckbox = false;

	while (running)
	{

		bool p_pressed = false;

		SDL_Event event;
		SDL_PollEvent(&event);

		ImGui_ImplSDL3_ProcessEvent(&event);

		if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
			running = false;

		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		
		

		{
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



		if (event.type == SDL_EVENT_KEY_DOWN)
		{
			if (event.key.key == SDLK_ESCAPE)
			{
				running = false;
			}

			if (event.key.key == SDLK_P)
			{
				p_pressed = true;
			}
		}	

		
		//SDL_Rect temp_rect = { 1000, 600, 150, 150 };
		SDL_Rect temp_rect = { rect.x, rect.y, rect.w, rect.h };
		

		float mx, my;
		SDL_MouseButtonFlags flags = SDL_GetGlobalMouseState(&mx, &my);
		//std::cout << mx << " | " << my << std::endl;

		int wx, wy;
		SDL_GetWindowPosition(window, &wx, &wy);

		//temp_rect.x += mx;
		//temp_rect.y += my;

		HWND hwnd = GetSDLWindowHandle();
		if (IsPointInsideObject(mx, my, temp_rect))
		{
			//DisableClickThrough(hwnd);
			SetActiveWindow(hwnd);
			//if (p_pressed)
			//std::cout << "Disabling Click Through" << std::endl;
		}
		else
		{
			//EnableClickThrough(hwnd);
		}

		if (moving)
		{
			rect.x = mx + xOffset;
			rect.y = my + yOffset;
			SDL_SetWindowPosition(window, rect.x - rect.w, rect.y - rect.h);
		}
		
		

		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || (flags & SDL_BUTTON_LMASK && !moving))
		{

			//float mx, my;
			//SDL_GetGlobalMouseState(&mx, &my);
			SetActiveWindow(hwnd);
			
			if (IsPointInsideObject(mx, my, temp_rect))
			{
				//std::cout << "Clicking object" << std::endl;
				moving = true;
				xOffset = temp_rect.x - mx;
				yOffset = temp_rect.y - my;
			}
			else
			{
				int sizeX, sizeY;
				SDL_GetWindowSize(window, &sizeX, &sizeY);
				SDL_Rect screenRect{ wx, wy, sizeX, sizeY };
				if (IsPointInsideObject(mx, my, screenRect))
				{
					PassClickThrough(mx, my);
				}
				//std::cout << "Not Clicking Object" << std::endl;
				//PassClickThrough(mx, my);
			}
		}

		if (event.type == SDL_EVENT_MOUSE_BUTTON_UP || !(flags & SDL_BUTTON_LMASK))
		{
			if (moving)
			{
				moving = false;
				xOffset = 0;
				yOffset = 0;
			}
		}


		//RENDER

		ImGui::Render();

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(renderer);

		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);

		//SDL_RenderRect(renderer, NULL);

		//SDL_FRect rect = { 1000, 600, 100, 100 };

		SDL_FRect tRect = { pRect->x, pRect->y, pRect->w, pRect->h };

		int x, y;
		SDL_GetWindowPosition(window, &x, &y);


		tRect.x -= x;
		tRect.y -= y;

		SDL_RenderFillRect(renderer, &tRect);
		//SDL_RenderRect(renderer, &rect);

		SDL_RenderPresent(renderer);
		
	}

	//TerminateThread(renderThread.native_handle(), 1);

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

void RenderThread(SDL_Window* window, SDL_Renderer* renderer, SDL_Rect* rect)
{
	/*
	while (true)
	{
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);

		//SDL_RenderRect(renderer, NULL);

		//SDL_FRect rect = { 1000, 600, 100, 100 };

		SDL_FRect tRect = { rect->x, rect->y, rect->w, rect->h };

		int x, y;
		SDL_GetWindowPosition(window, &x, &y);


		tRect.x -= x;
		tRect.y -= y;

		SDL_RenderFillRect(renderer, &tRect);
		//SDL_RenderRect(renderer, &rect);

		SDL_RenderPresent(renderer);
	}
	*/
}