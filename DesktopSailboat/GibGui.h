#pragma once
#include <string>
#include "functional"
#include "ImGUI/imgui.h"
#include "Settings.h"

#define BIND_IMGUI(imguiFunc) std::bind(imguiFunc, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, "%.3f", 0)

namespace GibGui
{
	template<typename T>
	using ImGuiSliderFunc = bool(*)(const char*, T*, T, T, const char*, ImGuiSliderFlags);

	template<typename T>
	using ImGuiDragFunc = bool(*)(const char*, T*, T, T, T, const char*, ImGuiSliderFlags);

	void ResetToDefaultButton(size_t offset, size_t size, std::string buttonLabel);
	template <typename T>
	void ResetToDefaultButton(T* settingsPointer, std::string buttonLabel) {
		const char* global = (const char*)&g_Settings;
		const char* variable = (const char*)settingsPointer;
		//m_assert("Variable passed is not a member of the settings struct", variable - global < sizeof(DesktopSailboat::Settings) && variable - global > 0);
		assert(variable - global < sizeof(DesktopSailboat::Settings) && variable - global > 0 && "Variable passed is not a member of the settings struct");
		ResetToDefaultButton(variable - global, sizeof(T), buttonLabel);
	}

	void Button(std::string text, std::function<void()> func);

	template <typename T>
	void Slider(std::string text, T* val, T min, T max, std::function<bool(const char* text, T* ptr, T min, T max)> imguiFunc)
	{
		ImGui::Text(text.c_str());
		ImGui::SameLine();

		std::string temp_text = "##" + text;
		imguiFunc(temp_text.c_str(), val, min, max);


	}
	template <typename T>
	//void SettingsSlider(const char* text, T* val, T min, T max, std::function<bool(const char*, T*, T, T, const char*, ImGuiSliderFlags)> imguiFunc, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	void SettingsSlider(
		const char* label,
		T* value,
		T min,
		T max,
		ImGuiSliderFunc<T> sliderFunc,
		const char* format = "%.3f",
		ImGuiSliderFlags flags = 0
	) {
		std::string temp = label;
		ImGui::Text(temp.c_str());
		ImGui::SameLine();

		temp = "##" + temp;
		sliderFunc(temp.c_str(), value, min, max, format, flags);

		ResetToDefaultButton(value, temp.c_str());
	}

	template <typename T>
	void SettingsDrag(const char* label, T* value, T step, T min, T max, ImGuiDragFunc<T> dragFunc, const char* format = "%.3f", ImGuiSliderFlags flags = 0)
	{
		std::string temp = label;
		ImGui::Text(temp.c_str());
		ImGui::SameLine();
		temp = "##" + temp;
		dragFunc(temp.c_str(), value, step, min, max, format, flags);

		ResetToDefaultButton(value, temp.c_str());
	}

	

}