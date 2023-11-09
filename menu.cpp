#include "menu.h"
#include "overlay/overlay.h"
#include "cvar.h"
#include "overlay/winhide.h"

void menu::draw()
{
    if (GetAsyncKeyState(config::menu::toggle_key) & 1) {
        config::menu::is_open = !config::menu::is_open;
    }

    if (!config::menu::is_open) return;

    ImGui::SetNextWindowSize(ImVec2(780, 770), ImGuiCond_FirstUseEver);
    ImGui::Begin("Labcore D&D External", 0, ImGuiWindowFlags_NoDecoration);
    ImGui::TextColored(ImVec4(16.f / 255.f, 209.f / 255.f, 243.f / 255.f, 1.f), "Labcore D&D External");
    ImGui::Separator();

    header_tab_item("Combat", config::menu::tab_index, 0); ImGui::SameLine();
    header_tab_item("Visuals", config::menu::tab_index, 1); ImGui::SameLine();
    header_tab_item("Config", config::menu::tab_index, 2);

    ImGui::Separator();

    if (config::menu::tab_index == 0) {
        ImGui::Text("Aimbot");
        ImGui::BeginChild("Aimbot", ImVec2(0, 170), true, ImGuiWindowFlags_ChildWindow);
            ImGui::Checkbox("Aimbot", &config::combat::aimbot);
            ImGui::Checkbox("Prediction", &config::combat::prediction);
            ImGui::Hotkey("Key", &config::combat::key);
            ImGui::Combo("Aim Type", &config::combat::aim_type, "By World Distance (In FOV)\0By Screen Distance (In FOV)\0By World Distance");
            ImGui::SliderFloat("Strength", &config::combat::strength, 0.f, 50.f, "%.0f");
            ImGui::SliderFloat("FOV", &config::combat::fov, 0.f, 1000.f, "%.0f");
        ImGui::EndChild();
        ImGui::Text("Exploits");
        ImGui::BeginChild("Exploits", ImVec2(0, 300), true);
            // Don't put this any higher or you get banned 
            ImGui::Checkbox("Big Head Hitbox", &config::combat::big_heads);
            ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "Expansion Size");
            ImGui::SliderFloat("X", &config::combat::big_head_expansion_x, 0.f, 6.f, "%.2f");
            ImGui::SliderFloat("Y", &config::combat::big_head_expansion_y, 0.f, 6.f, "%.2f");
            ImGui::SliderFloat("Z", &config::combat::big_head_expansion_z, 0.f, 6.f, "%.2f");
            ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "Activation Distance");
            ImGui::SliderFloat("  ", &config::combat::activation_distance, 1.f, 10.f, "%.2f");
            ImGui::Checkbox("No Hit Teamates", &config::combat::no_hit_teamates);
            ImGui::Checkbox("Move Speed", &config::combat::move_speed_enabled);
            if (config::combat::move_speed_enabled)
                ImGui::SliderFloat("  ", &config::combat::move_speed, 0.f, 350.f, "%.2f");

            ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "Warning: All features here may cause crashes");
            ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "Warning: In some instances, 72 hour bans have been reported use these at your own risk");
        ImGui::EndChild();
    }
    if (config::menu::tab_index == 2) {
        if (ImGui::CollapsingHeader("Overlay", ImGuiTreeNodeFlags_Framed)) {
            if (ImGui::Checkbox("Hidden", &config::menu::hide_overlay)) {
                std::thread([] { wh::hide_window(cvar::overlay->get_window_handle(), config::menu::hide_overlay);
                }).detach();
            }
        }
 
        if (ImGui::CollapsingHeader("Menu", ImGuiTreeNodeFlags_Framed)) {
            ImGui::Hotkey("Toggle Key", &config::menu::toggle_key);
        }
    }
    if (config::menu::tab_index == 1) {
        tab_item("Players", config::menu::visuals::tab_index, 0); ImGui::SameLine();
        tab_item("Chests", config::menu::visuals::tab_index, 1); ImGui::SameLine();
        tab_item("Portals", config::menu::visuals::tab_index, 2); ImGui::SameLine();
        tab_item("Shrines", config::menu::visuals::tab_index, 3); ImGui::SameLine();
        tab_item("Traps", config::menu::visuals::tab_index, 4); ImGui::SameLine();
        tab_item("Ore", config::menu::visuals::tab_index, 5); ImGui::SameLine();
        tab_item("AI", config::menu::visuals::tab_index, 6); ImGui::SameLine();
        tab_item("Loot", config::menu::visuals::tab_index, 7); ImGui::SameLine();
        tab_item("Other", config::menu::visuals::tab_index, 8); ImGui::SameLine();
        tab_item("Colors", config::menu::visuals::tab_index, 9);
        ImGui::Separator();

        switch (config::menu::visuals::tab_index) {
        case 0: {
            ImGui::Text("ESP");
            ImGui::BeginChild("ESP", ImVec2(0, 325), true);
            ImGui::Checkbox("Enabled", &config::esp::players::enabled);
            ImGui::Checkbox("Name", &config::esp::players::name);
            ImGui::Checkbox("Distance", &config::esp::players::distance);
            ImGui::Checkbox("Health", &config::esp::players::health);
            ImGui::Checkbox("Skeleton", &config::esp::players::skeleton);
            ImGui::Checkbox("Box", &config::esp::players::box);
            ImGui::Checkbox("Head Hitboxes", &config::esp::players::head_hitboxes);
            ImGui::Checkbox("Highlight Target", &config::esp::players::highlight_target);
            ImGui::Checkbox("Equipped Items", &config::esp::players::equipped_items);
            ImGui::Checkbox("Armor Items", &config::esp::players::armor_items);
            ImGui::Checkbox("Utility Items", &config::esp::players::utility_items);
            ImGui::Checkbox("Sheathed Items", &config::esp::players::sheathed_items);
            ImGui::EndChild();
            ImGui::Text("Lobby Window");
            ImGui::BeginChild("Lobby", ImVec2(0, 200), true);
            ImGui::Checkbox("Enabled", &config::esp::players::lobby::enabled);
            ImGui::Checkbox("Party ID", &config::esp::players::lobby::party_id);
            ImGui::Checkbox("Level", &config::esp::players::lobby::level);
            ImGui::Checkbox("Class", &config::esp::players::lobby::class_name);
            ImGui::Checkbox("Name", &config::esp::players::lobby::name);
            ImGui::SliderFloat("Transparency", &config::esp::players::lobby::transparency, 0.f, 1.f, "%.1f");
            ImGui::Hotkey("Toggle Show", &config::esp::players::lobby::toggle_key);
            ImGui::EndChild();
            ImGui::Text("Game Status");
            ImGui::BeginChild("Game Status", ImVec2(0, 50), true);
            ImGui::Checkbox("Enabled", &config::esp::players::game_status::enabled);
            ImGui::EndChild();
            break;
        }
        case 1: {
            ImGui::Checkbox("Enabled", &config::esp::chest::enabled);
            ImGui::Checkbox("Name", &config::esp::chest::name);
            ImGui::Checkbox("Distance", &config::esp::chest::distance);
            if (ImGui::CollapsingHeader("Chests")) {
                ImGui::BeginChild("Chests", ImVec2(0, 200), true);

                ImGui::Checkbox("Gold", &config::esp::chest::gold);
                ImGui::Checkbox("Lions Head", &config::esp::chest::lions_head);
                ImGui::Checkbox("Ornate", &config::esp::chest::ornate);
                ImGui::Checkbox("Coffin", &config::esp::chest::coffin);
                ImGui::Checkbox("Mimic", &config::esp::chest::mimic);
                ImGui::Checkbox("Flat", &config::esp::chest::flat);
                ImGui::Checkbox("Other", &config::esp::chest::other);

                ImGui::EndChild();
            }
            ImGui::SliderFloat("Render Distance", &config::esp::chest::render_distance, 0.f, 100.f, "%.0f");
            break;
        }
        case 2: {
            ImGui::Checkbox("Enabled", &config::esp::portal::enabled);
            ImGui::Checkbox("Name", &config::esp::portal::name);
            ImGui::Checkbox("Distance", &config::esp::portal::distance);
            if (ImGui::CollapsingHeader("Portals")) {
                ImGui::BeginChild("Portals", ImVec2(0, 200), true);

                ImGui::Checkbox("Down", &config::esp::portal::down);
                ImGui::Checkbox("Escape", &config::esp::portal::escape);

                ImGui::EndChild();
            }
            break;
        }
        case 3: {
            ImGui::Checkbox("Enabled", &config::esp::shrine::enabled);
            ImGui::Checkbox("Name", &config::esp::shrine::name);
            ImGui::Checkbox("Distance", &config::esp::shrine::distance);
            if (ImGui::CollapsingHeader("Shrines")) {
                ImGui::BeginChild("Shrines", ImVec2(0, 200), true);

                ImGui::Checkbox("Health", &config::esp::shrine::health);
                ImGui::Checkbox("Power", &config::esp::shrine::power);
                ImGui::Checkbox("Speed", &config::esp::shrine::speed);
                ImGui::Checkbox("Revive", &config::esp::shrine::revive);
                ImGui::Checkbox("Protection", &config::esp::shrine::shield);

                ImGui::EndChild();
            }
            break;
        }
        case 4: {
            ImGui::Checkbox("Enabled", &config::esp::trap::enabled);
            ImGui::Checkbox("Name", &config::esp::trap::name);
            ImGui::Checkbox("Distance", &config::esp::trap::distance);
            if (ImGui::CollapsingHeader("Traps")) {
                ImGui::BeginChild("Traps", ImVec2(0, 200), true);

                ImGui::Checkbox("Wall Spikes", &config::esp::trap::wall_spike);
                ImGui::Checkbox("Floor Spikes", &config::esp::trap::floor_spike);
                ImGui::Checkbox("Hunting Traps", &config::esp::trap::hunting_trap);

                ImGui::EndChild();
            }
            ImGui::SliderFloat("Render Distance", &config::esp::trap::render_distance, 0.f, 100.f, "%.0f");



            break;
        }
        case 5: {
            ImGui::Checkbox("Enabled", &config::esp::ore::enabled);
            ImGui::Checkbox("Name", &config::esp::ore::name);
            ImGui::Checkbox("Distance", &config::esp::ore::distance);

            if (ImGui::CollapsingHeader("Ores")) {
                ImGui::BeginChild("Ores", ImVec2(0, 200), true);

                ImGui::Checkbox("Gold", &config::esp::ore::gold);
                ImGui::Checkbox("Ruby Silver", &config::esp::ore::ruby_silver);
                ImGui::Checkbox("Cobalt", &config::esp::ore::cobalt);
  

                ImGui::EndChild();
            }
            ImGui::SliderFloat("Render Distance", &config::esp::ore::render_distance, 0.f, 100.f, "%.0f");

            break;
        }
        case 6: {
            ImGui::Checkbox("Enabled", &config::esp::ai::enabled);
			ImGui::Checkbox("Name", &config::esp::ai::name);
			ImGui::Checkbox("Distance", &config::esp::ai::distance);
			ImGui::Checkbox("Health", &config::esp::ai::health);
            // filters
            if (ImGui::CollapsingHeader("AI")) {
				ImGui::BeginChild("Filters", ImVec2(0, 220), true);

				ImGui::Checkbox("Bosses", &config::esp::ai::bosses);
				ImGui::Checkbox("Minibosses", &config::esp::ai::minibosses);
				ImGui::Checkbox("Goblin", &config::esp::ai::goblin);
				ImGui::Checkbox("Skeleton", &config::esp::ai::skeleton);
				ImGui::Checkbox("Mummy", &config::esp::ai::mummy);
				ImGui::Checkbox("Zombie", &config::esp::ai::zombie);
				ImGui::Checkbox("Death Skull", &config::esp::ai::skull);
				ImGui::Checkbox("Spider", &config::esp::ai::spider);
				ImGui::Checkbox("Other", &config::esp::ai::other);

				ImGui::EndChild();
			}
            ImGui::SliderFloat("Render Distance", &config::esp::ai::render_distance, 0.f, 100.f, "%.0f");
			break;
        }
        case 7: {
            ImGui::Checkbox("Enabled", &config::esp::loot::enabled);
            ImGui::Checkbox("Name", &config::esp::loot::name);
            ImGui::Checkbox("Distance", &config::esp::loot::distance);
            ImGui::SliderFloat("Render Distance", &config::esp::loot::render_distance, 0.f, 100.f, "%.0f");
            break;
        }
        case 8: {
            ImGui::Checkbox("Draw Actors", &config::esp::other::actors);
            break;
        }
        case 9: {
            ImGui::PushItemWidth(200.f);

            if (ImGui::CollapsingHeader("Player Colors", ImGuiTreeNodeFlags_Framed)) {
                ImGui::ColorPicker3("Skeleton", &config::esp::players::colors::skeleton.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
                ImGui::ColorPicker3("Hitboxes", &config::esp::players::colors::hitboxes.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
                ImGui::ColorPicker3("Box", &config::esp::players::colors::box.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
                ImGui::ColorPicker3("Name", &config::esp::players::colors::name.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
                ImGui::ColorPicker3("Distance", &config::esp::players::colors::distance.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
                ImGui::ColorPicker3("Friendly", &config::esp::players::colors::friendly.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
                ImGui::ColorPicker3("Target", &config::esp::players::colors::target.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
            }
            if (ImGui::CollapsingHeader("Item Rarity Colors", ImGuiTreeNodeFlags_Framed)) {
                ImGui::ColorPicker3("Junk", &config::colors::color_junk.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
                ImGui::ColorPicker3("Poor", &config::colors::color_poor.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
                ImGui::ColorPicker3("Common", &config::colors::color_common.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
                ImGui::ColorPicker3("Uncommon", &config::colors::color_uncommon.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
                ImGui::ColorPicker3("Rare", &config::colors::color_rare.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
                ImGui::ColorPicker3("Epic", &config::colors::color_epic.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
                ImGui::ColorPicker3("Legendary", &config::colors::color_legendary.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
            }
            if (ImGui::CollapsingHeader("Portal Colors", ImGuiTreeNodeFlags_Framed)) {
                ImGui::ColorPicker3("Escape", &config::esp::portal::colors::escape.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
                ImGui::ColorPicker3("Down", &config::esp::portal::colors::down.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
            }
            if (ImGui::CollapsingHeader("Shrine Colors", ImGuiTreeNodeFlags_Framed)) {
                ImGui::ColorPicker3("Health", &config::esp::shrine::colors::health.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
                ImGui::ColorPicker3("Power", &config::esp::shrine::colors::power.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
                ImGui::ColorPicker3("Shield", &config::esp::shrine::colors::shield.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
                ImGui::ColorPicker3("Speed", &config::esp::shrine::colors::speed.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
                ImGui::ColorPicker3("Revive", &config::esp::shrine::colors::revive.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
            }
            if (ImGui::CollapsingHeader("Trap Colors", ImGuiTreeNodeFlags_Framed)) {
                ImGui::ColorPicker3("Floor Spikes", &config::esp::trap::colors::floor_spike.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
                ImGui::ColorPicker3("Wall Spike", &config::esp::trap::colors::wall_spike.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
                ImGui::ColorPicker3("Hunting Trap", &config::esp::trap::colors::hunting_trap.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
            }
            if (ImGui::CollapsingHeader("Ore Colors", ImGuiTreeNodeFlags_Framed)) {
                ImGui::ColorPicker3("Gold", &config::esp::ore::colors::gold.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
                ImGui::ColorPicker3("Ruby Silver", &config::esp::ore::colors::ruby_silver.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
                ImGui::ColorPicker3("Cobalt", &config::esp::ore::colors::cobalt.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
            }
            if (ImGui::CollapsingHeader("AI Colors", ImGuiTreeNodeFlags_Framed)) {
                ImGui::ColorPicker3("AI", &config::esp::ai::colors::ai.r, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs);
            }
            
            ImGui::PopItemWidth();
            ImGui::Text("Use arrow keys to scroll up / down");
            break;
        }
        }
    }

    ImGui::End();
    
}
