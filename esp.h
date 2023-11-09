#pragma once

#include "overlay/overlay.h"
#include "cvar.h"
#include "util.h"
#include <format>
#include <unordered_map>
#include "mathh.h"
#include <string>

namespace esp {
	extern std::unordered_map<std::string, color_m*> actor_color_map;
	extern std::unordered_map<std::string, bool*> actor_filter_map;
	extern std::unordered_map<std::string, std::string> actor_friendly_names;
	void draw();
}