#include "esp.h"
#include "config.h"
#include "overlay/overlay.h"
#include "combat.h"

namespace esp {
	std::unordered_map<std::string, color_m*> actor_color_map = {
		{ "BP_AltarOfSacrifice_C", &config::esp::shrine::colors::revive },
		{ "BP_Statue01_C", &config::esp::shrine::colors::health },
		{ "BP_Statue02_C", &config::esp::shrine::colors::shield },
		{ "BP_Statue03_C", &config::esp::shrine::colors::power },
		{ "BP_Statue04_C", &config::esp::shrine::colors::speed },
		{ "BP_WallSpike_C", &config::esp::trap::colors::wall_spike },
		{ "BP_FloorSpikes_C", &config::esp::trap::colors::floor_spike },
		{ "Gold", &config::esp::chest::colors::gold },
		{ "Ornate", &config::esp::chest::colors::ornate },
		{ "Lion", &config::esp::chest::colors::lions_head },
		{ "Coffin", &config::esp::chest::colors::coffin },
		{ "Flat", &config::esp::chest::colors::flat },
		{ "Mimic", &config::esp::chest::colors::mimic },
		{ "HuntingTrapProp", &config::esp::trap::colors::hunting_trap },
		{ "BP_GoldOre_N_C", &config::esp::ore::colors::gold },
		{ "BP_RubysilverOre_N_C", &config::esp::ore::colors::ruby_silver },
		{ "BP_CobaltOre_N_C", &config::esp::ore::colors::cobalt },
		{ "BP_StaticMeshItemHolder_C", &config::esp::loot::colors::ground_loot }
	};
	std::unordered_map<std::string, bool*> actor_filter_map = {
		{ "BP_AltarOfSacrifice_C", &config::esp::shrine::revive },
		{ "BP_Statue01_C", &config::esp::shrine::health },
		{ "BP_Statue02_C", &config::esp::shrine::shield },
		{ "BP_Statue03_C", &config::esp::shrine::power },
		{ "BP_Statue04_C", &config::esp::shrine::speed },
		{ "BP_WallSpike_C", &config::esp::trap::wall_spike },
		{ "BP_FloorSpikes_C", &config::esp::trap::floor_spike },
		{ "Gold", &config::esp::chest::gold },
		{ "Ornate", &config::esp::chest::ornate },
		{ "Lion", &config::esp::chest::lions_head },
		{ "Coffin", &config::esp::chest::coffin },
		{ "Flat", &config::esp::chest::flat },
		{ "HuntingTrapProp", &config::esp::trap::hunting_trap },
		{ "BP_FloorPortalScrollDown_C", &config::esp::portal::down },
		{ "BP_FloorPortalScrollEscape_C", &config::esp::portal::escape },
		{ "Mimic", &config::esp::chest::mimic },
		{ "BP_GoldOre_N_C", &config::esp::ore::gold },
		{ "BP_RubysilverOre_N_C", &config::esp::ore::ruby_silver },
		{ "BP_CobaltOre_N_C", &config::esp::ore::cobalt },
		{ "Champion", &config::esp::ai::minibosses },
		{ "DemonBerserker", &config::esp::ai::minibosses },
		{ "Cockatrice", &config::esp::ai::minibosses },
		{ "CentaurDemon", &config::esp::ai::minibosses },
		{ "Mage", &config::esp::ai::minibosses },
		{ "Wraith", &config::esp::ai::minibosses },
		{ "Skeleton", &config::esp::ai::skeleton },
		{ "Zombie", &config::esp::ai::zombie },
		{ "Goblin", &config::esp::ai::goblin },
		{ "DeathSkull", &config::esp::ai::skull },
		{ "Mummy", &config::esp::ai::mummy },
		{ "Spider", &config::esp::ai::spider }
	};

	std::unordered_map<std::string, std::string> actor_friendly_names = {
		{ "BP_AltarOfSacrifice_C", "Revive Shrine" },
		{ "BP_Statue01_C", "Health Shrine" },
		{ "BP_Statue02_C", "Shield Shrine" },
		{ "BP_Statue03_C", "Power Shrine" },
		{ "BP_Statue04_C", "Speed Shrine" },
		{ "BP_GoldOre_N_C", "Gold Ore" },
		{ "BP_RubysilverOre_N_C", "Rubysilver Ore" },
		{ "BP_CobaltOre_N_C", "Cobalt Ore" },
		{ "Type.Item.Rarity.Junk", "Junk" },
		{ "Type.Item.Rarity.Poor", "Poor" },
		{ "Type.Item.Rarity.Common", "Common" },
		{ "Type.Item.Rarity.Uncommon", "Uncommon" },
		{ "Type.Item.Rarity.Epic", "Epic" },
		{ "Type.Item.Rarity.Rare", "Rare" },
		{ "Type.Item.Rarity.Legendary", "Legendary" },
		{ "Type.Item.Rarity.Unique", "Unique" },
		{ "BP_FloorPortalScrollEscape_C", "Escape Portal" },
		{ "BP_FloorPortalScrollDown_C", "Down Portal" },
		{ "BP_FloorPortalScrollDown_C", "Down Portal" },
		{ "BP_FloorPortalScrollDown_C", "Down Portal" },
		{ "SimpleChestSmall", "Small Oak Chest" },
		{ "SimpleChestMedium", "Medium Oak Chest" },
		{ "SimpleChestLarge", "Large Wood Chest" },
		{ "WoodChestMedium", "Medium Wood Chest" },
		{ "WoodChestSmall", "Small Wood Chest" },
		{ "WoodChestLarge", "Large Wood Chest" },
		{ "OrnateChestSmall", "Small Ornate Chest" },
		{ "OrnateChestMedium", "Medium Ornate Chest" },
		{ "OrnateChestLarge", "Lions Head Chest" },
		{ "FlatChestLarge", "Large Flat Chest" },
		{ "GoldChest", "Gold Chest" },
		{ "Coffin", "Coffin" },
		{ "SpiderMummy", "Spider" },
		{ "Mimic", "Mimic" },
	};
	
	std::string get_friendly_name(std::string actor_name) {
		auto entry = esp::actor_friendly_names.find(actor_name);
		if (entry != esp::actor_friendly_names.end()) {
			return entry->second;
		}

		for (auto& [key, value] : esp::actor_friendly_names) {
			if (actor_name.find(key) != std::string::npos) {
				return value;
			}
		}

		actor_name = util::string_replace(actor_name, "BP_", "");
		actor_name = util::string_replace(actor_name, "SK_", "");
		actor_name = util::string_replace(actor_name, "_Common_C", "");
		actor_name = util::string_replace(actor_name, "_C", "");
		actor_name = util::string_replace(actor_name, "_", " ");
		actor_name = util::string_replace(actor_name, "Id.Item.", "");

		return actor_name;
	}


	static bool is_actor_filtered(std::string actor_name, bool* was_found = 0) {
		auto set_was_found = [was_found](bool set) {
			if (was_found != 0) {
				*was_found = set;
			}
		};

		auto entry = esp::actor_filter_map.find(actor_name);
		if (entry != esp::actor_filter_map.end()) {
			set_was_found(true);
			return *entry->second;
		}

		for (auto& [key, value] : esp::actor_filter_map) {
			if (actor_name.find(key) != std::string::npos) {
				set_was_found(true);
				return *value;
			}
		}

		set_was_found(false);

		return false;
	}

	static color_m get_actor_color(std::string name) {
		auto entry = esp::actor_color_map.find(name);
		if (entry != esp::actor_color_map.end()) return *entry->second;

		for (auto& [key, value] : esp::actor_color_map) {
			if (name.find(key) != std::string::npos) return *value;
		}

		return color_m::white();
	}

	namespace loot {
		int esp_stack = 0;
		void draw_name(std::string name, vector2 screen_location, color_m color) {
			cvar::overlay->draw_text(screen_location, ImColor(color), name.c_str(), true, 13.f);
			esp_stack++;
		}
		void draw_distance(double distance, vector2 screen_location, color_m color) {
			cvar::overlay->draw_text(vector2{ screen_location.x, screen_location.y + (12.f * esp_stack) }, ImColor(color), std::format("{0:.1f}m", distance).c_str(), true, 13.f);
			esp_stack++;
		}
		void loot_esp(f_camera_cache camera_cache) {
			vector3 local_position = cvar::local_root_cmp->get_relative_location();

			for (abp_static_mesh_item_holder_c* ground_loot : cvar::ground_loot) {
				if (!ground_loot) continue;

				u_art_data_item* artitem = ground_loot->art_data_item();
				if (!artitem) continue;

				/*u_item* item = ground_loot->item_object();
				std::cout << std::hex << (uintptr_t)item << std::dec << std::endl;
				if (!item) continue;
				
				auto [item_name, rarity_name] = util::get_item_name_rarity(item);
				auto rarity = util::get_rarity_from_name(rarity_name);
				if ((int)rarity > config::esp::loot::rarity_level) continue;*/

				u_scene_component* root_cmp = ground_loot->root_component();
				if (!root_cmp) continue;

				vector3 position = root_cmp->get_relative_location();
				double distance = ((position - local_position) / 100).magnitude();

				if (distance > config::esp::loot::render_distance) continue;

				vector3 screen_location{};
				if (util::w2s(position, camera_cache.pov, screen_location)) {
					std::string loot_actor_name = util::actor_name(ground_loot);
					std::string loot_sk_name = util::get_name_from_fname(artitem->item_static_mesh()->fname_index());
					std::string friendly_loot_name = get_friendly_name(loot_sk_name);
					friendly_loot_name = friendly_loot_name == "None" ? "WorldLoot" : friendly_loot_name;
					color_m loot_color = get_actor_color(loot_actor_name);

					if (config::esp::loot::name)
						draw_name(friendly_loot_name, screen_location, loot_color);
					if (config::esp::loot::distance)
						draw_distance(distance, screen_location, color_m::white());
				}

				esp_stack = 0;
			}
		}
	}

	namespace ai {
		int esp_stack = 0;
		void draw_name(std::string name, vector2 screen_location, color_m color) {
			cvar::overlay->draw_text(screen_location, ImColor(color), name.c_str(), true);
			esp_stack++;
		}
		void draw_distance(double distance, vector2 screen_location, color_m color) {
			cvar::overlay->draw_text(vector2{ screen_location.x, screen_location.y + (12.f * esp_stack) }, ImColor(color), std::format("{0:.1f}m", distance).c_str(), true);
			esp_stack++;
		}
		void draw_health(float health, float max_health, vector2 screen_location) {
			cvar::overlay->draw_text(vector2{ screen_location.x, screen_location.y + (12.f * esp_stack) }, ImColor(util::get_hp_color(health, max_health)), std::format("{}% ({}/{})", static_cast<int>(health / max_health * 100), static_cast<int>(health), static_cast<int>(max_health)).c_str(), true);
			esp_stack++;
		}
		void ai_esp(f_camera_cache camera_cache) {
			vector3 local_position = cvar::local_root_cmp->get_relative_location();

			for (adc_monster_base* monster : cvar::monsters) {
				if (!monster) continue;

				u_scene_component* root_cmp = monster->root_component();
				if (!root_cmp) continue;

				vector3 position = root_cmp->get_relative_location();

				double distance = ((position - local_position) / 100).magnitude();
				if (distance > config::esp::ai::render_distance) continue;

				auto [max_health, health] = util::get_health(monster);

				if (health <= 0.f) continue;

				vector3 screen_location{};

				if (util::w2s(position, camera_cache.pov, screen_location)) {
					std::string monster_name = util::actor_name(monster);
					std::string friendly_monster_name = get_friendly_name(monster_name);
					if (monster_name == "") continue;

					color_m monster_color = config::esp::ai::colors::ai;

					bool was_found = false;
					if (is_actor_filtered(monster_name, &was_found)) {
						// item is found in filter list and is enabled
						if (config::esp::ai::name)
							draw_name(friendly_monster_name, screen_location, monster_color);
						if (config::esp::ai::distance)
							draw_distance(distance, screen_location, color_m::white());
						if (config::esp::ai::health)
							draw_health(health, max_health, screen_location);

						if (false)
						{
							if (util::is_a(monster, "ABP_SkeletonSpearman_C") || util::is_a(monster, "ABP_SkeletonMage_Common_C"))
							{
								auto skeleton = (abp_skeletonspearman_c*)monster;
								auto hitboxes = skeleton->get_all_hitboxes();
								for (auto hitbox : hitboxes)
								{
									hitbox->set_relative_scale(vector3(5, 5, 5));
								}
							}

							else if (util::is_a(monster, "ABP_SkeletonGuardmanFromFakeDeath_Common_C") || util::is_a(monster, "ABP_SkeletonGuardman_Summoned_C") || util::is_a(monster, "ABP_SkeletonGuardman_Common_C") || util::is_a(monster, "ABP_SkeletonArcher_Common_C") || util::is_a(monster, "ABP_SkeletonArcher_Elite_C") || util::is_a(monster, "ABP_SkeletonArcher_Nightmare_C"))
							{
								auto skeleton = (abp_skeletonguardman*)monster;
								auto hitbox = skeleton->head_hitbox();
								hitbox->set_relative_scale(vector3(5, 5, 5));
							}

							else if (util::is_a(monster, "ABP_SkeletonSwordman_C") || util::is_a(monster, "ABP_SkeletonSwordman_Elite_C") || util::is_a(monster, "ABP_SkeletonSwordman_Nightmare_C") || util::is_a(monster, "ABP_SkeletonFootman_Common_C") || util::is_a(monster, "ABP_SkeletonFootman_Elite_C") || util::is_a(monster, "ABP_SkeletonFootman_Nightmare_C"))
							{
								auto skeleton = (abp_skeletonswordman*)monster;
								auto hitbox = skeleton->head_hitbox();
								hitbox->set_relative_scale(vector3(5, 5, 5));
							}

							else if (util::is_a(monster, "ABP_SpiderMummy_Common_C") || util::is_a(monster, "ABP_SpiderMummy_Elite_C") || util::is_a(monster, "ABP_SpiderMummy_Nightmare_C"))
							{
								auto skeleton = (abp_spidermummy*)monster;
								auto hitboxes = skeleton->get_all_hitboxes();
								for (auto hitbox : hitboxes)
								{
									hitbox->set_relative_scale(vector3(5, 5, 5));
								}
							}

							else if (util::is_a(monster, "ABP_DeathBeetle_Common_C") || util::is_a(monster, "ABP_DeathBeetle_Elite_C") || util::is_a(monster, "ABP_DeathBeetle_Nightmare_C"))
							{
								auto skeleton = (abp_deathbeetle*)monster;
								auto hitboxes = skeleton->get_all_hitboxes();
								for (auto hitbox : hitboxes)
								{
									hitbox->set_relative_scale(vector3(5, 5, 5));
								}
							}

							else if (util::is_a(monster, "ABP_DeathSkull_Common_C") || util::is_a(monster, "ABP_DeathSkull_Elite_C") || util::is_a(monster, "ABP_DeathSkull_Nightmare_C"))
							{
								auto skeleton = (abp_deathbeetle*)monster;
								auto hitboxes = skeleton->get_all_hitboxes();
								for (auto hitbox : hitboxes)
								{
									hitbox->set_relative_scale(vector3(5, 5, 5));
								}
							}

							else if (util::is_a(monster, "ABP_Zombie_Common_C") || util::is_a(monster, "ABP_Zombie_Elite_C") || util::is_a(monster, "ABP_Zombie_Nightmare_C"))
							{
								auto skeleton = (abp_zombie*)monster;
								auto hitbox = skeleton->head_hitbox();
								hitbox->set_relative_scale(vector3(5, 5, 5));
							}
						}
					}
					else if (!was_found) {
						// not found in filter list
						if (config::esp::ai::other) {
							if (config::esp::ai::name)
								draw_name(friendly_monster_name, screen_location, monster_color);
							if (config::esp::ai::distance)
								draw_distance(distance, screen_location, color_m::white());
							if (config::esp::ai::health)
								draw_health(health, max_health, screen_location);
						}
					}

					esp_stack = 0;
				}
			}
		}
	}
	namespace shrines {
		void draw_name(std::string name, vector2 screen_location, color_m color) {
			cvar::overlay->draw_text(screen_location, ImColor(color), name.c_str(), true);
		}
		void draw_distance(double distance, vector2 screen_location, color_m color) {
			cvar::overlay->draw_text(vector2{ screen_location.x, screen_location.y + 12.f }, ImColor(color), std::format("{0:.1f}m", distance).c_str(), true);
		}
		void shrine_esp(f_camera_cache camera_cache) {
			vector3 local_position = cvar::local_root_cmp->get_relative_location();

			for (actor* shrine_actor : cvar::shrines) {
				if (!shrine_actor) continue;

				u_scene_component* root_cmp = shrine_actor->root_component();
				if (!root_cmp) continue;

				vector3 position = root_cmp->get_relative_location();
				double distance = ((position - local_position) / 100).magnitude();

				vector3 screen_location{};

				if (util::w2s(position, camera_cache.pov, screen_location)) {
					std::string shrine_name = util::actor_name(shrine_actor);
					std::string friendly_name = get_friendly_name(shrine_name);

					if (friendly_name == "") continue;

					if (!is_actor_filtered(shrine_name)) continue;

					color_m shrine_color = get_actor_color(shrine_name);

					if (config::esp::shrine::name)
						draw_name(friendly_name, screen_location, shrine_color);
					if (config::esp::shrine::distance)
						draw_distance(distance, screen_location, color_m::white());
				}
			}
		}
	}

	namespace players {
		int esp_stack = 0;
		namespace game_status {
			void draw_game_status() {
				int count_down = 0;
				int count_alive = 0;
				int count_escaped = 0;
				int count_dead = 0;

				ImColor alive = ImColor(0, 255, 0);
				ImColor dead = ImColor(255, 0, 0);
				ImColor escaped = ImColor(100, 100, 255);
				ImColor down = ImColor(255, 100, 100);

				std::map<int, std::vector<f_account_data_replication>> parties = util::get_parties(cvar::lobby_players);

				for (auto& party : parties) {
					for (auto& player : party.second) {
						player.alive ? player.down ? count_down++ : player.escape ? count_escaped++ : count_alive++ : count_dead++;
					}
				}

				vector2 draw_location = util::screen_size();
				draw_location.x -= 300;
				draw_location.y -= 400;

				ImGui::SetNextWindowBgAlpha(0.2f);
				ImGui::SetNextWindowPos(ImVec2(static_cast<float>(draw_location.x), static_cast<float>(draw_location.y)), ImGuiCond_Once);
				ImGui::SetNextWindowSize(ImVec2(278, 55), ImGuiCond_Once);
				ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize;
				if (!config::menu::is_open) flags |= ImGuiWindowFlags_NoMove;
				ImGui::Begin("Game Status", 0, flags);

				// alive count
				ImGui::TextColored(alive, "Alive: %i", count_alive); ImGui::SameLine();
				// dead count
				ImGui::TextColored(dead, "Dead: %i", count_dead); ImGui::SameLine();
				// escaped count
				ImGui::TextColored(escaped, "Escaped: %i", count_escaped); ImGui::SameLine();
				// down count
				ImGui::TextColored(down, "Down: %i", count_down);

				ImGui::End();

			}
		}
		namespace lobby {
	
			void draw_window() {
				if (GetAsyncKeyState(config::esp::players::lobby::toggle_key) & 1) {
					config::esp::players::lobby::show = !config::esp::players::lobby::show;
				}

				if (!config::esp::players::lobby::show) return;

				ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Once);
				ImGui::SetNextWindowBgAlpha(config::esp::players::lobby::transparency);

				auto should_move = config::menu::is_open ? 0 : ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

				ImGui::Begin(
					"Lobby", 
					0, 
					ImGuiWindowFlags_NoCollapse | 
					ImGuiWindowFlags_NoScrollbar | 
					ImGuiWindowFlags_NoScrollWithMouse |
					should_move
				);

				std::map<int, std::vector<f_account_data_replication>> parties = util::get_parties(cvar::lobby_players);

				ImColor alive = ImColor(0, 255, 0);
				ImColor dead = ImColor(255, 0, 0);
				ImColor escaped = ImColor(100, 100, 255);
				ImColor down = ImColor(255, 100, 100);
				ImColor spectating = ImColor(235, 82, 8);

				auto get_color = [alive, dead, escaped, down, spectating](f_account_data_replication player) -> ImColor {
					// use ternary if alive and not escaped or down then green else red
					/*return 
						player.alive ?
							player.down ? 
								down : player.escape ?
									escaped : alive : dead;*/

					if (player.alive)
					{
						if (player.down)
							return down;
						return alive;
					}
					else if ((!player.alive || player.escape) && !player.exit)
					{
						return spectating;
					}
					else if (player.exit)
					{
						return escaped;
					}
				};

				auto draw_player_row = [get_color](f_account_data_replication player) {
					if (config::esp::players::lobby::level) {
						ImGui::TableNextColumn();
						ImGui::TextColored(get_color(player), "%i", player.level);
					}
					if (config::esp::players::lobby::class_name) {
						auto class_name = util::read_fstring(player.nickname.streaming_nickname);
						class_name = class_name.substr(0, class_name.find_last_of("#"));
						ImGui::TableNextColumn();
						ImGui::TextColored(get_color(player), "%s", class_name.c_str());
					}
					if (config::esp::players::lobby::name) {
						auto player_name = util::read_fstring(player.nickname.original_nickname);
						ImGui::TableNextColumn();
						ImGui::TextColored(get_color(player), "%s", player_name.c_str());
					}
					if (config::esp::players::lobby::party_id) {
						ImGui::TableNextColumn();
						ImGui::TextColored(get_color(player), "%s", util::read_fstring(player.party_id).c_str());
					}
				};
				
				if (ImGui::BeginTable("PlayerTable", 4, ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersInnerH| ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
				{
					if (config::esp::players::lobby::level)
						ImGui::TableSetupColumn("Level", ImGuiTableColumnFlags_WidthFixed);
					if (config::esp::players::lobby::class_name)
						ImGui::TableSetupColumn("Class", ImGuiTableColumnFlags_WidthFixed);
					if (config::esp::players::lobby::name)
						ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
					if (config::esp::players::lobby::party_id)
						ImGui::TableSetupColumn("Party ID", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableHeadersRow();
					for (auto& party : parties) {
						if (party.first == 0) {

							for (auto& player : party.second) {
								draw_player_row(player);
								ImGui::TableNextRow();
								continue;
							}
		
						}
						else {
							for (auto& player : party.second) {
								draw_player_row(player);
								ImGui::TableNextRow();
							}
						}
						
					}
					ImGui::EndTable();
				}

				// legend
				ImGui::TextColored(alive, "Alive"); ImGui::SameLine();
				ImGui::TextColored(dead, "Dead"); ImGui::SameLine();
				ImGui::TextColored(spectating, "Spectating"); ImGui::SameLine();
				ImGui::TextColored(escaped, "Left Match"); ImGui::SameLine();
				ImGui::TextColored(down, "Down");

				ImGui::End();
			}
		}
		
		void draw_skeleton(a_pawn* pawn, f_camera_cache camera_cache, f_transform c2w, u_skeletal_mesh_component* skeletal_mesh_comp) {
			for (auto skeleton_pair : sdk::socket_skeleton) {
				auto [first_bone, second_bone] = skeleton_pair;

				vector3 first_bone_screen{};
				vector3 second_bone_screen{};
				vector3 first_bone_location = util::get_world_position(c2w, skeletal_mesh_comp->get_bone(first_bone));
				vector3 second_bone_location = util::get_world_position(c2w, skeletal_mesh_comp->get_bone(second_bone));

				if (util::w2s(first_bone_location, camera_cache.pov, first_bone_screen) &&
					util::w2s(second_bone_location, camera_cache.pov, second_bone_screen)) {
					auto is_target = pawn == cvar::target;
					
					auto color = util::is_friendly(pawn) ? config::esp::players::colors::friendly 
						: is_target ? config::esp::players::colors::target 
						: config::esp::players::colors::skeleton;

					cvar::overlay->draw_list->AddLine(first_bone_screen, second_bone_screen, ImColor(color));
				}
			}
		}
		
		void draw_box(f_camera_cache camera_cache, f_transform c2w, u_skeletal_mesh_component* skeletal_mesh_comp) {
			
			auto head_position = util::get_world_position(c2w, skeletal_mesh_comp->get_bone(player_bone::head));
			auto left_foot_position = util::get_world_position(c2w, skeletal_mesh_comp->get_bone(player_bone::l_foot));
			auto right_foot_position = util::get_world_position(c2w, skeletal_mesh_comp->get_bone(player_bone::r_foot));

			vector3 head_screen_position{};
			vector3 left_foot_screen_position{};
			vector3 right_foot_screen_position{};

			vector2 top_left{ 0.f, 0.f };
			vector2 bottom_right{ 0.f, 0.f };

			if (util::w2s(head_position, camera_cache.pov, head_screen_position) &&
				util::w2s(left_foot_position, camera_cache.pov, left_foot_screen_position) && 
				util::w2s(right_foot_position, camera_cache.pov, right_foot_screen_position)) {
				double width = right_foot_screen_position.x - left_foot_screen_position.x;

				width = width < 35.f ? 35.f : width;

				top_left.x = head_screen_position.x - width / 2;
				top_left.y = head_screen_position.y;

				bottom_right.x = head_screen_position.x + width / 2;
				bottom_right.y = left_foot_screen_position.y;


				cvar::overlay->draw_rect(top_left, bottom_right, ImColor(config::esp::players::colors::box), true);

				
			}
			
		}
		
		void draw_wearables(u_equipment_inv_comp* equipment_inventory, vector2 screen_location) {
			auto equip_slot_map = equipment_inventory->equip_slot_map().map();

			for (const auto& [slot_index, item_information] : equip_slot_map) {
			 	sdk::u_item* item = item_information->item();
				if (!item) continue;

				if (config::esp::players::utility_items) {
					if (slot_index == sdk::edc_equipment_slot_index::armor_back ||
						slot_index == sdk::edc_equipment_slot_index::accessory_ring1 ||
						slot_index == sdk::edc_equipment_slot_index::accessory_ring2 ||
						slot_index == sdk::edc_equipment_slot_index::accessory_necklace)
					{
						sdk::u_item* item = item_information->item();
						if (!item) continue;

						auto [item_name, rarity_name] = util::get_item_name_rarity(item);
						item_name = get_friendly_name(item_name);

						auto pcolor = sdk::rarity_colors[rarity_name];
						if (!pcolor)
						{
							std::cout << "Failed to get rarity color for rarity: " << rarity_name << std::endl;
							continue;
						}

						cvar::overlay->draw_text(vector2{ screen_location.x + 12.f, screen_location.y + (esp_stack * 12.f) }, ImColor(*pcolor), item_name.c_str(), true);
						esp_stack++;
					}
				}
				if (config::esp::players::armor_items) {
					if (slot_index == sdk::edc_equipment_slot_index::armor_chest ||
						slot_index == sdk::edc_equipment_slot_index::armor_head ||
						slot_index == sdk::edc_equipment_slot_index::armor_legs ||
						slot_index == sdk::edc_equipment_slot_index::armor_foot ||
						slot_index == sdk::edc_equipment_slot_index::armor_hands)
					{
						sdk::u_item* item = item_information->item();
						if (!item) continue;

						auto [item_name, rarity_name] = util::get_item_name_rarity(item);
						item_name = get_friendly_name(item_name);

						auto pcolor = sdk::rarity_colors[rarity_name];
						if (!pcolor)
						{
							std::cout << "Failed to get rarity color for rarity: " << rarity_name << std::endl;
							continue;
						}

						cvar::overlay->draw_text(vector2{ screen_location.x + 12.f, screen_location.y + (esp_stack * 12.f) }, ImColor(*pcolor), item_name.c_str(), true);
						esp_stack++;
					}
				}
			}
		}
		
		void draw_equipped(u_equipment_inv_comp* equipment_inventory, vector2 screen_location) {
			t_array<a_item_actor*> equipped_items = equipment_inventory->equipped_item_actors();
			for (a_item_actor* item : equipped_items.list()) {
				if (!item) continue;

				auto [item_name, rarity_name] = util::get_item_name_rarity(item);
				item_name = get_friendly_name(item_name);

				auto pcolor = sdk::rarity_colors[rarity_name];
				if (!pcolor)
				{
					std::cout << "Failed to get rarity color for rarity: " << rarity_name << std::endl;
					continue;
				}

				cvar::overlay->draw_text(vector2{ screen_location.x + 12.f, screen_location.y + (esp_stack * 12.f) }, ImColor(*pcolor), item_name.c_str(), true);
				
				
				esp_stack++;	
			}
		}
		
		void draw_sheathed(u_equipment_inv_comp* equipment_inventory, vector2 screen_location) {
			t_array<a_item_actor*> sheathed_items = equipment_inventory->sheath_item_actors();
			for (a_item_actor* item : sheathed_items.list()) {
				if (!item) continue;

				auto [item_name, rarity_name] = util::get_item_name_rarity(item);
				item_name = get_friendly_name(item_name);
				
				cvar::overlay->draw_text(vector2{ screen_location.x + 24.f, screen_location.y + (esp_stack * 12.f) }, ImColor(*sdk::rarity_colors[rarity_name]), item_name.c_str(), true);
				esp_stack++;
			}
		}
		
		void draw_names(a_pawn* pawn, vector2 screen_location, bool draw_class_name = true) {
			auto [player_name, class_name] = util::get_names(pawn);

			// name is first on stack, dont add stack value to it
			cvar::overlay->draw_text(vector2{ screen_location.x + 12.f, screen_location.y }, ImColor(config::esp::players::colors::name), player_name.c_str(), true);
			esp_stack++;

			if (!draw_class_name) return;

			cvar::overlay->draw_text(vector2{ screen_location.x + 12.f, screen_location.y + (esp_stack * 12.f) }, ImColor(config::esp::players::colors::name), class_name.c_str(), true);
			esp_stack++;
		}

		void draw_distance(double distance, vector2 screen_location) {
			cvar::overlay->draw_text(vector2{ screen_location.x + 12.f, screen_location.y + (esp_stack * 12.f) }, ImColor(config::esp::players::colors::distance), std::format("{0:.1f}m", distance).c_str(), true);
			esp_stack++;
		}

		void draw_health(float health, float max_health, vector2 screen_location) {
			cvar::overlay->draw_text(vector2{ screen_location.x + 12.f, screen_location.y + (esp_stack * 12.f) }, ImColor(util::get_hp_color(health, max_health)), std::format("{}% ({}/{})", static_cast<int>(health / max_health * 100), static_cast<int>(health), static_cast<int>(max_health)).c_str(), true);
			esp_stack++;
		}

		void draw_dead_body(actor* actor, vector2 screen_location, color_m color) {
			auto [player_name, class_name] = util::get_names((a_pawn*)actor);
			std::string txt = "[";
			txt += player_name;
			txt += "][";
			txt += class_name;
			txt += "]";
			// name is first on stack, dont add stack value to it
			cvar::overlay->draw_text(vector2{ screen_location.x + 12.f, screen_location.y }, ImColor(config::esp::players::colors::name), "Dead Body", true);
			esp_stack++;

			cvar::overlay->draw_text(vector2{ screen_location.x + 12.f, screen_location.y + (esp_stack * 12.f) }, ImColor(config::esp::players::colors::name), txt.c_str(), true);
			esp_stack++;
		}

		void draw_head_hitbox(abp_player_character* pawn, f_camera_cache camera_cache) {
			if (config::esp::players::head_hitboxes) {
				ubp_dchitbox_c* hitbox = pawn->head_hitbox();
				vector3 world_location = util::get_world_position(hitbox->get_comp_to_world(), hitbox->get_transform());
				auto box_points = util::get_draw_box_points(hitbox);
				for (auto& [point1, point2] : box_points) {
					vector3 p1_s_loc{};
					vector3 p2_s_loc{};
					bool p1_on_screen = util::w2s(point1, camera_cache.pov, p1_s_loc);
					bool p2_on_screen = util::w2s(point2, camera_cache.pov, p2_s_loc);
					if (p1_on_screen || p2_on_screen)
						cvar::overlay->draw_list->AddLine(p1_s_loc, p2_s_loc, ImColor(config::esp::players::colors::hitboxes));
				}
			}
		}

		void player_esp(f_camera_cache camera_cache) {
			vector3 local_position = cvar::local_root_cmp->get_relative_location();

			for (a_player_state* player : cvar::players) {
				esp::players::esp_stack = 0; 

				if (!player) continue;

				//std::cout << "pawn" << std::endl;
				abp_player_character* pawn = (abp_player_character*)player->pawn();
				if (!pawn || pawn == cvar::local_pawn) continue;

				//std::cout << "root_cmp" << std::endl;
				u_scene_component* root_cmp = pawn->root_component();
				if (!root_cmp) continue;

				//std::cout << "skeletal_mesh_comp" << std::endl;
				u_skeletal_mesh_component* skeletal_mesh_comp = pawn->skeletal_mesh();
				if (!skeletal_mesh_comp) continue;

				//std::cout << "get_relative_location" << std::endl;
				vector3 position = root_cmp->get_relative_location();

				//std::cout << "distance" << std::endl;
				double distance = ((position - local_position) / 100).magnitude();

				//std::cout << "get_health" << std::endl;
				auto [max_health, health] = util::get_health(pawn);
				if (health <= 0.f) continue;

				//std::cout << "get_comp_to_world" << std::endl;
				f_transform c2w = skeletal_mesh_comp->get_comp_to_world();
				vector3 screen_location{};
				
				//std::cout << "draw_head_hitbox" << std::endl;
				if (config::esp::players::head_hitboxes)
					draw_head_hitbox(pawn, camera_cache);

				//std::cout << "draw_skeleton" << std::endl;
				if (config::esp::players::skeleton)
					draw_skeleton(pawn, camera_cache, c2w, skeletal_mesh_comp);

				//std::cout << "draw_box" << std::endl;
				if (config::esp::players::box)
					draw_box(camera_cache, c2w, skeletal_mesh_comp);
	
				//std::cout << "w2s" << std::endl;
				if (util::w2s(position, camera_cache.pov, screen_location)) {

					screen_location.x += 12.f;

					//std::cout << "draw_names" << std::endl;
					if (config::esp::players::name)
						draw_names(pawn, vector2(screen_location));

					//std::cout << "draw_distance" << std::endl;
					if (config::esp::players::distance)
						draw_distance(distance, vector2(screen_location));

					//std::cout << "draw_health" << std::endl;
					if (config::esp::players::health)
						draw_health(health, max_health, vector2(screen_location));

					if (config::esp::players::equipped_items || config::esp::players::sheathed_items) {
						u_equipment_inv_comp* equipment_inventory = pawn->equipment_inv_comp();
						if (!equipment_inventory) return;

						//std::cout << "draw_equipped" << std::endl;
						if (config::esp::players::equipped_items)
							draw_equipped(equipment_inventory, vector2(screen_location));

						//std::cout << "draw_wearables" << std::endl;
						if (config::esp::players::armor_items || config::esp::players::utility_items)
							draw_wearables(equipment_inventory, vector2(screen_location));

						//std::cout << "draw_sheathed" << std::endl;
						if (config::esp::players::sheathed_items)
							draw_sheathed(equipment_inventory, vector2(screen_location));
					}
				}
			}
		}

		void deadbody_esp(f_camera_cache camera_cache)
		{
			vector3 local_position = cvar::local_root_cmp->get_relative_location();
			for (actor* dead_body : cvar::dead_bodies)
			{
				if (!dead_body) continue;

				u_scene_component* root_cmp = dead_body->root_component();
				if (!root_cmp) continue;

				auto [max_health, health] = util::get_health((adc_character_base*)dead_body);
				if (health > 1.f) continue;

				vector3 position = root_cmp->get_relative_location();
				double distance = ((position - local_position) / 100).magnitude();

				vector3 screen_location{};
				if (util::w2s(position, camera_cache.pov, screen_location)) {
					draw_dead_body(dead_body, screen_location, color_m::white());
					draw_distance(distance, screen_location);
				}
			}
		}
	}
	namespace ore {
		void draw_name(std::string name, vector2 screen_location, color_m color) {
			cvar::overlay->draw_text(screen_location, ImColor(color), name.c_str(), true);
		}
		void draw_distance(double distance, vector2 screen_location, color_m color) {
			cvar::overlay->draw_text(vector2{ screen_location.x, screen_location.y + 12.f }, ImColor(color), std::format("{0:.1f}m", distance).c_str(), true);
		}
		void ore_esp(f_camera_cache camera_cache) {
			vector3 local_position = cvar::local_root_cmp->get_relative_location();

			for (actor* ore_actor : cvar::ores) {
				if (!ore_actor) continue;

				u_scene_component* root_cmp = ore_actor->root_component();
				if (!root_cmp) continue;

				vector3 position = root_cmp->get_relative_location();
				double distance = ((position - local_position) / 100).magnitude();

				if (distance > config::esp::ore::render_distance) continue;

				vector3 screen_location{};

				if (util::w2s(position, camera_cache.pov, screen_location)) {
					std::string ore_name = util::actor_name(ore_actor);
					std::string friendly_ore_name = get_friendly_name(ore_name);

					if (!is_actor_filtered(ore_name)) continue;

					color_m ore_color = get_actor_color(ore_name);

					if (config::esp::ore::name)
						draw_name(friendly_ore_name, screen_location, ore_color);
					if (config::esp::ore::distance)
						draw_distance(distance, screen_location, color_m::white());
				}
			}
		}
	}
	namespace portals {

		void draw_name(std::string name, vector2 screen_location, bool is_open, color_m color) {
			name = is_open ? name : name + " (Open)";
			cvar::overlay->draw_text(screen_location, ImColor(color), name.c_str(), true);
		}
		void draw_distance(double distance, vector2 screen_location, color_m color) {
			cvar::overlay->draw_text(vector2{ screen_location.x, screen_location.y + 12.f }, ImColor(color), std::format("{0:.1f}m", distance).c_str(), true);
		}
		void portal_esp(f_camera_cache camera_cache) {
			vector3 local_position = cvar::local_root_cmp->get_relative_location();

			for (a_floor_portal* portal_actor : cvar::portals) {
				if (!portal_actor) continue;

				auto is_open = !portal_actor->is_open();

				u_scene_component* root_cmp = portal_actor->root_component();
				if (!root_cmp) continue;

				if (portal_actor->is_used()) continue;
				//if (portal_actor->get_playportalfxtimeline__direction() != ETimelineDirection::ETimelineDirection__Forward) continue;

				vector3 position = root_cmp->get_relative_location();
				double distance = ((position - local_position) / 100).magnitude();

				vector3 screen_location{};

				if (util::w2s(position, camera_cache.pov, screen_location)) {
					std::string portal_name = util::actor_name(portal_actor);

					bool is_escape = portal_name.find("Escape") != std::string::npos;

					color_m color = portal_name.find("Escape") != std::string::npos ?
					config::esp::portal::colors::escape : config::esp::portal::colors::down;

					std::string friendly_name = get_friendly_name(portal_name);

					if (is_escape && !config::esp::portal::escape) continue;
					if (!is_escape && !config::esp::portal::down) continue;

					if (config::esp::portal::name)
						draw_name(friendly_name, screen_location, is_open, color);
					if (config::esp::portal::distance)
						draw_distance(distance, screen_location, color_m{ 255.f });
				}
			}
		}
	}
	namespace other {
		void draw_actor(actor* actor, vector2 screen_location, color_m color) {
			cvar::overlay->draw_text(screen_location, ImColor(color), util::actor_name(actor).c_str(), true);
		}
		void draw_distance(double distance, vector2 screen_location, color_m color) {
			cvar::overlay->draw_text(vector2{ screen_location.x, screen_location.y + 12.f }, ImColor(color), std::format("{0:.1f}m", distance).c_str(), true);
		}

		void other_esp(f_camera_cache camera_cache) {
			vector3 local_position = cvar::local_root_cmp->get_relative_location();

			for (actor* actor : cvar::all_actors) {
				if (!actor) continue;

				u_scene_component* root_cmp = actor->root_component();
				if (!root_cmp) continue;

				vector3 position = root_cmp->get_relative_location();
				double distance = ((position - local_position) / 100).magnitude();
				if (distance > 15.f) continue;

				vector3 screen_location{};

				if (util::w2s(position, camera_cache.pov, screen_location)) {

					draw_actor(actor, screen_location, color_m::white());
					draw_distance(distance, screen_location, color_m::white());
				}
			}
		}
	}
	namespace traps {
		void draw_name(std::string name, vector2 screen_location, color_m color) {
			cvar::overlay->draw_text(screen_location, ImColor(color), name.c_str(), true);
		}
		void draw_distance(double distance, vector2 screen_location, color_m color) {
			cvar::overlay->draw_text(vector2{ screen_location.x, screen_location.y + 12.f }, ImColor(color), std::format("{0:.1f}m", distance).c_str(), true);
		}
		void trap_esp(f_camera_cache camera_cache) {
			vector3 local_position = cvar::local_root_cmp->get_relative_location();

			for (actor* trap_actor : cvar::traps) {
				if (!trap_actor) continue;

				u_scene_component* root_cmp = trap_actor->root_component();
				if (!root_cmp) continue;

				vector3 position = root_cmp->get_relative_location();
				double distance = ((position - local_position) / 100).magnitude();

				if (distance > config::esp::trap::render_distance) continue;

				vector3 screen_location{};

				if (util::w2s(position, camera_cache.pov, screen_location)) {
					std::string trap_actor_name = util::actor_name(trap_actor);
					std::string trap_name = trap_actor_name.substr(3, trap_actor_name.find("_", 3) - 3);

					if (!is_actor_filtered(trap_actor_name)) continue;

					color_m trap_color = get_actor_color(trap_actor_name);

					if (config::esp::trap::name)
						draw_name(trap_name, screen_location, trap_color);
					if (config::esp::trap::distance)
						draw_distance(distance, screen_location, color_m::white());

					auto floor_trap = (abp_floorspikes_c*)trap_actor;
					if (config::esp::trap::break_traps && trap_actor_name.find("FloorSpikes") != std::string::npos)
					{
						floor_trap->get_hitbox()->set_relative_scale(vector3(0, 0, 0));
						floor_trap->get_activebox()->set_relative_scale(vector3(0, 0, 0));
						floor_trap->set_canactivate(false);
						floor_trap->break_collisiondetector();
						//auto boneneckscale = cvar::local_pawn->get_bonescaleneck();
						//std::cout << "Bone Scale Neck: " << boneneckscale.x << " | " << boneneckscale.y << " | " << boneneckscale.z << std::endl;
					}

					if (config::esp::trap::show_hitbox)
					{
						ubp_dchitbox_c* hitbox = floor_trap->get_hitbox();
						vector3 world_location = util::get_world_position(hitbox->get_comp_to_world(), hitbox->get_transform());
						auto box_points = util::get_draw_box_points(hitbox);
						for (auto& [point1, point2] : box_points) {
							vector3 p1_s_loc{};
							vector3 p2_s_loc{};
							bool p1_on_screen = util::w2s(point1, camera_cache.pov, p1_s_loc);
							bool p2_on_screen = util::w2s(point2, camera_cache.pov, p2_s_loc);
							if (p1_on_screen || p2_on_screen)
								cvar::overlay->draw_list->AddLine(p1_s_loc, p2_s_loc, ImColor(config::esp::players::colors::hitboxes));
						}
					}
				}
			}
		}
	}
	namespace chests {
		void draw_name(std::string name, vector2 screen_location, color_m color) {
			cvar::overlay->draw_text(screen_location, ImColor(color), name.c_str(), true);
		}
		void draw_distance(double distance, vector2 screen_location, color_m color) {
			cvar::overlay->draw_text(vector2{ screen_location.x, screen_location.y + 12.f }, ImColor(color), std::format("{0:.1f}m", distance).c_str(), true);
		}
		void chest_esp(f_camera_cache camera_cache) {
			vector3 local_position = cvar::local_root_cmp->get_relative_location();

			for (actor* chest_actor : cvar::chests) {
				if (!chest_actor) continue;

				u_scene_component* root_cmp = chest_actor->root_component();
				if (!root_cmp) continue;

				vector3 position = root_cmp->get_relative_location();
				double distance = ((position - local_position) / 100).magnitude();
				if (distance > config::esp::chest::render_distance) continue;

				vector3 screen_location{};

				if (util::w2s(position, camera_cache.pov, screen_location)) {
					std::string chest_actor_name = util::actor_name(chest_actor);
					std::string friendly_chest_name = get_friendly_name(chest_actor_name);
					bool was_found = false;
					if (is_actor_filtered(chest_actor_name, &was_found)) {
						// item is found in filter list and is enabled
						color_m chest_color = get_actor_color(chest_actor_name);
						if (config::esp::chest::name)
							draw_name(friendly_chest_name, screen_location, chest_color);
						if (config::esp::chest::distance)
							draw_distance(distance, screen_location, color_m::white());
					}
					else if (!was_found) {
						// not found in filter list
						if (config::esp::chest::other) {
							// draw if other esp is enabled and item is not in filter list (not found)
							color_m chest_color = get_actor_color(chest_actor_name);
							if (config::esp::chest::name)
								draw_name(friendly_chest_name, screen_location, chest_color);
							if (config::esp::chest::distance)
								draw_distance(distance, screen_location, color_m::white());
						}
					}
				}
			}
		}
	}
	namespace aimbot {
		void draw_fov_circle(float fov) {
			if (config::combat::aim_type != combat::aimbot_type::by_world_distance)
			cvar::overlay->draw_list->AddCircle(
				vector2{ cvar::overlay->window_width / 2, cvar::overlay->window_height / 2 },
				fov,
				ImColor(color_m::white()),
				64,
				1.f
			);
		}
	}
}

void esp::draw()
{
	if (!cvar::local_pawn)
		return;

	//std::cout << "camera_cache" << std::endl;
	cvar::camera_cache = cvar::local_camera_manager->last_frame_cam_cache_private();

	//std::cout << "player_esp" << std::endl;
	if (config::esp::players::enabled) 
		esp::players::player_esp(cvar::camera_cache);

	if (config::esp::players::dead_bodies)
		esp::players::deadbody_esp(cvar::camera_cache);

	//std::cout << "chest_esp" << std::endl;
	if (config::esp::chest::enabled) 
		esp::chests::chest_esp(cvar::camera_cache);

	//std::cout << "portal_esp" << std::endl;
	if (config::esp::portal::enabled) 
		esp::portals::portal_esp(cvar::camera_cache);

	//std::cout << "other_esp" << std::endl;
	if (config::esp::other::actors)
		esp::other::other_esp(cvar::camera_cache);

	//std::cout << "shrine_esp" << std::endl;
	if (config::esp::shrine::enabled)
		esp::shrines::shrine_esp(cvar::camera_cache);	

	//std::cout << "ore_esp" << std::endl;
	if (config::esp::ore::enabled)
		esp::ore::ore_esp(cvar::camera_cache);

	//std::cout << "trap_esp" << std::endl;
	if (config::esp::trap::enabled)
		esp::traps::trap_esp(cvar::camera_cache);

	//std::cout << "draw_fov_circle" << std::endl;
	if (config::combat::aimbot)
		esp::aimbot::draw_fov_circle(config::combat::fov);

	//std::cout << "draw_window" << std::endl;
	if (config::esp::players::lobby::enabled)
		esp::players::lobby::draw_window();

	//std::cout << "draw_game_status" << std::endl;
	if (config::esp::players::game_status::enabled)
		esp::players::game_status::draw_game_status();

	//std::cout << "ai_esp" << std::endl;
	if (config::esp::ai::enabled)
		esp::ai::ai_esp(cvar::camera_cache);

	//std::cout << "loot_esp" << std::endl;
	if (config::esp::loot::enabled)
		esp::loot::loot_esp(cvar::camera_cache);

	if (config::menu::fullbright && cvar::game_viewport)
	{
		cvar::game_viewport->set_viewmode_index(0x1);
		config::menu::fixed_fullbright = false;
	}
	else if (!config::menu::fixed_fullbright)
	{
		cvar::game_viewport->set_viewmode_index(0x3);
		config::menu::fixed_fullbright = true;
	}

	if (config::menu::testing)
	{
		vector3 local_position = cvar::local_root_cmp->get_relative_location();

		for (actor* ore_actor : cvar::projectiles) {
			if (!ore_actor) continue;

			u_scene_component* root_cmp = ore_actor->root_component();
			if (!root_cmp) continue;

			vector3 position = root_cmp->get_relative_location();
			vector3 screen_loc;
			if (util::w2s(position, cvar::camera_cache.pov, screen_loc))
				cvar::overlay->draw_text(screen_loc, ImColor(1, 1, 1), "Arrow", true);
			//root_cmp->set_relative_location(local_position + vector3(0, 0, 1));
		}
	}
}
