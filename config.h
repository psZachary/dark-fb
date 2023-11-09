#pragma once

#include "mathh.h"

namespace config {
	namespace menu {
		inline bool is_open = true;
		inline int toggle_key = VK_INSERT;
		inline bool hide_overlay = true;
	}
	namespace colors {
		inline color_m color_junk = color_rgb{ 100, 100, 100, 255 };
		inline color_m color_poor = color_rgb{ 100, 100, 100, 255 };
		inline color_m color_common = color_rgb{ 222, 222, 222, 255 };
		inline color_m color_uncommon = color_rgb{ 98, 190, 11, 255 };
		inline color_m color_epic = color_rgb{ 173, 90, 255, 255 };
		inline color_m color_rare = color_rgb{ 74, 155, 209, 255 };
		inline color_m color_legendary = color_rgb{ 247, 162, 45, 255 };
	}
	namespace combat {
		inline bool aimbot = true;
		inline float strength = 30.f;
		inline float fov = 150.f;
		inline int key = 5;
		inline int aim_type = 1;
		inline bool prediction = true;
		inline bool big_heads = false;
		inline bool no_hit_teamates = false;
		inline bool big_head_only_target = false;
		inline float big_head_expansion = 5.0f;
		inline float activation_distance = 5.0f;
	}
	namespace esp {
		namespace ai {
			inline bool enabled = true;
			inline bool distance = true;
			inline bool name = true;
			inline bool health = true;
			inline bool minibosses = true;
			inline bool bosses = true;
			inline bool goblin = true;
			inline bool skeleton = true;
			inline bool mummy = true;
			inline bool zombie = true;
			inline bool skull = true;
			inline bool spider = true;
			inline bool other = true;
			inline float render_distance = 10.f;
			namespace colors {
				inline color_m ai = color_rgb{ 220, 220, 220, 255 };
			}
		}
		namespace loot {
			inline bool enabled = true;
			inline bool name = true;
			inline bool distance = true;
			inline float render_distance = 10.f;
			namespace colors {
				// bright purple (contrasting purple)
				inline color_m ground_loot = color_rgb{ 255, 0, 255, 255 };

			}
		}
		namespace players {
			inline bool enabled = true;
			inline bool name = true;
			inline bool distance = true;
			inline bool health = true;
			inline bool equipped_items = true;
			inline bool armor_items = true;
			inline bool utility_items = true;
			inline bool sheathed_items = false;
			inline bool skeleton = true;
			inline bool box = false;
			inline bool head_hitboxes = true;
			inline bool highlight_target = true;
			namespace lobby {
				inline bool enabled = false;
				inline bool party_id;
				inline bool level = true;
				inline bool class_name = true;
				inline bool name = true;
				inline bool show = true;
				inline float transparency = 0.5f;
				inline int toggle_key = VK_DELETE;
			}
			namespace game_status {
				inline bool enabled;
			}
			namespace colors {
				inline color_m skeleton = color_rgb{ 255, 255, 255, 255 };
				inline color_m box = color_rgb{ 255, 255, 255, 255 };
				inline color_m name = color_rgb{ 255, 255, 255, 255 };
				inline color_m distance = color_rgb{ 255, 255, 255, 255 };
				inline color_m friendly = color_rgb{ 0, 255, 0, 255 };
				inline color_m target = color_rgb{ 255, 200, 200, 255 };
			}
		}
		namespace trap {
			inline bool enabled = true;
			inline bool name = true;
			inline bool distance = true;
			inline bool wall_spike = true;
			inline bool floor_spike = true;
			inline bool hunting_trap = true;

			namespace colors {
				inline color_m floor_spike = color_rgb{ 173, 76, 76, 255 };
				inline color_m wall_spike = color_rgb{ 173, 76, 76, 255 };
				inline color_m hunting_trap = color_rgb{ 173, 76, 76, 255 };
			}

			inline float render_distance = 10.f;
		}
		namespace ore {
			inline bool enabled = true;
			inline bool name = true;
			inline bool distance = true;
			inline bool gold = true;
			inline bool ruby_silver = true;
			inline bool cobalt = true;
			inline float render_distance = 25.f;
			namespace colors {
				inline color_m gold = color_rgb{ 252, 219, 3, 255 };
				inline color_m ruby_silver = color_rgb{ 153, 9, 15, 255 };
				inline color_m cobalt = color_rgb{ 71, 88, 194, 255 };
			}
		}
		namespace shrine {
			inline bool enabled = true;
			inline bool name = true;
			inline bool distance = true;
			inline bool revive = true;
			inline bool health = true;
			inline bool shield = false;
			inline bool power = false;
			inline bool speed = false;

			namespace colors {
				inline color_m revive = color_rgb{ 210, 210, 230, 255 };
				inline color_m health = color_rgb{ 187, 196, 140, 255 };
				inline color_m shield = color_rgb{ 231, 140, 160, 255 };
				inline color_m power = color_rgb{ 252, 210, 125, 255 };
				inline color_m speed = color_rgb{ 160, 187, 233, 255 };
			}
		}
		namespace other {
			inline bool actors = false;
		}
		namespace portal {
			inline bool enabled = true;
			inline bool name = true;
			inline bool distance = true;
			inline bool escape = true;
			inline bool down = true;

			namespace colors {
				inline color_m down = color_rgb{ 234, 39, 69, 255 };
				inline color_m escape = color_rgb{ 43, 133, 255, 255 };
			}
		}
		namespace chest {
			inline bool enabled = true;
			inline bool name = true;
			inline bool distance = true;
			inline bool gold = true;
			inline bool ornate = true;
			inline bool lions_head = true;
			inline bool coffin = true;
			inline bool mimic = true;
			inline bool flat = true;
			inline bool other = true;
			inline float render_distance = 12.f;

			namespace colors {
				inline color_m gold = color_rgb(252, 219, 3, 255);
				inline color_m ornate = color_rgb{ 184, 105, 15, 255 };
				inline color_m lions_head = color_rgb{ 252, 135, 3, 255 };
				inline color_m coffin = color_rgb{ 255, 255, 255, 255 };
				inline color_m mimic = color_rgb{ 173, 76, 76, 255 };
				inline color_m flat = color_rgb{ 255, 255, 255, 255 };
				inline color_m other = color_rgb{ 255, 255, 255, 255 };
			}
		}
	}
	namespace menu {
		inline int tab_index = 0;
		namespace visuals {
			inline int tab_index = 0;
		}
	}

}