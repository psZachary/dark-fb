#include "combat.h"

std::unordered_map<a_pawn*, vector3> box_extent_cache{};
std::unordered_map<a_pawn*, vector3> rel_scale_cache{};

void combat::do_aimbot() {
	if (!cvar::validate_cvars()) return;
	if (!cvar::target) return;
	if (util::is_friendly(cvar::target)) return;

	vector3 aim_location = util::get_bone_position(cvar::target, sdk::player_bone::head);
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

void combat::big_heads(float activation_distance, float expansion)
{
    for (auto& player : cvar::players) {
        if (!player) continue;
         
        auto pawn = (sdk::abp_player_character*)player->pawn();
        if (!pawn) continue;

        if (pawn == cvar::local_pawn) continue;

        auto head_hitbox = pawn->head_hitbox();
        if (!head_hitbox) continue;

        if (box_extent_cache.find(pawn) == box_extent_cache.end()) {
            box_extent_cache[pawn] = head_hitbox->get_box_extent();
        }
        if (rel_scale_cache.find(pawn) == rel_scale_cache.end()) {
			rel_scale_cache[pawn] = head_hitbox->get_relative_scale();
		}

        vector3 cached_box_extent = box_extent_cache[pawn];
        vector3 old_rel_scale = rel_scale_cache[pawn];

        cached_box_extent = cached_box_extent == vector3::zero() ? vector3{ 30, 20, 20 } : cached_box_extent;
        cached_box_extent = old_rel_scale == vector3::zero() ? vector3{ 1, 1, 1 } : old_rel_scale;

        vector3 head_position = util::get_bone_position(pawn, sdk::player_bone::head);
        vector3 local_position = cvar::local_root_cmp->get_relative_location();

        bool in_activation_distance = ((head_position - local_position) / 100).magnitude() < activation_distance;
        bool is_friendly = util::is_friendly(pawn);
        if (config::combat::big_heads && in_activation_distance && !is_friendly) {
            head_hitbox->set_box_extent(cached_box_extent * expansion);
            head_hitbox->set_relative_scale(old_rel_scale * expansion);
        }
        else {
            if (is_friendly && config::combat::no_hit_teamates) {
                for (auto& hitbox : pawn->get_all_hitboxes()) {
                    hitbox->set_box_extent({ 0, 0, 0 });
					hitbox->set_relative_scale({ 0, 0, 0 });
                }
				continue;
            }
            head_hitbox->set_box_extent(cached_box_extent);
            head_hitbox->set_relative_scale(old_rel_scale);
        }
    }
}
