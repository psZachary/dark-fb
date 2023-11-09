#pragma once
#include "cvar.h"
#include "util.h"


namespace combat {
	enum aimbot_type : int {
		fov_by_world_distance,
		fov_by_screen_distance,
		by_world_distance,
	};

	inline vector3 last_aim_location;

	void do_aimbot();
	void big_heads(float activation_distance, float expansion);
}