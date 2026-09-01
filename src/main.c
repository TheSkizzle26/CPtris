#include <cp_base.c>

#include "board.c"
#include "cell.c"
#include "active.c"

int main() {
    cp_init();
    cp_setTargetFPS(60);

    // clear screen
    for (unsigned i = 0; i < (unsigned)cp_getPixelCount(); i++)
        cp_pixels[i] = 100;

    cell_init();
    active_nextPiece();

    while (!cp_isKeyDown(CP_KEY_CLEAR)) {
        active_update();
        board_renderDirty();

        cp_refreshScreen();
        cp_finishFrame();
    }

    cp_quit();
    return 0;
}
