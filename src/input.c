#ifndef INPUT
#define INPUT

#include <stdbool.h>

// ------------ INTERFACE -------------

typedef struct {
    bool up;
    bool down;
    bool left;
    bool right;
    bool a;
    bool b;
} input_Keys;

input_Keys input_current;
input_Keys input_last;

void input_tick();

// ---------- IMPLEMENTATION ----------

#include <cp_base.c>

void input_tick() {
    input_last = input_current;

    input_current.up = cp_isKeyDown(CP_KEY_8);
    input_current.down = cp_isKeyDown(CP_KEY_2);
    input_current.left = cp_isKeyDown(CP_KEY_4);
    input_current.right = cp_isKeyDown(CP_KEY_6);
    input_current.a = cp_isKeyDown(CP_KEY_EXE);
    input_current.b = cp_isKeyDown(CP_KEY_EXP);
}

#endif