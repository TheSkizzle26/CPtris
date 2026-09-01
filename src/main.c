#include <cp_base.c>

int main() {
    cp_init();
    cp_setTargetFPS(1);

    for (int y = 0; y < cp_getRenderHeight(); y++) {
        for (int x = 0; x < cp_getRenderWidth(); x++) {
            cp_pixels[y*cp_getRenderWidth() + x] = (x << 2) ^ (y << 2);
        }
    }

    while (!cp_isKeyDown(CP_KEY_CLEAR)) {
        cp_pixels[10000 + cp_tick] = -1;

        cp_refreshScreen();
        cp_finishFrame();
    }

    cp_quit();
    return 0;
}
