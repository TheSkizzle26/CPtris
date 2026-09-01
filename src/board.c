#ifndef BOARD
#define BOARD

#include <stdbool.h>
#include <stdint.h>

#include "cell.c"

// ------------ INTERFACE -------------

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define BOARD_CELLS (BOARD_WIDTH*BOARD_HEIGHT)

// TODO: check if other data types is faster
unsigned board_cells[BOARD_CELLS];
bool board_dirty[BOARD_CELLS];

void board_renderCell(unsigned i);
void board_renderDirty();

// ---------- IMPLEMENTATION ----------

void board_renderDirty() {
    unsigned i = 0;

    for (unsigned y = 0; y < BOARD_HEIGHT; y++) {
        for (unsigned x = 0; x < BOARD_WIDTH; x++) {
            if (board_dirty[i]) {
                board_dirty[i] = false;
                cell_render(board_cells[i], x, y);
            }

            i++;
        }
    }
}

#endif