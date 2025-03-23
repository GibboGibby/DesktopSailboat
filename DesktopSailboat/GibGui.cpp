#include "GibGui.h"
#include "Settings.h"

void GibGui::Button(std::string text, std::function<void()> func)
{
	ImGui::Text(text.c_str());
	ImGui::SameLine();

	text = "##" + text;
	if (ImGui::Button(text.c_str()))
	{
		func();
	}
}
/*
template <typename T>
void GibGui::SettingsSlider(const char* text, T* val, T min, T max, std::function<bool(const char*, T* , T, T, const char*, ImGuiSliderFlags)> imguiFunc, const char* format, ImGuiSliderFlags flags)
{
	std::string temp = text;
	ImGui::Text(temp.c_str());
	ImGui::SameLine();

	temp = "##" + text;
	imguiFunc(temp.c_str(), val, min, max, format, flags);

	ResetToDefaultButton(val, text.c_str());
}
*/

bool GibGui::ResetToDefaultButton(size_t offset, size_t size, std::string buttonLabel)
{
	ImGui::SameLine();
	buttonLabel = "Reset##" + buttonLabel;
	if (ImGui::Button(buttonLabel.c_str()))
	{
		DesktopSailboat::Settings settings;
		memcpy((char*)&g_Settings + offset, (const char*)&settings + offset, size);
		return true;
	}
	return false;
}

