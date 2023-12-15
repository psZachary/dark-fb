#include "combat.h"

std::unordered_map<a_pawn*, vector3> box_extent_cache{};
std::unordered_map<a_pawn*, vector3> rel_scale_cache{};

void combat::set_attributes()
{
    if (!cvar::validate_cvars()) return;

    if (!cvar::local_pawn) return;

    if (!config::combat::action_speed_enabled && !config::combat::move_speed_enabled) return;

    auto ability_system_component = cvar::local_pawn->ability_system_cmp();
    if ((uintptr_t)ability_system_component < 0x10000) return;
    auto attribute_list = ability_system_component->get_spawned_attributes().list();
    static bool printed = false;
    if (!printed)
    {
        for (auto& attribute_set : attribute_list) {
            if (!attribute_set) continue;

            std::cout << "Health: " << attribute_set->get_health() << std::endl;
            std::cout << "Max Health: " << attribute_set->get_max_health() << std::endl;
            std::cout << "Move Speed: " << attribute_set->get_move_speed() << std::endl;
            std::cout << "Action Speed: " << attribute_set->get_action_speed() << std::endl;
            std::cout << "Iteraction Speed: " << attribute_set->get_interaction_speed() << std::endl;
            std::cout << "Item Equip Speed: " << attribute_set->get_itemequip_speed() << std::endl;

        }

        printed = true;
    }

    for (auto& attribute_set : attribute_list) {
        if (!attribute_set) continue;

        if (config::combat::move_speed_enabled)
        {
            attribute_set->set_move_speed(config::combat::move_speed);
        }

        if (config::combat::action_speed_enabled)
        {
            attribute_set->set_action_speed(config::combat::action_speed);
        }

        /*if (config::combat::itemequip_speed_enabled)
        {
            attribute_set->set_itemequip_speed(config::combat::itemequip_speed);
        }*/
    }
}

void combat::do_aimbot() {
	if (!cvar::validate_cvars()) return;
	if (!cvar::target) return;
	if (util::is_friendly(cvar::target)) return;

    auto target_bone = sdk::player_bone::head;
    if (config::combat::aim_bone == 1)
        target_bone = sdk::player_bone::neck;
    else if (config::combat::aim_bone == 2)
        target_bone = sdk::player_bone::upper_spine;
    else if (config::combat::aim_bone == 3)
        target_bone = sdk::player_bone::mid_spine;
    else if (config::combat::aim_bone == 4)
        target_bone = sdk::player_bone::lower_spine;

	vector3 aim_location = util::get_bone_position(cvar::target, target_bone);
	if (aim_location == vector3::zero()) return;

	if (last_aim_location == vector3::zero()) last_aim_location = aim_location;

	if ((last_aim_location - aim_location).magnitude() > 100.f) return;

	u_scene_component* root_component = cvar::target->root_component();
	if (!root_component) return;

	double distance = (cvar::camera_cache.pov.location - aim_location).magnitude();

	vector3 pred_location = util::get_future_position(aim_location, root_component->get_velocity(), distance, config::combat::strength);
	if (pred_location == vector3::zero()) return;

    vector2 aim_angle = util::calc_angle(config::combat::prediction ? pred_location : aim_location);
	if (aim_angle == vector2{ 0.f, 0.f }) return;

	last_aim_location = aim_location;

	cvar::local_player_controller->set_control_rotation(aim_angle);
}

void combat::arrow_tp() {
    if (!cvar::validate_cvars()) return;
    if (!cvar::target) return;
    //if (util::is_friendly(cvar::target)) return;
    for (auto projectile : cvar::projectiles)
    {
        //std::cout << "Setting projectile!" << std::endl;
        auto root_comp = projectile->root_component();
        auto character = (sdk::abp_player_character*)cvar::target;
        //root_comp->set_relative_location(character->head_hitbox()->get_relative_location());
        //std::cout << "Relative Scale: " << root_comp->get_relative_scale() << std::endl;
        root_comp->set_relative_scale(vector3(5, 5, 5));
    }
}

void combat::big_heads(float activation_distance, vector3 expansion)
{
    for (auto& player : cvar::players) {
        if (!player) continue;
         
        auto pawn = (sdk::abp_player_character*)player->pawn();
        if (!pawn) continue;

        if (pawn == cvar::local_pawn) continue;

        auto head_hitbox = pawn->head_hitbox();
        if (!head_hitbox) continue;

        auto [max_hp, hp] = util::get_health(pawn);

        if (hp <= 0.f || max_hp <= 0.f)
            continue;

        if (rel_scale_cache.find(pawn) == rel_scale_cache.end()) {
			rel_scale_cache[pawn] = head_hitbox->get_relative_scale();
		}

        vector3 cached_rel_scale = rel_scale_cache[pawn];

        cached_rel_scale = cached_rel_scale == vector3::zero() ? vector3{ 1, 1, 1 } : cached_rel_scale;

        vector3 head_position = util::get_bone_position(pawn, sdk::player_bone::head);
        vector3 local_position = cvar::local_root_cmp->get_relative_location();

        bool in_activation_distance = ((head_position - local_position) / 100).magnitude() < activation_distance;
        bool is_friendly = util::is_friendly(pawn);
        // Enemy big hitboxes
        if (!is_friendly) {
            if (config::combat::big_heads && in_activation_distance) {
                head_hitbox->set_relative_scale(cached_rel_scale * expansion);
            }
            else if (!config::combat::big_heads || !in_activation_distance) {
                if (head_hitbox->get_relative_scale() != cached_rel_scale)
                    head_hitbox->set_relative_scale(cached_rel_scale);
            }
        }
        // No hit teamates
        if (is_friendly) {
            for (auto& hitbox : pawn->get_all_hitboxes()) {
                if (config::combat::no_hit_teamates) 
                    hitbox->set_relative_scale(vector3{ 0, 0, 0 });
                else if (hitbox->get_relative_scale() != cached_rel_scale) 
					hitbox->set_relative_scale(cached_rel_scale);
            }
            continue;
        }

        if (true)
        {
            for (auto& hitbox : cvar::local_pawn->get_all_hitboxes()) {
                hitbox->set_relative_scale(vector3{ 0, 0, 0 });
            }
        }
    }
}
