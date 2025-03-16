#include "GuiRenderer.h"
#include "ParticleSystem.h"
#include "Settings.h"

GuiRenderer::GuiRenderer(const std::shared_ptr<Window> window, const std::shared_ptr<Renderer> renderer)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	int wx, wy;
	SDL_GetWindowPosition(window->GetSDLWindow(), &wx, &wy);
	ImGui::SetNextWindowPos(ImVec2(wx, wy), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	ImGui::StyleColorsDark();

	ImGui_ImplSDL3_InitForSDLRenderer(window->GetSDLWindow(), renderer->GetSDLRenderer());
	ImGui_ImplSDLRenderer3_Init(renderer->GetSDLRenderer());

	this->window = window;
}

GuiRenderer::~GuiRenderer()
{

}


void GuiRenderer::NewFrame()
{
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	
	buttonId = 0;
}

void GuiRenderer::SetCallbacks(VoidCallback spawnParticle)
{
	SpawnParticle = spawnParticle;
}

void GuiRenderer::DrawFrame()
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
				float mx, my;
				SDL_GetGlobalMouseState(&mx, &my);
				int wx, wy;
				SDL_GetWindowPosition(window->GetSDLWindow(), &wx, &wy);
				Vector2 movedAmt(wx - startPos.x, my - startPos.y);
				std::cout << "Moved amt = " << movedAmt.x << ", " << movedAmt.y << std::endl;
				particleSystem->AddForceToAllParticles(movedAmt * 3.0f);
			}
		}

		ImGui::Text("Number of Particles = %lli", particleSystem->GetNumberOfParticles());

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		break;
	case 1:
		if (ImGui::SliderInt("Particle Count", &g_Settings.app.particleCount, 1, 100000))
		{
			// Done every frame this is changed. Could do a system to callback once it returns false after returning true
			// This would indicate that the value has been changed but has also stopped being changed (for the particle count)
			// As changing every frame might be a bad idea
			DesktopSailboat::SaveSettings();
		}
		ImGui::SliderFloat("Viscosity", &g_Settings.app.viscosity, 0.0f, 10.0f);
		ImGui::Checkbox("Limit FPS: ", &g_Settings.app.limitFPS);
		ImGui::SameLine();
		if (ImGui::InputInt("##FpsLimit", &g_Settings.app.fpsLimit))
		{
			if (g_Settings.app.fpsLimit < 15) g_Settings.app.fpsLimit = 15;
		}



		ImGui::Checkbox("Limit Simulation FPS: ", &g_Settings.app.limitSimFPS);
		ImGui::SameLine();
		ImGui::InputInt("##simFpsLimit", &g_Settings.app.simFpsLimit);
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


		if (ImGui::InputFloat("Box Width", &g_Settings.app.boxWidth) || ImGui::InputFloat("Box Height", &g_Settings.app.boxHeight))
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

		//ImGui::SliderFloat2("Gravity", &g_Settings.sim.Gravity.x, -100.f, 100.f);
		ImGui::DragFloat2("Gravity", &g_Settings.sim.Gravity.x, 0.1f,  -100.f, 100.f);
		//std::cout << offsetof(DesktopSailboat::Settings, sim.Gravity);
		ResetToDefaultButton(offsetof(DesktopSailboat::Settings, sim.Gravity), sizeof(Vector2));
		//ImGui::InputFloat2("Gravity", &g_Settings.sim.Gravity.x);


		ImGui::SliderInt("Particles To Spawn", &g_Settings.sim.BlockParticles, 1, 500);
		ResetToDefaultButton(&g_Settings.sim.BlockParticles);

		ImGui::Text("Particle Count = %lli", particleSystem->GetNumberOfParticles());
	}


	ImGui::End();
}

void GuiRenderer::Render(const std::shared_ptr<Renderer> renderer)
{
	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer->GetSDLRenderer());
}

void GuiRenderer::ResetToDefaultButton(size_t offset, size_t size)
{
	ImGui::SameLine();
	ImGui::PushID(buttonId++);
	if (ImGui::Button("Reset##"))
	{
		DesktopSailboat::Settings settings;
		memcpy((char*) & g_Settings + offset, (const char*) & settings + offset, size);
	}
	ImGui::PopID();
}

template<typename T>
void GuiRenderer::ResetToDefaultButton(T* settingsPointer)
{
	const char* global = (const char*)&g_Settings;
	const char* variable = (const char*)settingsPointer;
	//m_assert("Variable passed is not a member of the settings struct", variable - global < sizeof(DesktopSailboat::Settings) && variable - global > 0);
	assert(variable - global < sizeof(DesktopSailboat::Settings) && variable - global > 0 && "Variable passed is not a member of the settings struct");
	ResetToDefaultButton(variable - global, sizeof(T));
}
