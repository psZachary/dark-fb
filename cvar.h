#pragma once
#include "sdk.h"
#include "overlay/overlay.h"

using namespace sdk;

namespace cvar {
	inline u_world* uworld = 0;
	inline u_game_instance* game_instance = 0;
	inline u_localplayer* local_player = 0;
	inline abp_player_character* local_pawn = 0;
	inline u_scene_component* local_root_cmp = 0;
	inline a_player_controller* local_player_controller = 0;
	inline u_game_viewport_client* game_viewport = 0;
	inline u_level* persistent_level = 0;
	inline u_game_state* game_state = 0;
	inline a_player_camera_manager* local_camera_manager = 0;
	inline f_camera_cache camera_cache{};

	inline std::vector<actor*> chests{};
	inline std::vector<a_floor_portal*> portals{};
	inline std::vector<actor*> all_actors{};
	inline std::vector<actor*> shrines{};
	inline std::vector<actor*> ores{};
	inline std::vector<actor*> traps{};
	inline std::vector<actor*> dead_bodies{};
	inline std::vector<actor*> projectiles{};
	inline std::vector<abp_static_mesh_item_holder_c*> ground_loot{};
	inline std::vector<adc_monster_base*> monsters{};
	inline std::vector<a_player_state*> players{};
	inline std::vector<f_account_data_replication> lobby_players{};
	inline std::mutex target_mutex{};

	inline a_pawn* target = 0;
	inline c_overlay* overlay = 0;
	inline bool all_validated = false;
	inline bool target_lock = false;

	bool validate_cvars();
	void cache_actors();
    void cache_cvars();
    void cache_threaded();
}