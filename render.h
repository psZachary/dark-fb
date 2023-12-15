#pragma once

#include "overlay/overlay.h"
#include "util.h"
#include "config.h"
#include "cvar.h"
#include "esp.h"
#include "main.h"
#include "overlay/overlay.h"
#include "overlay/winhide.h"
#include "menu.h"

void on_overlay_render() {
    try
    {
        menu::draw();
        esp::draw();
    }
    catch (...)
    {
        //std::cout << "Error on overlay render callback!" << std::endl;
    }
}

inline int render_threaded() {
    c_overlay overlay;

    cvar::overlay = &overlay;
    
    wh::WH_RESULT res = wh::hide_window(overlay.get_window_handle());
    if (res != wh::success && res != wh::already_hidden) {
        fail("failed to hide window");
    }

    overlay.bind_render_callback(on_overlay_render);

    while (true) {
        overlay.msg_loop();
    }
}