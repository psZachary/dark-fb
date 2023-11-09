#pragma once

#include "overlay/imgui/imgui.h"
#include <Windows.h>
#include "config.h"

#define header_tab_item(name, menu_index, index) if (ImGui::Button(name, ImVec2(120.f, 25.000f))) { menu_index = index; }
#define tab_item(name, menu_index, index) if (ImGui::Button(name, ImVec2(70.f, 25.000f))) { menu_index = index; }

namespace menu {
	void draw();
}
