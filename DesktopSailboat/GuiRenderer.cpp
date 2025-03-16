#include "GuiRenderer.h"
#include "ParticleSystem.h"
#include "Settings.h"
#include "GibGui.h"

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
		

		//ImGui::SliderFloat2("Gravity", &g_Settings.sim.Gravity.x, -100.f, 100.f);
		ImGui::DragFloat2("Gravity", &g_Settings.sim.Gravity.x, 0.1f,  -100.f, 1000.f);
		//std::cout << offsetof(DesktopSailboat::Settings, sim.Gravity);
		ResetToDefaultButton(offsetof(DesktopSailboat::Settings, sim.Gravity), sizeof(Vector2));
		//ImGui::InputFloat2("Gravity", &g_Settings.sim.Gravity.x);


		


		GibGui::SettingsSlider("Rest Density: ", &g_Settings.sim.RestDensity, 0.f, 600.f, ImGui::SliderFloat);
		GibGui::SettingsSlider("Gas Constant: ", &g_Settings.sim.GasConst, 0.f, 4000.f, ImGui::SliderFloat);
		GibGui::SettingsSlider("Kernel Height: ", &g_Settings.sim.KernelHeight, 0.01f, 32.f, ImGui::SliderFloat);
		GibGui::SettingsSlider("Mass: ", &g_Settings.sim.Mass, 0.1f, 25.f, ImGui::SliderFloat);
		GibGui::SettingsSlider("Viscosity: ", &g_Settings.sim.Viscosity, 0.f, 1000.f, ImGui::SliderFloat);
		GibGui::SettingsSlider("Time Step: ", &g_Settings.sim.DT, 0.0001f, 0.0099f, ImGui::SliderFloat, "%.4f");
		GibGui::SettingsSlider("Bound Damping: ", &g_Settings.sim.BoundDamping, -1.f, 1.f, ImGui::SliderFloat);

		SIMULATION.HSQ = SIMULATION.KernelHeight * SIMULATION.KernelHeight;
		SIMULATION.EPS = SIMULATION.KernelHeight;

		SIMULATION.POLY6 = static_cast<float>(4.f / (M_PI * pow(SIMULATION.KernelHeight, 8.f)));
		SIMULATION.SPIKY_GRAD = static_cast<float>(- 10.f / (M_PI * pow(SIMULATION.KernelHeight, 5.f)));
		SIMULATION.VISC_LAP = static_cast<float>(40.f / (M_PI * pow(SIMULATION.KernelHeight, 5.f)));


		ImGui::NewLine();
		ImGui::SliderInt("Particles To Spawn", &g_Settings.sim.BlockParticles, 1, 500);
		ResetToDefaultButton(&g_Settings.sim.BlockParticles);
		if (ImGui::Button("Spawn Another Particle"))
		{
			particleSystem->SpawnParticle();
		}
		if (ImGui::Button("Reset SPH"))
		{
			particleSystem->ResetParticles();
		}
		ImGui::Text("Particle Count = %lli", particleSystem->GetNumberOfParticles());


		if (ImGui::IsAnyItemActive)
		{
			DesktopSailboat::SaveSettings();
		}

	}

	if (ImGui::IsAnyItemHovered())
	{
		window->DisableClickThrough();

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
