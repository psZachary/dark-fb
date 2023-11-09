#pragma once
#include <cstdint>
#include "memory.h"
#include "mathh.h"
#include "offsets.h"
#include "config.h"
#include <map>
#include <string>
#include <vector>

using namespace mem;

namespace sdk {
	constexpr const char* WINDOW = "Dark and Darker  ";
	constexpr const char* WINDOW_CLASS = "UnrealWindow";
	constexpr const char* MODULE_NAME = "DungeonCrawler.exe";
	constexpr const wchar_t* MODULE_NAME_WSTR = L"DungeonCrawler.exe";

	inline std::map<std::string, color_m*> rarity_colors{
		{ "Type.Item.Rarity.Junk", &config::colors::color_junk },
		{ "Type.Item.Rarity.Poor", &config::colors::color_poor },
		{ "Type.Item.Rarity.Common", &config::colors::color_common },
		{ "Type.Item.Rarity.Uncommon", &config::colors::color_uncommon },
		{ "Type.Item.Rarity.Epic", &config::colors::color_epic },
		{ "Type.Item.Rarity.Rare", &config::colors::color_rare },
		{ "Type.Item.Rarity.Legendary", &config::colors::color_legendary },
		{ "Type.Item.Rarity.Unique", &config::colors::color_legendary }
	};

	struct fname {
		int32_t index;
		int32_t number;
	};
	struct f_minimal_view_info
	{
		vector3 location;
		vector3 rotation;
		float fov;
	};
	struct f_camera_cache
	{
		char pad_0x0[0x10];
		f_minimal_view_info pov;
	};

	class u_object {
	public:
		int fname_index() {
			return rpm<int>((uintptr_t)this + 0x18);
		}
		u_object* super() {
			// 0x30
			return rpm<u_object*>((uintptr_t)this + 0x40);
		}
		u_object* class_private() {
			return rpm<u_object*>((uintptr_t)this + 0x10);
		}
		u_object* outer() {
			return rpm<u_object*>((uintptr_t)this + 0x20);
		}
	};

	struct f_bone_node {
		fname name;
		int32_t parent_idx;
		char _pad[0x4];
	};

	template <typename T>
	class t_array {
	public:
		uintptr_t _data;
		int32_t count;
		int32_t _max;

		T data() {
			return rpm<T>(_data + 0x0);
		}

		T at(int32_t index) {
			return rpm<T>(_data + (index * sizeof(T)));
		}
		
		std::vector<T> list() {
			std::vector<T> list{};

			if (!count || count <= 0) return list;

			T* buffer = new T[count];
			read_raw(_data, buffer, count * sizeof(T));
			for (int i = 0; i < count; i++) {
				T entry = buffer[i];

				list.push_back(entry);
			}
			delete[] buffer;
			return list;
		}
	};
	template<typename KT, typename VT>
	class t_pair
	{
	public:
		KT first;
		VT second;
	};

	template<typename ELT>
	class t_set_element
	{
	public:
		ELT value;                                             
		int32_t hash_next_id;                                          
		int32_t hash_index;                                           
	};
	template <typename KY, typename V>
	class t_map {
	public:
		t_array<t_set_element<t_pair<KY, V>>> elements;

		std::map<KY, V> map() {
			std::vector<t_set_element<t_pair<KY, V>>> elements = this->elements.list();
			std::map<KY, V> map{};
			for (const auto& element : elements) {
				map[element.value.first] = element.value.second;
			}
			return map;
		}
	};

	struct f_transform {
		quat rotation;
		vector3 translation;
		char pad38[0x8];
		vector3 scale3d;

		matrix4x4_t to_matrix_with_scale()
		{
			matrix4x4_t m;
			m._41 = translation.x;
			m._42 = translation.y;
			m._43 = translation.z;

			double x2 = rotation.x + rotation.x;
			double y2 = rotation.y + rotation.y;
			double z2 = rotation.z + rotation.z;

			double xx2 = rotation.x * x2;
			double yy2 = rotation.y * y2;
			double zz2 = rotation.z * z2;
			m._11 = (1.0f - (yy2 + zz2)) * scale3d.x;
			m._22 = (1.0f - (xx2 + zz2)) * scale3d.y;
			m._33 = (1.0f - (xx2 + yy2)) * scale3d.z;

			double yz2 = rotation.y * z2;
			double wx2 = rotation.w * x2;
			m._32 = (yz2 - wx2) * scale3d.z;
			m._23 = (yz2 + wx2) * scale3d.y;

			double xy2 = rotation.x * y2;
			double wz2 = rotation.w * z2;
			m._21 = (xy2 - wz2) * scale3d.y;
			m._12 = (xy2 + wz2) * scale3d.x;

			double xz2 = rotation.x * z2;
			double wy2 = rotation.w * y2;
			m._31 = (xz2 + wy2) * scale3d.z;
			m._13 = (xz2 - wy2) * scale3d.x;

			m._14 = 0.0f;
			m._24 = 0.0f;
			m._34 = 0.0f;
			m._44 = 1.0f;

			return m;
		}
	};

	enum player_bone : int {
		upper_spine = 8,
		mid_spine = 6,
		lower_spine = 4,
		left_eye = 62,
		right_eye = 60,
		head = 156,
		jaw = 58,
		r_knee = 130,
		l_knee = 118,
		r_foot = 128,
		l_foot = 116,
		r_pelvis = 132,
		l_pelvis = 120,
		r_shoulder = 108, 
		l_shoulder = 54,
		neck = 56,
		r_elbow = 70,
		l_elbow = 52,
		r_hand = 72,
		l_hand = 50,
	};

	inline std::vector<std::pair< player_bone, player_bone >> socket_skeleton = {
		{ player_bone::upper_spine, player_bone::r_shoulder },
		{ player_bone::upper_spine, player_bone::l_shoulder },
		{ player_bone::upper_spine, player_bone::neck },
		{ player_bone::neck, player_bone::head },
		{ player_bone::upper_spine, player_bone::mid_spine },
		{ player_bone::mid_spine, player_bone::lower_spine },
		{ player_bone::lower_spine, player_bone::r_pelvis },
		{ player_bone::lower_spine, player_bone::l_pelvis },
		{ player_bone::r_pelvis, player_bone::r_knee },
		{ player_bone::l_pelvis, player_bone::l_knee },
		{ player_bone::r_knee, player_bone::r_foot },
		{ player_bone::l_knee, player_bone::l_foot },
		{ player_bone::r_shoulder, player_bone::r_elbow },
		{ player_bone::l_shoulder, player_bone::l_elbow },
		{ player_bone::r_elbow, player_bone::r_hand },
		{ player_bone::l_elbow, player_bone::l_hand },
	};

	class fstring : public t_array<wchar_t> {
	public:
		wchar_t* read_string() {
			if (count <= 0 || count > 500) return 0;
			auto buffer = new wchar_t[count];
			mem::read_raw(_data, buffer, count * sizeof(wchar_t));
			return buffer;
		}
	};
	class u_scene_component : public u_object {
	public:
		vector3 get_relative_location() {
			return rpm<vector3>((uintptr_t)this + 0x128);
		}
		void set_relative_location(vector3 set) {
			wpm<vector3>((uintptr_t)this + 0x128, set);
		}
		vector3 get_velocity() {
			return rpm<vector3>((uintptr_t)this + 0x170);

		}
		f_transform get_comp_to_world() {
			return rpm<f_transform>((uintptr_t)this + 0x240);
		}
		void set_relative_scale(vector3 scale3d) {
			wpm<vector3>((uintptr_t)this + 0x158, scale3d);
		}
		vector3 get_relative_scale() {
			return rpm<vector3>((uintptr_t)this + 0x158);
		}
		f_transform get_transform() {
			f_transform t{ };
			t.scale3d = get_relative_scale();
			t.translation = get_relative_location();
			return t;
		}
	};

	class actor : public u_object {
	public:
		u_scene_component* root_component() {
			return rpm<u_scene_component*>((uintptr_t)this + 0x198);
		}
	
	};

	class u_level {
	public:
		t_array<actor*> actors() {
			return rpm<t_array<actor*>>((uintptr_t)this + 0xA8);
		}
	};


	class u_game_viewport_client {
	public:
		void set_viewmode_index(int32_t index) {
			wpm<int32_t>((uintptr_t)this + 0xB0, index);
		}
	};

	class a_controller : public actor {
	public:
		void set_control_rotation(vector3 rot) { 
			wpm<vector3>((uintptr_t)this + 0x308, rot);
		}
		void set_control_rotation(vector2 rot) {
			wpm<vector2>((uintptr_t)this + 0x308, rot);
		}
	};

	class u_loot_component : public u_scene_component {
	public:
		actor* get_loot_target_actor() {
			return rpm<actor*>((uintptr_t)this + 0x1E0);
		}
		void set_loot_target_actor(actor* value) {
			//wpm<actor*>((uintptr_t)this + 0x1E0 + 0xB8, value);
			wpm<actor*>((uintptr_t)this + 0x1E0, value);
		}
		float get_loot_distance() {
			return rpm<float>((uintptr_t)this + 0x1E0 + 0x8);
		}
		fname bone_name() {
			return rpm<fname>((uintptr_t)this + 0x1E0 + 0xE0);
		}
	};

	class a_player_camera_manager : public actor {
	public:
		f_camera_cache last_frame_cam_cache_private() {
			return rpm<f_camera_cache>((uintptr_t)this + 0x22B0);
		}
	};

	struct fdc_karma {
		int32_t rating;
		uint8_t level;
		unsigned char ukndata[0x3];
	};

	struct f_nickname {
		fstring original_nickname;
		fstring streaming_nickname;
		fdc_karma karma;
	};

	class f_account_data_replication {
	public:
		fstring account_id;
		f_nickname nickname;
		fstring player_character_id;
		fstring party_id;
		fstring character_id;
		int32_t gender;
		int32_t level;
		bool init;
		bool exit;
		bool login;
		bool alive;
		bool escape;
		bool down;
		unsigned char uk[0x2];
	};

	class f_gameplay_attribute_data {
	public:
		char pad_0x8[0x8];
		float base_value;
		float current_value;
	};

	//uabilitysystemtestattbriuteset
	class u_attribute_set {
	public:
		float get_max_health() {
			return rpm<float>((uintptr_t)this + 0x658 + 0xC);
		}
		float get_health() {
			return rpm<float>((uintptr_t)this + 0x648 + 0xC);
		}
		void set_move_speed(float value) {
			wpm<float>((uintptr_t)this + 0x750 + offsetof(f_gameplay_attribute_data, current_value), value);
		}
		float get_move_speed() {
			return rpm<float>((uintptr_t)this + 0x750 + offsetof(f_gameplay_attribute_data, current_value));
		}
		void set_action_speed(float value) {
			wpm<float>((uintptr_t)this + 0x7C0 + offsetof(f_gameplay_attribute_data, current_value), value);
		}
		float get_action_speed() {
			return rpm<float>((uintptr_t)this + 0x7C0 + offsetof(f_gameplay_attribute_data, current_value));
		}
	};

	class ability_system_component {
	public:
		t_array<u_attribute_set*> get_spawned_attributes() {
			return rpm<t_array<u_attribute_set*>>((uintptr_t)this + 0x11F0);
		}
	};

	class f_gameplay_tag : public fname {

	};

	class f_design_data_item {
	public:
		f_gameplay_tag id_tag;
		char pad_0x80[0x80];
		f_gameplay_tag rarity_type;
	};

	class a_item_actor : public actor {
	public:
		// FDesignDataItem DesignDataItem; // 0x0468   (0x0188)
		f_design_data_item design_data_item() {
			return rpm<f_design_data_item>((uintptr_t)this + 0x468);
		}
	};

	class u_item : public u_object {
	public:
		f_design_data_item design_data_item() {
			return rpm<f_design_data_item>((uintptr_t)this + 0x138);
		}
	};

	enum edc_equipment_slot_index : uint8_t
	{
		none = 0,
		armor_head = 1,
		armor_chest = 2,
		armor_hands = 3,
		armor_legs = 4,
		armor_foot = 5,
		belt_l = 6,
		belt_r = 7,
		utility_l = 8,
		utility_r = 9,
		weapon_l1 = 10,
		weapon_l2 = 11,
		weapon_r1 = 12,
		weapon_r2 = 13,
		utility_l1 = 14,
		utility_l2 = 15,
		utility_r1 = 16,
		utility_r2 = 17,
		soul_heart = 18,
		accessory_necklace = 19,
		accessory_ring1 = 20,
		accessory_ring2 = 21,
		armor_back = 22,
		max = 23
	};

	class udc_equipment_slot : public u_object {
	public:
		edc_equipment_slot_index slot_index() {
			return rpm<edc_equipment_slot_index>((uintptr_t)this + 0x28);
		}
		u_item* item() { 
			return rpm<u_item*>((uintptr_t)this + 0x30);
		}
	};

	class u_equipment_inv_comp {
	public:
		// TArray<AItemActor*> EquippedItemActors; // 0x0110   (0x0010)  
		// TArray<AItemActor*> SheathItemActors;   // 0x0120   (0x0010)  
		t_array<a_item_actor*> equipped_item_actors() {
			return rpm<t_array<a_item_actor*>>((uintptr_t)this + 0x110);
		}
		t_array<a_item_actor*> sheath_item_actors() {
			return rpm<t_array<a_item_actor*>>((uintptr_t)this + 0x120);
		}
		t_map<uint8_t, udc_equipment_slot*> equip_slot_map() {
			return rpm<t_map<uint8_t, udc_equipment_slot*>>((uintptr_t)this + 0x378);
		}
	};

	class u_skeleton : public u_object {
	public:
		t_array<f_bone_node> bone_tree() {
			return rpm<t_array<f_bone_node>>((uintptr_t)this + 0x38);
		}
	};

	class u_skeletal_mesh : public u_object {
	public:
		// 0xF8
		u_skeleton* skeleton() {
			return rpm<u_skeleton*>((uintptr_t)this + 0xF8);
		}
	};

	class u_skeletal_mesh_component : public u_scene_component {
	public:
		t_array<f_transform> cached_bone_space_transforms() {
			return rpm<t_array<f_transform>>((uintptr_t)this + 0x968);
		}
		f_transform get_bone(int bone_id) {
			uintptr_t bone_arr = rpm<uintptr_t>((uintptr_t)this + 0x5C0);
			if (!bone_arr) bone_arr = rpm<uintptr_t>((uintptr_t)this + 0x5C0 - 0x10);
			if (!bone_arr) bone_arr = rpm<uintptr_t>((uintptr_t)this + 0x5C0 - 0x10);

			return rpm<f_transform>(bone_arr + (bone_id * 0x30));
		}
		u_skeletal_mesh* skeletal_mesh() {
			return rpm<u_skeletal_mesh*>((uintptr_t)this + 0x578);
		}
	};

	class a_character : public actor {
	public:
		u_skeletal_mesh_component* skeletal_mesh() {
			return rpm<u_skeletal_mesh_component*>((uintptr_t)this + 0x318);
		}
	};

	class adc_character_base : public a_character {
	public:
		ability_system_component* ability_system_cmp() {
			return rpm<ability_system_component*>((uintptr_t)this + 0x6B0);
		}
	};

	class ubp_dchitbox_c : public u_scene_component {
	public:
		vector3 get_box_extent() {
			return rpm<vector3>((uintptr_t)this + 0x558);
		}
		void set_box_extent(vector3 value) {
			wpm<vector3>((uintptr_t)this + 0x558, value);
		}
	};

	class adc_monster_base : public adc_character_base {
	public:

	};

	class a_pawn : public adc_character_base {
	public:
		f_account_data_replication account_replication_data() {
			return rpm<f_account_data_replication>((uintptr_t)this + 0x738);
		}
		u_equipment_inv_comp* equipment_inv_comp() {
			return rpm<u_equipment_inv_comp*>((uintptr_t)this + 0xAD0);
		}
	};

	class u_interactable_target_component : public u_object {
	public:
		// t_array<actor*> 0x148 interactors
		t_array<actor*> interactors() {
			return rpm<t_array<actor*>>((uintptr_t)this + 0x148);
		}
		u_scene_component* current_interact_part() {
			return rpm<u_scene_component*>((uintptr_t)this + 0x158);
		}
	};
	
	class u_art_data_item : public u_object {
	public:
		// fname 0x30
		fname asset_type() {
			return rpm<fname>((uintptr_t)this + 0x30);
		}
		u_object* item_anim_instance_class() {
			return rpm<u_object*>((uintptr_t)this + 0x78);
		}
		u_object* item_static_mesh() {
			return rpm<u_object*>((uintptr_t)this + 0x48);
		}

	};

	class abp_static_mesh_item_holder_c : public actor {
	public:
		u_item* item_object() {
			return rpm<u_item*>((uintptr_t)this + 0x378);
		}
		// 0x380
		u_art_data_item* art_data_item() {
			return rpm<u_art_data_item*>((uintptr_t)this + 0x380);
		}
	};

	class abp_player_character : public a_pawn {
	public:
		ubp_dchitbox_c* head_hitbox() {
			return rpm<ubp_dchitbox_c*>((uintptr_t)this + 0xAC8);
		}
		std::vector<ubp_dchitbox_c*> get_all_hitboxes() {
			constexpr uintptr_t hitbox_arr_start = 0xA50;
			constexpr uintptr_t hitbox_arr_end = 0xAC8;
			int list_size = (hitbox_arr_end - hitbox_arr_start) / 8;

			ubp_dchitbox_c** hitbox_arr = new ubp_dchitbox_c*[list_size];

			read_raw((uintptr_t)this + hitbox_arr_start, hitbox_arr, (hitbox_arr_end - hitbox_arr_start));

			std::vector<ubp_dchitbox_c*> hitboxes{};

			for (int i = 0; i < list_size; i++) {
				ubp_dchitbox_c* hitbox = hitbox_arr[i];
				if (!hitbox) continue;
				hitboxes.push_back(hitbox);
			}

			hitboxes.push_back(head_hitbox());

			return hitboxes;
		}
		u_loot_component* loot_component() {
			return rpm<u_loot_component*>((uintptr_t)this + 0xA20);
		}
		u_interactable_target_component* target_component() {
			return rpm<u_interactable_target_component*>((uintptr_t)this + 0xA28);
		}
	};

	class a_chest_base_c : public actor {
	public:

	};

	class a_player_controller : public a_controller {
	public:
		a_player_camera_manager* camera_manager() {
			return rpm<a_player_camera_manager*>((uintptr_t)this + 0x348);
		}
		a_pawn* pawn() {
			return rpm<a_pawn*>((uintptr_t)this + 0x338);
		}
		void set_max_jumps(int32_t value) {
			wpm<int32_t>((uintptr_t)this + 0x424, value);
		}
	};

	class u_player {
	public:
		a_player_controller* player_controller() {
			return rpm<a_player_controller*>((uintptr_t)this + 0x30);
		}
	};

	class u_localplayer : public u_player {
	public:
		u_game_viewport_client* viewport_client() {
			return rpm<u_game_viewport_client*>((uintptr_t)this + 0x78);
		}
	};

	class a_floor_portal : public actor {
	public:
		bool is_open() {
			return rpm<bool>((uintptr_t)this + 0x3F0);
		}
		int type() {
			return (int)rpm<uint8_t>((uintptr_t)this + 0x3C0);
		}
	};

	class u_game_instance {
	public:
		u_localplayer* get_localplayer() {
			auto lclplrs = rpm<t_array<u_localplayer*>>((uintptr_t)this + 0x38);
			return lclplrs.at(0);
		}
	};

	class a_player_state : public actor {
	public:
		a_pawn* pawn() {
			return rpm<a_pawn*>((uintptr_t)this + 0x308);
		}
	};

	class u_game_state {
	public:
		t_array<a_player_state*> player_array() {
			return rpm<t_array<a_player_state*>>((uintptr_t)this + 0x2A8);
		}
		t_array<f_account_data_replication> account_data_replication() {
			return rpm<t_array<f_account_data_replication>>((uintptr_t)this + 0x3F0);
		}

	};

	class u_world {
	public:
		static u_world* get_world() {
			return rpm<u_world*>(mem::module_base + GWORLD);
		}

		u_level* persistent_level() {
			return rpm<u_level*>((uintptr_t)this + 0x30);
		}

		t_array<u_level*> levels() {
			return rpm<t_array<u_level*>>((uintptr_t)this + 0x170);
		}

		u_game_state* game_state() {
			return rpm<u_game_state*>((uintptr_t)this + 0x158);
		}

		u_game_instance* owning_game_instance() {
			return rpm<u_game_instance*>((uintptr_t)this + 0x1b8);
		}
	};
}