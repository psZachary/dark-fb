#include "cvar.h"
#include "util.h"
#include "combat.h"

bool cvar::validate_cvars()
{
    return cvar::all_validated;
}

void cvar::cache_actors()
{
    std::vector<actor*> chest_actors{};
    std::vector<a_floor_portal*> portal_actors{};
    std::vector<actor*> all_actors{};
    std::vector<actor*> shrine_actors{};
    std::vector<actor*> trap_actors{};
    std::vector<actor*> ore_actors{};
    std::vector<actor*> dead_body_actors{};
    std::vector<actor*> projectile_actors{};
    std::vector<abp_static_mesh_item_holder_c*> ground_loot_actors{};
    std::vector<adc_monster_base*> monster_actors{};

    t_array<u_level*> levels = cvar::uworld->levels();
    for (u_level* level : levels.list()) {
        if (!level) continue;

        t_array<actor*> actors = level->actors();
        for (actor*& actor : actors.list()) {
            if (!actor) continue;

            auto actor_name = util::actor_name(actor);

            if (util::is_a(actor, "BP_StaticMeshItemHolder_C")) {
                ground_loot_actors.push_back((abp_static_mesh_item_holder_c*)actor);
                continue;
            }

            if (actor_name.find("PlayerCharacterDungeon_C") != std::string::npos)
            {
                dead_body_actors.push_back(actor);
                continue;
            }

            if (actor_name.find("Chest") != std::string::npos ||
                actor_name.find("Coffin") != std::string::npos ||
                actor_name.find("Mimic") != std::string::npos) {
                chest_actors.push_back(actor);
                continue;
            }

            if (util::is_a(actor, "BP_DCMonsterBase_C")) {
                monster_actors.push_back((adc_monster_base*)(actor));
				continue;
			}

            if (actor_name.find("FloorPortal") != std::string::npos||
                actor_name.find("CryptEscape") != std::string::npos ||
                actor_name.find("CryptDown") != std::string::npos
                ) {
                portal_actors.push_back((a_floor_portal*)(actor));
                continue;
            }

            if (actor_name.find("Statue") != std::string::npos ||
                actor_name.find("Altar") != std::string::npos) {
                if (config::esp::shrine::enabled)
                    shrine_actors.push_back(actor);
                continue;
            }

            //ABP_FloorSpikes_C
            if (actor_name.find("Spike") != std::string::npos ||
                actor_name.find("HuntingTrap") != std::string::npos) {
                trap_actors.push_back(actor);
                continue;
            }

            //
            if (actor_name.find("Ore") != std::string::npos) {
                ore_actors.push_back(actor);
				continue;
            }

            /*if (actor_name.find("Projectile") != std::string::npos)
            {
                std::cout << "Found " << actor_name << std::endl;
                continue;
            }*/

            /*if (actor_name.find("BP_ArrowProjectile_C") != std::string::npos) {
                projectile_actors.push_back(actor);
                continue;
            }*/

            if (config::esp::other::actors)
                all_actors.push_back(actor);
        }
    }

    cvar::chests = chest_actors;
    cvar::portals = portal_actors;
    cvar::all_actors = all_actors;
    cvar::shrines = shrine_actors;
    cvar::traps = trap_actors;
    cvar::ores = ore_actors;
    cvar::monsters = monster_actors;
    cvar::dead_bodies = dead_body_actors;
    cvar::ground_loot = ground_loot_actors;
    //cvar::projectiles = projectile_actors;
}

void cvar::cache_cvars()
{
    cvar::uworld = u_world::get_world();
    if (!cvar::uworld) { cvar::all_validated = false; return; }

    cache_actors();

    cvar::game_instance = uworld->owning_game_instance();
    if (!cvar::game_instance) { cvar::all_validated = false; return; }
    cvar::local_player = game_instance->get_localplayer();
    if (!cvar::local_player) { cvar::all_validated = false; return; }
    cvar::game_viewport = local_player->viewport_client();
    if (!cvar::game_viewport) { cvar::all_validated = false; return; }
    cvar::persistent_level = uworld->persistent_level();
    if (!cvar::persistent_level) { cvar::all_validated = false; return; }
    cvar::game_state = uworld->game_state();
    if (!cvar::game_state) { cvar::all_validated = false; return; }
    cvar::local_player_controller = local_player->player_controller();
    if (!cvar::local_player_controller) { cvar::all_validated = false; return; }
    cvar::local_pawn = (abp_player_character*)local_player_controller->pawn();
    if (!cvar::local_pawn) { cvar::all_validated = false; return; }
    cvar::local_root_cmp = local_pawn->root_component();
    if (!cvar::local_root_cmp) { cvar::all_validated = false; return; }
    cvar::local_camera_manager = local_player_controller->camera_manager();
    if (!cvar::local_camera_manager) { cvar::all_validated = false; return; }
    
    cvar::all_validated = true;

    try {
        cvar::lobby_players = game_state->account_data_replication().list();
        cvar::players = game_state->player_array().list();
    }
    catch (std::exception e) {
        return;
    }

    target_mutex.lock();
    if (!target_lock)
    {
        if (config::combat::aim_type == combat::aimbot_type::fov_by_screen_distance) 
            cvar::target = util::get_closest_player_in_fov_by_screen_distance(sdk::player_bone::head, config::combat::fov);
        if (config::combat::aim_type == combat::aimbot_type::fov_by_world_distance)
            cvar::target = util::get_closest_player_in_fov_by_world_distance(sdk::player_bone::head, config::combat::fov);
        if (config::combat::aim_type == combat::aimbot_type::by_world_distance)
            cvar::target = util::get_closest_player_by_world_distance(sdk::player_bone::head);
    }
    target_mutex.unlock();
}

void cvar::cache_threaded()
{
    while (true) {
        cache_cvars();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
