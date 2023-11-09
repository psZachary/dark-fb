#include <iostream>
#include <thread>
#include <chrono>
#include "memory.h"
#include "sdk.h"
#include "render.h"
#include "cvar.h"
#include "main.h"
#include "combat.h"


using namespace sdk;

void exploit_threaded() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(250));

        if (!cvar::validate_cvars()) continue;

        combat::big_heads(config::combat::activation_distance, config::combat::big_head_expansion);
    }
}

void load_cheat() {
    while (!mem::process_id) {
        static int attempts = 0;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        mem::attach(WINDOW);
        if (attempts++ > 30) {
			fail("failed to attach to process");
		}
    }

    module_base = mem::get_module_base(MODULE_NAME, mem::process_id);
    while (!module_base) {
        module_base = mem::get_module_base(MODULE_NAME, mem::process_id);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::thread render_thread(render_threaded);
    std::thread cache_thread(cvar::cache_threaded);
    std::thread exploit_thread(exploit_threaded);

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        if (!cvar::validate_cvars()) continue;

        if (config::combat::aimbot && GetAsyncKeyState(config::combat::key)) {
            cvar::target_lock = true;
            combat::do_aimbot();
        }
        else {
            cvar::target_lock = false;
            combat::last_aim_location = vector3::zero();
        }
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));
}
#ifndef DLL
int main() {
    std::cout << "[F1] load cheat" << std::endl << "[F2] clear overlay" << std::endl;

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        if (GetAsyncKeyState(VK_F1) & 1) break;
        if (GetAsyncKeyState(VK_F2) & 1) {
            HWND overlay = FindWindowA(NULL, "NVIDIA GeForce Overlay");
            DWORD process_id = 0;
            GetWindowThreadProcessId(overlay, &process_id);
            HANDLE process_handle = OpenProcess(PROCESS_TERMINATE, FALSE, process_id);
            TerminateProcess(process_handle, 0);
            std::cout << "cleared overlay" << std::endl;
        }
    }

    system("cls");

    load_cheat();

    return 0;

}
#endif
#ifdef DLL
void start_console() {
    AllocConsole();
    FILE* f = 0;
    freopen_s(&f, "CONOUT$", "w", stdout);
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        //start_console();
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)load_cheat, 0, 0, 0);
    }
    return TRUE;
}

#endif