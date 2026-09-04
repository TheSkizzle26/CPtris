#ifndef ACTIVE
#define ACTIVE

#include <stdbool.h>

// ------------ INTERFACE -------------

struct {
    unsigned x;
    unsigned y;
    unsigned rotation;
} active_transform;

// quick access to piece information
struct {
    unsigned cellType;
    unsigned size;
    unsigned rotationCount;
    bool *rotations;
} active_piece;

struct {
    unsigned leftTicks;
    unsigned rightTicks;
    bool moveLeft;
    bool moveRight;
} active_das;

unsigned active_hash = 5381;
unsigned active_gravityTicks;
unsigned active_stallTicks;
bool active_dirty;

struct {
    bool cells[7][64];
    unsigned rotations[7];
    unsigned size[7];
    unsigned cellType[7];
} active_templates = {
    .cells = {
        { // T
            0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        }, { // J
            0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        }, { // Z
            0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        }, { // O
            1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        }, { // S
            0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
        }, { // L
            0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
            0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
        }, { // I
            0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0,
        }
    },
    .rotations = {
        4, // T
        4, // J
        2, // Z
        1, // O
        2, // S
        4, // L
        2  // I
    },
    .size = {
        3, // T
        3, // J
        3, // Z
        2, // O
        3, // S
        3, // L
        4  // I
    },
    .cellType = {
        3, // T
        2, // J
        1, // Z
        3, // O
        2, // S
        1, // L
        3  // I
    }
};

unsigned active_fallDelay[30] = {
    48, 43, 38, 33, 28,
    23, 18, 13, 8,  6,
    5,  5,  5,  4,  4,
    4,  3,  3,  3,  2,
    2,  2,  2,  2,  2,
    2,  2,  2,  2,  1
};

unsigned active_AREDelay[22] = {
    18, 18, // invisible
    18, 18, 18, 18,
    16, 16, 16, 16,
    14, 14, 14, 14,
    12, 12, 12, 12,
    10, 10, 10, 10
};

unsigned active_getFallDelay();
void active_stall(unsigned ticks);
void active_nextHash();
void active_spawnNextPiece();
void active_markDirty();
bool active_isColliding();
void active_place();
void active_fall();
void active_attemptMove(signed movement);
void active_attemptRotation(const signed rotation);
void active_handleDAS();
void active_notStalled();
void active_tick();
void active_render();

// ---------- IMPLEMENTATION ----------

#include <cp_base.c>

#include "main.c"
#include "input.c"
#include "cell.c"
#include "board.c"
#include "progress.c"

unsigned active_getFallDelay() {
    return active_fallDelay[progress_level > 29 ? 29 : progress_level];
}

void active_stall(const unsigned ticks) {
    active_stallTicks = ticks;
}

void active_nextHash() {
    active_hash = (active_hash << 5) + active_hash + cp_getTick() % 128;
}

void active_spawnNextPiece() {
    unsigned piece = 0;

    const unsigned size = active_templates.size[piece];

    active_transform.x = (BOARD_WIDTH - size) >> 1;
    active_transform.y = size - 2;
    active_transform.rotation = 0;

    active_piece.cellType = active_templates.cellType[piece];
    active_piece.size = size;
    active_piece.rotationCount = active_templates.rotations[piece];
    active_piece.rotations = active_templates.cells[piece];

    active_dirty = true;
    active_gravityTicks = active_getFallDelay();
}

void active_markDirty() {
    if (!active_piece.rotations)
        return;

    const unsigned rotationOffset = active_transform.rotation * 16;

    for (unsigned dy = 0; dy < active_piece.size; dy++)
        for (unsigned dx = 0; dx < active_piece.size; dx++)
            if (active_piece.rotations[rotationOffset + dy*4 + dx])
                board_markDirty(active_transform.x + dx, active_transform.y + dy);
}

bool active_isColliding() {
    const unsigned rotationOffset = active_transform.rotation * 16;

    for (unsigned dy = 0; dy < active_piece.size; dy++) {
        for (unsigned dx = 0; dx < active_piece.size; dx++) {
            if (active_piece.rotations[rotationOffset + dy*4 + dx]) {
                const unsigned gx = active_transform.x + dx;
                const unsigned gy = active_transform.y + dy;

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
    const unsigned rotationOffset = active_transform.rotation * 16;
    unsigned dirtyCount = 0;
    unsigned clearCount = 0;
    unsigned pieceBottom = 0;

    for (unsigned dy = 0; dy < active_piece.size; dy++) {
        const unsigned gy = active_transform.y + dy;
        if (gy >= BOARD_HEIGHT) continue;

        for (unsigned dx = 0; dx < active_piece.size; dx++) {
            if (active_piece.rotations[rotationOffset + dy*4 + dx]) {
                board_setCell(active_transform.x + dx, gy, active_piece.cellType);

                if (gy > pieceBottom)
                    pieceBottom = gy;
            }
        }

        bool full = true;

        for (unsigned x = 0; x < BOARD_WIDTH; x++) {
            if (!board_getCell(x, gy)) {
                full = false;
                break;
            }
        }

        if (full) {
            clearCount++;

            if (gy-1 > dirtyCount)
                dirtyCount = gy-1;

            for (unsigned y = gy; y > 0; y--) {
                cp_copyMemory(
                    board_cells + (y-1) * BOARD_WIDTH,
                    BOARD_WIDTH * sizeof(unsigned),
                    board_cells + y * BOARD_WIDTH
                );
            }
        }
    }

    if (clearCount) {
        cp_setMemory(1, dirtyCount * BOARD_WIDTH, board_dirty + BOARD_WIDTH*2);
        progress_registerLineClears(clearCount);

        // don't render piece as it would overwrite the pushed-down cells
        active_dirty = false;
    }

    active_stall(active_AREDelay[pieceBottom]);
}

void active_fall() {
    active_gravityTicks--;

    if (!active_gravityTicks) {
        active_gravityTicks = active_getFallDelay();
        
        active_markDirty();
        
        active_transform.y++;
        active_dirty = true;

        if (active_isColliding()) {
            active_transform.y--;
            active_place();
            // next piece is spawned after ARE
        }
    }
}

void active_attemptMove(const signed movement) {
    const unsigned old = active_transform.x;
    const unsigned new = active_transform.x + movement;

    active_transform.x = new;
    const bool isColliding = active_isColliding();
    active_transform.x = old;

    if (isColliding)
        return;

    active_markDirty();
    active_transform.x = new;
    active_dirty = true;
}

void active_attemptRotation(const signed rotation) {
    const unsigned old = active_transform.rotation;
    const unsigned new = (active_transform.rotation + rotation) % active_piece.rotationCount;

    active_transform.rotation = new;
    const bool isColliding = active_isColliding();
    active_transform.rotation = old;

    if (isColliding)
        return;

    active_markDirty();
    active_transform.rotation = new;
    active_dirty = true;
}

void active_handleDAS() {
    active_das.moveLeft = false;
    active_das.moveRight = false;

    if (input_current.left) {
        active_das.leftTicks++;
        if (active_das.leftTicks == 16) {
            active_das.moveLeft = true;
            active_das.leftTicks = 10;
        }
    } else active_das.leftTicks = 0;

    if (input_current.right) {
        active_das.rightTicks++;
        if (active_das.rightTicks == 16) {
            active_das.moveRight = true;
            active_das.rightTicks = 10;
        }
    } else active_das.rightTicks = 0;
}

void active_notStalled() {
    const signed rotation = (
        (input_current.a && !input_last.a) -
        (input_current.b && !input_last.b)
    );

    if (rotation)
        active_attemptRotation(rotation);

    const signed movement = (
        (input_current.right && !input_last.right) -
        (input_current.left && !input_last.left)
    ) + (
        active_das.moveRight -
        active_das.moveLeft
    );

    if (movement)
        active_attemptMove(movement);

    active_fall();
}

void active_tick() {
    active_nextHash();
    active_handleDAS();

    if (active_stallTicks) {
        active_stallTicks--;

        if (!active_stallTicks)
            active_spawnNextPiece();
    } else {
        active_notStalled();
    }
}

void active_render() {
    if (!active_dirty)
        return;

    const unsigned rotationOffset = active_transform.rotation * 16;

    for (unsigned dy = active_transform.y < 2 ? 2 - active_transform.y : 0; dy < active_piece.size; dy++)
        for (unsigned dx = 0; dx < active_piece.size; dx++)
            if (active_piece.rotations[rotationOffset + dy*4 + dx])
                cell_render(active_piece.cellType, active_transform.x + dx, active_transform.y + dy);

    active_dirty = false;
    main_queryRefresh();
}

#endif