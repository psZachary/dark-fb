#pragma once

#include <iostream>
#include "sdk.h"
#include "memory.h"
#include "cvar.h"
#include "esp.h"
#include <algorithm>

namespace util {
	static vector2 screen_size() {
		static auto screen_size = vector2{};
		if (screen_size != vector2{ 0, 0 }) return screen_size;

		RECT rect;
		GetWindowRect(mem::hwnd, &rect);
		screen_size = vector2{ (float)(rect.right - rect.left), (float)(rect.bottom - rect.top) };

		return screen_size;
	}

	static void get_offscreen_point(vector2& point, double degrees)
	{
		double x2 = screen_size().x / 2;
		double y2 = screen_size().y / 2;
		double d = sqrt(pow((point.x - x2), 2) + (pow((point.y - y2), 2)));
		double r = degrees / d;
		point.x = r * point.x + (1 - r) * x2;
		point.y = r * point.y + (1 - r) * y2; 
	}
	static vector3 get_future_position(vector3 position, vector3 velocity, double distance, double strength) {
		if (velocity.magnitude() == 0) return position;

		double time = distance / (velocity.magnitude() * strength);

		vector3 future_position = position + velocity * time;

		future_position.z = position.z;

		return future_position;
	}

	static void move_mouse(int x, int y) {
		mouse_event(MOUSEEVENTF_MOVE, x, y, NULL, NULL);
	}

	inline std::map<int, std::string> cached_fnames{};

	static std::string get_name_from_fname(int key)
	{
		auto cached_name = cached_fnames.find(key);
		if (cached_name != cached_fnames.end())
			return cached_name->second;

		auto chunkOffset = (UINT)((int)(key) >> 16);
		auto name_offset = (USHORT)key;

		auto pool_chunk = rpm<UINT64>(mem::module_base + GNAMES + ((chunkOffset + 2) * 8));
		auto entry_offset = pool_chunk + (ULONG)(2 * name_offset);
		auto name_entry = rpm<INT16>(entry_offset);

		auto len = name_entry >> 6;
		char buff[1028];
		if ((DWORD)len && len > 0)
		{
			memset(buff, 0, 1028);
			read_raw(entry_offset + 2, buff, len);
			buff[len] = '\0';
			std::string ret(buff);
			cached_fnames.emplace(key, ret);
			return std::string(ret);
		}
		else return "";
	}

	static std::string actor_name(sdk::actor* act)
	{
		int key = act->fname_index();
		return get_name_from_fname(key);
	}

	static matrix4x4_t matrix(vector3 rot, vector3 origin = vector3(0, 0, 0))
	{
		double rad_pitch = (rot.x * M_PI / 180);
		double rad_yaw = (rot.y * M_PI / 180);
		double rad_roll = (rot.z * M_PI / 180);

		double SP = sin(rad_pitch);
		double CP = cos(rad_pitch);
		double SY = sin(rad_yaw);
		double CY = cos(rad_yaw);
		double SR = sin(rad_roll);
		double CR = cos(rad_roll);

		matrix4x4_t matrix;
		matrix.m[0][0] = CP * CY;
		matrix.m[0][1] = CP * SY;
		matrix.m[0][2] = SP;
		matrix.m[0][3] = 0.f;

		matrix.m[1][0] = SR * SP * CY - CR * SY;
		matrix.m[1][1] = SR * SP * SY + CR * CY;
		matrix.m[1][2] = -SR * CP;
		matrix.m[1][3] = 0.f;

		matrix.m[2][0] = -(CR * SP * CY + SR * SY);
		matrix.m[2][1] = CY * SR - CR * SP * SY;
		matrix.m[2][2] = CR * CP;
		matrix.m[2][3] = 0.f;

		matrix.m[3][0] = origin.x;
		matrix.m[3][1] = origin.y;
		matrix.m[3][2] = origin.z;
		matrix.m[3][3] = 1.f;

		return matrix;
	}

	static color_m get_hp_color(float health, float max_health) {
		float r = 255 - (health * 255 / max_health);
		float g = health * 255 / max_health;
		return color_m(r / 255, g / 255, 0, 1);
	}

	static std::tuple<float, float> get_health(sdk::adc_character_base* character) {
		if (!character) return std::tuple<float, float>{ 0.f, 0.f };

		auto asc = character->ability_system_cmp();
		if (!asc) return std::tuple<float, float>{ 0.f, 0.f };

		auto attributes = asc->get_spawned_attributes();

		auto attr = attributes.at(0);
		if (!attr) return std::tuple<float, float>{ 0.f, 0.f };

		return std::tuple<float, float>{attr->get_max_health(), attr->get_health() };
	}

	static vector3 get_world_position(sdk::f_transform c2w, sdk::f_transform component_transform) {
		matrix4x4_t matrix = matrix_multiply(component_transform.to_matrix_with_scale(), c2w.to_matrix_with_scale());
		return vector3(matrix._41, matrix._42, matrix._43);
	}

	static std::string string_replace(std::string subject, const std::string& search,
		const std::string& replace) {
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::string::npos) {
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
		return subject;
	}

	static std::vector<std::pair<vector3, vector3>> get_draw_box_points(sdk::u_scene_component* scene_cmp) {
		std::vector<std::pair<vector3, vector3>> points{};
		if (!scene_cmp) return points;

		f_transform component_transform = scene_cmp->get_transform();
		vector3 world_location = get_world_position(scene_cmp->get_comp_to_world(), component_transform);
		vector3 scale = component_transform.scale3d;
		
		scale = scale * 10.f;
		
		// front
		points.push_back({ vector3(world_location.x - scale.x, world_location.y - scale.y, world_location.z - scale.z), vector3(world_location.x + scale.x, world_location.y - scale.y, world_location.z - scale.z) });
		points.push_back({ vector3(world_location.x - scale.x, world_location.y - scale.y, world_location.z - scale.z), vector3(world_location.x - scale.x, world_location.y + scale.y, world_location.z - scale.z) });
		points.push_back({ vector3(world_location.x + scale.x, world_location.y - scale.y, world_location.z - scale.z), vector3(world_location.x + scale.x, world_location.y + scale.y, world_location.z - scale.z) });
		points.push_back({ vector3(world_location.x - scale.x, world_location.y + scale.y, world_location.z - scale.z), vector3(world_location.x + scale.x, world_location.y + scale.y, world_location.z - scale.z) });

		// back
		points.push_back({ vector3(world_location.x - scale.x, world_location.y - scale.y, world_location.z + scale.z), vector3(world_location.x + scale.x, world_location.y - scale.y, world_location.z + scale.z) });
		points.push_back({ vector3(world_location.x - scale.x, world_location.y - scale.y, world_location.z + scale.z), vector3(world_location.x - scale.x, world_location.y + scale.y, world_location.z + scale.z) });
		points.push_back({ vector3(world_location.x + scale.x, world_location.y - scale.y, world_location.z + scale.z), vector3(world_location.x + scale.x, world_location.y + scale.y, world_location.z + scale.z) });
		points.push_back({ vector3(world_location.x - scale.x, world_location.y + scale.y, world_location.z + scale.z), vector3(world_location.x + scale.x, world_location.y + scale.y, world_location.z + scale.z) });

		// sides
		points.push_back({ vector3(world_location.x - scale.x, world_location.y - scale.y, world_location.z - scale.z), vector3(world_location.x - scale.x, world_location.y - scale.y, world_location.z + scale.z) });
		points.push_back({ vector3(world_location.x + scale.x, world_location.y - scale.y, world_location.z - scale.z), vector3(world_location.x + scale.x, world_location.y - scale.y, world_location.z + scale.z) });
		points.push_back({ vector3(world_location.x - scale.x, world_location.y + scale.y, world_location.z - scale.z), vector3(world_location.x - scale.x, world_location.y + scale.y, world_location.z + scale.z) });
		points.push_back({ vector3(world_location.x + scale.x, world_location.y + scale.y, world_location.z - scale.z), vector3(world_location.x + scale.x, world_location.y + scale.y, world_location.z + scale.z) });

		return points;
	}

	static vector3 get_bone_position(sdk::a_pawn* pawn, int bone_index) {
		if (!pawn) return vector3::zero();

		auto mesh = pawn->skeletal_mesh();
		if (!mesh) return vector3::zero();

		auto c2w = mesh->get_comp_to_world();
		auto component_transform = mesh->get_bone(bone_index);

		return get_world_position(c2w, component_transform);
	}

	static bool is_a(sdk::u_object* object, std::string name) {
		if (util::get_name_from_fname(object->class_private()->fname_index()).compare(name) == 0)
			return true;

		auto super = object->class_private()->super();
		while (super) {
			if (util::get_name_from_fname(super->fname_index()).compare(name) == 0)
				return true;
			super = super->super();
		}
		return false;
	}
	
	static std::tuple<std::string, std::string> get_item_name_rarity(sdk::a_item_actor* item) {
		if (!item) return { "", "" };

		sdk::f_design_data_item design_data_item = item->design_data_item();

		std::string item_name = util::get_name_from_fname(design_data_item.id_tag.index);
		std::string rarity_name = util::get_name_from_fname(design_data_item.rarity_type.index);

		return { item_name, rarity_name };
	}

	static std::tuple<std::string, std::string> get_item_name_rarity(sdk::u_item* item) {
		if (!item) return { "", "" };

		sdk::f_design_data_item design_data_item = item->design_data_item();

		std::string item_name = util::get_name_from_fname(design_data_item.id_tag.index);
		std::string rarity_name = util::get_name_from_fname(design_data_item.rarity_type.index);

		return { item_name, rarity_name };
	}

	static e_rarity get_rarity_from_name(std::string name)
	{
		if (name.find("Junk") != std::string::npos)
			return e_rarity::rarity_junk;
		else if (name.find("Poor") != std::string::npos)
			return e_rarity::rarity_poor;
		else if (name.find("Common") != std::string::npos)
			return e_rarity::rarity_common;
		else if (name.find("Uncommon") != std::string::npos)
			return e_rarity::rarity_common;
		else if (name.find("Epic") != std::string::npos)
			return e_rarity::rarity_epic;
		else if (name.find("Rare") != std::string::npos)
			return e_rarity::rarity_rare;
		else if (name.find("Legendary") != std::string::npos)
			return e_rarity::rarity_legendary;
		else if (name.find("Unique") != std::string::npos)
			return e_rarity::rarity_unique;
	}

	static bool w2s(vector3 location, sdk::f_minimal_view_info camera_cache, vector3& screen_loc) {
		auto pov = camera_cache;
		vector3 rotation = pov.rotation;
		matrix4x4_t temp_matrix = matrix(rotation);


		vector3 v_axis_x = vector3(temp_matrix.m[0][0], temp_matrix.m[0][1], temp_matrix.m[0][2]);
		vector3 v_axis_y = vector3(temp_matrix.m[1][0], temp_matrix.m[1][1], temp_matrix.m[1][2]);
		vector3 v_axis_z = vector3(temp_matrix.m[2][0], temp_matrix.m[2][1], temp_matrix.m[2][2]);

		vector3 v_delta = location - pov.location;
		vector3 v_transformed = vector3(v_delta.dot(v_axis_y), v_delta.dot(v_axis_z), v_delta.dot(v_axis_x));

		if (v_transformed.z < 1.f)
			return false;


		screen_loc.x = (screen_size().x / 2) + v_transformed.x * ((screen_size().x / 2) / tanf(pov.fov * (float)M_PI / 360.f)) / v_transformed.z;
		screen_loc.y = (screen_size().y / 2) - v_transformed.y * ((screen_size().x / 2) / tanf(pov.fov * (float)M_PI / 360.f)) / v_transformed.z;

		if (screen_loc.x > screen_size().x ||
			screen_loc.y > screen_size().y ||
			screen_loc.y < 0 ||
			screen_loc.x < 0) return false;

		return true;
	}

	static std::tuple<bool, box> get_box(sdk::a_pawn* player) {
		if (!cvar::local_player_controller) return { false, box{} };

		auto camera_manager = cvar::local_player_controller->camera_manager();
		if (!camera_manager) return { false, box{} };

		auto camera_cache = camera_manager->last_frame_cam_cache_private();

		vector3 head = get_bone_position(player, sdk::player_bone::head);
		vector3 base = get_bone_position(player, 0);

		box ret{};

		vector3 head_w2s{};
		vector3 base_w2s{};
		vector3 half_w2s{};

		if (util::w2s(head, camera_cache.pov, head_w2s) &&
			util::w2s(base, camera_cache.pov, base_w2s) &&
			util::w2s(base + vector3{ 0, 100.f, 0 }, camera_cache.pov, half_w2s)) {

			ret.height = { (base_w2s.y - head_w2s.y) * 1.20f };
			ret.width = { (base_w2s.y - half_w2s.y) * 2.45f };

			ret.position = vector2{ (base_w2s.x - ret.width * 0.50f), (base_w2s.y - ret.height) };
		}

		if (ret.position == vector2(0, 0))
			return { false, ret };

		return { true, ret };
	}

	static bool is_friendly(sdk::a_pawn* character) {
		if (!cvar::validate_cvars()) return false;
		//return false;
		//if (config::combat::no_hit_teamates)
		auto data = character->account_replication_data(); // get_account_data_replication_party_id();
		auto local_data = cvar::local_pawn->account_replication_data(); // get_account_data_replication_party_id();
		auto local_party_id = local_data.party_id.read_string();
		auto other_party_id = data.party_id.read_string();
		if (local_party_id.empty())
		{
			//std::cout << "Invalid Local PartyID" << std::endl;
			return false;
		}
		if (other_party_id.empty())
		{
			//std::cout << "Invalid Enemy PartyID" << std::endl;
			return false;
		}
		
		/*auto nickname = data.nickname.original_nickname.read_string();
		std::wstring nicknamews(nickname);
		std::string nickname_str(nicknamews.begin(), nicknamews.end());*/
		
		int local_party_id_int = std::stoi(local_party_id);

		int other_party_id_int = std::stoi(other_party_id);
		
		//std::cout << "Local Party ID: " << local_party_id_str << std::endl;
		//std::cout << nickname_str << " Party ID: " << other_party_id_str << std::endl;
		if (local_party_id_int == other_party_id_int)
		{
			return true;
		}

		return false;
	}

	static vector2 calc_angle(vector3 to) {
		if (!cvar::validate_cvars()) return { 0.f, 0.f };

		auto vinfo = cvar::local_camera_manager->last_frame_cam_cache_private().pov;

		vector3 delta = (vinfo.location - to);
		double hyp = sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);

		vector2 rot{};
		rot.x = acos(delta.z / hyp) * (double)((57.29578f));
		rot.y = atan(delta.y / delta.x) * (double)((57.29578f));
		
		rot.x += 270.f;
		if (rot.x > 360.f) {
			rot.x -= 360.f;
		}
		if (delta.x >= 0.0f) {
			rot.y += 180.0f;
		}
		if (rot.y < 0.f) {
			rot.y += 360.f;
		}

		return rot;
	}

	struct player_fov_info {
		double screen_distance;
		double world_distance;
		a_pawn* pawn;
	};

	static a_pawn* get_closest_player_by_world_distance(sdk::player_bone aim_bone) {
		if (!cvar::validate_cvars()) return 0;

		a_pawn* closest_player = 0;
		double closest_dst = DBL_MAX;

		vector3 local_position = cvar::local_root_cmp->get_relative_location();

		for (a_player_state* player : cvar::players) {
			if (!player) continue;

			a_pawn* pawn = player->pawn();
			if (!pawn || pawn == cvar::local_pawn) continue;

			if (util::is_friendly(pawn)) continue;

			auto [health, max_health] = util::get_health(pawn);
			if (health <= 0.f) continue;

			vector3 position = util::get_bone_position(pawn, aim_bone);

			if (position == vector3::zero()) continue;

			double distance = (local_position - position).magnitude();

			if (distance < closest_dst) {
				closest_dst = distance;
				closest_player = pawn;
			}
		}

		return closest_player;
	}

	static std::vector<player_fov_info> get_players_in_fov(sdk::player_bone aim_bone, float fov) {
		std::vector<player_fov_info> pawn_list{};

		if (!cvar::validate_cvars()) return pawn_list;

		a_pawn* closest_player = 0;
		double closest_dst = DBL_MAX;

		auto camera_manager = cvar::local_player_controller->camera_manager();
		if (!camera_manager) return pawn_list;

		auto camera_cache = camera_manager->last_frame_cam_cache_private();

		vector3 local_position = cvar::local_root_cmp->get_relative_location();

		for (a_player_state* player : cvar::players) {
			if (!player) continue;

			a_pawn* pawn = player->pawn();
			if (!pawn || pawn == cvar::local_pawn) continue;

			if (util::is_friendly(pawn)) continue;

			auto [health, max_health] = util::get_health(pawn);
			if (health <= 0.f) continue;

			vector3 position = util::get_bone_position(pawn, aim_bone);

			if (position == vector3::zero()) continue;

			vector3 screen_position{};
			if (!util::w2s(position, camera_cache.pov, screen_position)) continue;

			vector2 screen_center = util::screen_size() / 2;

			double s_distance = (screen_center - vector2(screen_position)).magnitude();
			double w_distance = (local_position - position).magnitude();

			if (s_distance > fov) continue;
			else pawn_list.push_back({ s_distance, w_distance, pawn });
		}

		return pawn_list;
	}

	static a_pawn* get_closest_player_in_fov_by_screen_distance(sdk::player_bone aim_bone, float fov) {
		auto fov_players = get_players_in_fov(aim_bone, fov);

		if (fov_players.size() == 0) return 0;
		
		std::sort(fov_players.begin(), fov_players.end(), [](player_fov_info a, player_fov_info b) {
			return a.screen_distance < b.screen_distance;
		});

		return fov_players[0].pawn;
	}

	static a_pawn* get_closest_player_in_fov_by_world_distance(sdk::player_bone aim_bone, float fov) {
		auto fov_players = get_players_in_fov(aim_bone, fov);

		if (fov_players.size() == 0) return 0;

		std::sort(fov_players.begin(), fov_players.end(), [](player_fov_info a, player_fov_info b) {
			return a.world_distance < b.world_distance;
		});

		return fov_players[0].pawn;
	}

	static std::string read_fstring(sdk::fstring f_string) {
		return f_string.read_string();
	}

	static std::map<int, std::vector<sdk::f_account_data_replication>> get_parties(std::vector<sdk::f_account_data_replication> player_data) {
		std::map<int, std::vector<f_account_data_replication>> parties{};

		for (auto& lobby_player : player_data) {
			try {
				std::string party_id_str = util::read_fstring(lobby_player.party_id);
				int party_id = std::stoi(party_id_str == "" ? "0" : party_id_str);
				if (party_id == 0) {
					parties[0].push_back(lobby_player);
				}
				else {
					if (parties.find(party_id) == parties.end()) {
						parties[party_id].push_back(lobby_player);
					}
					else {
						parties[party_id].push_back(lobby_player);
					}
				}
			}
			catch (std::exception e) {
				continue;
			}
		}

		return parties;
	}
	static std::tuple<std::string, std::string> get_names(sdk::a_pawn* pawn) {
		auto replicated_data = pawn->account_replication_data();
		if (replicated_data.account_id._data == 0 || replicated_data.nickname.original_nickname._data == 0 || replicated_data.nickname.streaming_nickname._data == 0)
			return { "", "" };

		auto original_nickname = replicated_data.nickname.original_nickname;
		auto streaming_nickname = replicated_data.nickname.streaming_nickname;

		auto player_name = read_fstring(original_nickname);
		auto class_name = read_fstring(streaming_nickname);
		class_name = class_name.substr(0, class_name.find_last_of("#"));

		return { player_name, class_name };
	}
}