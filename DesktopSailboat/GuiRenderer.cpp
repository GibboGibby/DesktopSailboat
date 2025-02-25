#include "GuiRenderer.h"

GuiRenderer::GuiRenderer(const std::shared_ptr<Window> window, const std::shared_ptr<Renderer> renderer)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL3_InitForSDLRenderer(window->GetSDLWindow(), renderer->GetSDLRenderer());
	ImGui_ImplSDLRenderer3_Init(renderer->GetSDLRenderer());
}

GuiRenderer::~GuiRenderer()
{

}


void GuiRenderer::NewFrame()
{
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
}

void GuiRenderer::Render(const std::shared_ptr<Renderer> renderer)
{
	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer->GetSDLRenderer());
}
