#ifndef ACTIVE
#define ACTIVE

#include <stdbool.h>
#include <cp_base.c>

#include "cell.c"
#include "board.c"

// ------------ INTERFACE -------------

struct {
    bool changed;
    unsigned x;
    unsigned y;
    unsigned rotation;
    unsigned gravityTicks;
    unsigned piece;
    unsigned cellType;
    unsigned size;
    bool *rotations;
} active_current;

unsigned active_lastPiece;
unsigned active_hash = 5381;

struct {
    bool next;
} active_lastInputs;

bool active_templateCells[7][64] = {
    { // T
        0, 0, 0, 0,
        1, 1, 1, 0,
        0, 1, 0, 0,
        0, 0, 0, 0,

        0, 1, 0, 0,
        1, 1, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, 0,

        0, 1, 0, 0,
        1, 1, 1, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,

        0, 1, 0, 0,
        0, 1, 1, 0,
        0, 1, 0, 0,
        0, 0, 0, 0,
    },
    { // J
        0, 0, 0, 0,
        1, 1, 1, 0,
        0, 0, 1, 0,
        0, 0, 0, 0,

        0, 1, 0, 0,
        0, 1, 0, 0,
        1, 1, 0, 0,
        0, 0, 0, 0,

        1, 0, 0, 0,
        1, 1, 1, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,

        0, 1, 1, 0,
        0, 1, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, 0,
    },
    { // Z
        0, 0, 0, 0,
        1, 1, 0, 0,
        0, 1, 1, 0,
        0, 0, 0, 0,

        0, 0, 1, 0,
        0, 1, 1, 0,
        0, 1, 0, 0,
        0, 0, 0, 0,
    },
    { // O
        1, 1, 0, 0,
        1, 1, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
    },
    { // S
        0, 0, 0, 0,
        0, 1, 1, 0,
        1, 1, 0, 0,
        0, 0, 0, 0,

        1, 0, 0, 0,
        1, 1, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, 0,
    },
    { // L
        0, 1, 0, 0,
        0, 1, 0, 0,
        0, 1, 1, 0,
        0, 0, 0, 0,

        0, 0, 0, 0,
        1, 1, 1, 0,
        1, 0, 0, 0,
        0, 0, 0, 0,

        1, 1, 0, 0,
        0, 1, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, 0,

        0, 0, 1, 0,
        1, 1, 1, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
    },
    { // I
        0, 0, 0, 0,
        0, 0, 0, 0,
        1, 1, 1, 1,
        0, 0, 0, 0,

        0, 0, 1, 0,
        0, 0, 1, 0,
        0, 0, 1, 0,
        0, 0, 1, 0,
    }
};

unsigned active_templateRotations[7] = {
    4, // T
    4, // J
    2, // Z
    1, // O
    2, // S
    4, // L
    2  // I
};

unsigned active_templateSizes[7] = {
    3, // T
    3, // J
    3, // Z
    2, // O
    3, // S
    3, // L
    4  // I
};

unsigned active_templateCellTypes[7] = {
    3, // T
    2, // J
    1, // Z
    3, // O
    2, // S
    1, // L
    3  // I
};

unsigned active_framesPerGridcell[30] = {
    48, 43, 38, 33, 28,
    23, 18, 13, 8,  6,
    5,  5,  5,  4,  4,
    4,  3,  3,  3,  2,
    2,  2,  2,  2,  2,
    2,  2,  2,  2,  1
};

unsigned active_getFramesPerGridcell();
void active_nextHash();
void active_nextPiece();
void active_markDirty();
bool active_isColliding();
void active_place();
void active_fall();
void active_tick();
void active_render();

// ---------- IMPLEMENTATION ----------

unsigned active_getFramesPerGridcell() {
    // TODO: use level number
    return active_framesPerGridcell[9];
}

void active_nextHash() {
    active_hash = (active_hash << 5) + active_hash + cp_getTick() % 128;
}

void active_nextPiece() {
    unsigned piece = active_hash % 7;
    if (piece == active_lastPiece) {
        active_nextHash();
        piece = active_hash % 7;
    }

    active_lastPiece = active_current.piece;
    
    active_current.changed = true;
    active_current.x = 0;
    active_current.y = 0;
    active_current.rotation = 0;
    active_current.gravityTicks = active_getFramesPerGridcell();
    active_current.piece = piece;
    active_current.cellType = active_templateCellTypes[piece];
    active_current.size = active_templateSizes[piece];
    active_current.rotations = active_templateCells[piece];
}

void active_markDirty() {
    if (!active_current.rotations)
        return;

    for (unsigned dy = 0; dy < active_current.size; dy++)
        for (unsigned dx = 0; dx < active_current.size; dx++)
            board_markDirty(active_current.x + dx, active_current.y + dy);
}

bool active_isColliding() {
    const unsigned rotationOffset = active_current.rotation * 16;

    for (unsigned dy = 0; dy < active_current.size; dy++) {
        for (unsigned dx = 0; dx < active_current.size; dx++) {
            if (active_current.rotations[rotationOffset + dy*4 + dx]) {
                const unsigned gx = active_current.x + dx;
                const unsigned gy = active_current.y + dy;

                if (gx >= BOARD_WIDTH || gy >= BOARD_HEIGHT)
                    return true;

                if (board_getCell(gx, gy))
                    return true;
            }
        }
    }

    return false;
}

void active_place() {
    const unsigned rotationOffset = active_current.rotation * 16;

    for (unsigned dy = 0; dy < active_current.size; dy++) {
        bool full = true;

        for (unsigned dx = 0; dx < active_current.size; dx++) {
            const unsigned gx = active_current.x + dx;
            const unsigned gy = active_current.y + dy;

            if (active_current.rotations[rotationOffset + dy*4 + dx])
                board_setCell(gx, gy, active_current.cellType);

            if (!board_getCell(gx, gy))
                full = false;
        }

        if (full) {

        }
    }
}

void active_fall() {
    active_current.gravityTicks--;

    if (!active_current.gravityTicks) {
        active_current.gravityTicks = active_getFramesPerGridcell();
        
        active_markDirty();
        
        active_current.y++;
        active_current.changed = true;

        if (active_isColliding()) {
            active_current.y--;
            active_place();

            active_nextPiece();
        }
    }
}

void active_tick() {
    active_nextHash();

    // debug
    if (cp_isKeyDown(CP_KEY_EXE)) {
        active_markDirty();
        active_nextPiece();
    }

    active_fall();

    active_lastInputs.next = cp_isKeyDown(CP_KEY_EXE);
}

void active_render() {
    if (!active_current.changed)
        return;

    const unsigned rotationOffset = active_current.rotation * 16;

    for (unsigned dy = 0; dy < active_current.size; dy++)
        for (unsigned dx = 0; dx < active_current.size; dx++)
            if (active_current.rotations[rotationOffset + dy*4 + dx])
                cell_render(active_current.cellType, active_current.x + dx, active_current.y + dy);

    active_current.changed = false;
}

#endif