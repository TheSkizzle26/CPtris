#ifndef BOARD
#define BOARD

#include <stdbool.h>

// ------------ INTERFACE -------------

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 22 // 2 invisible lines
#define BOARD_CELLS (BOARD_WIDTH*BOARD_HEIGHT)

// TODO: check if other data types are faster
unsigned board_cells[BOARD_CELLS];
bool board_dirty[BOARD_CELLS];

void board_reset();
unsigned board_getCell(unsigned x, unsigned y);
void board_setCell(unsigned x, unsigned y, unsigned type);
void board_markDirty(unsigned x, unsigned y);
void board_renderDirty();

// ---------- IMPLEMENTATION ----------

#include "main.c"
#include "cell.c"

void board_reset() {
    for (unsigned i = 0; i < BOARD_CELLS; i++)
        board_cells[i] = 0;
}

unsigned board_getCell(const unsigned x, const unsigned y) {
    return board_cells[y * BOARD_WIDTH + x];
}

void board_setCell(unsigned x, unsigned y, unsigned type) {
    board_cells[y * BOARD_WIDTH + x] = type;
}

void board_markDirty(const unsigned x, const unsigned y) {
    board_dirty[y * BOARD_WIDTH + x] = 1;
}

#include <stdio.h>

void board_renderDirty() {
    unsigned i = BOARD_WIDTH*2;

    printf("render dirty\n");

    for (unsigned y = 2; y < BOARD_HEIGHT; y++) {
        for (unsigned x = 0; x < BOARD_WIDTH; x++) {
            if (board_dirty[i]) {
                printf("%u, %u\n", x, y);
                board_dirty[i] = false;
                cell_render(board_cells[i], x, y);
            }

            i++;
        }
    }

    printf("end render\n");
    fflush(stdout);

    main_queryRefresh();
}

#endif