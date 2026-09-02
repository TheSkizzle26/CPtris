#ifndef MAIN
#define MAIN

#include <stdbool.h>

// ------------ INTERFACE -------------

bool main_refreshQueried = true;

void main_queryRefresh();
int main();

// ---------- IMPLEMENTATION ----------

#include <cp_base.c>

#include "board.c"
#include "cell.c"
#include "active.c"
#include "progress.c"

void main_queryRefresh() {
    main_refreshQueried = true;
}

int main() {
    cp_init();
    cp_setTargetFPS(60);

    cp_setMemory(0, cp_getPixelCount() * sizeof(uint16_t), cp_pixels);

    cell_init();
    progress_reset();

    while (!cp_isKeyDown(CP_KEY_CLEAR)) {
        active_tick();
        board_renderDirty();
        active_render();

        if (main_refreshQueried) {
            main_refreshQueried = false;
            cp_refreshScreen();
        }

        cp_finishFrame();
    }

    cp_quit();
    return 0;
}

#endif