#ifndef INPUT
#define INPUT

#include <stdbool.h>

// ------------ INTERFACE -------------

typedef struct {
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

    input_current.left = cp_isKeyDown(CP_KEY_1);
    input_current.right = cp_isKeyDown(CP_KEY_2);
    input_current.a = cp_isKeyDown(CP_KEY_EXE);
    input_current.b = cp_isKeyDown(CP_KEY_EXP);
}

#endif