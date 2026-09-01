#include <cp_base.c>

int main() {
    cp_init();
    cp_setTargetFPS(60);

    while (!cp_isKeyDown(CP_KEY_CLEAR)) {
        cp_refreshScreen();
        cp_finishFrame();
    }

    cp_quit();
    return 0;
}
