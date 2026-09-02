#ifndef CPLIB_BASE
#define CPLIB_BASE

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// ------------ INTERFACE -------------

// keys are defined in the system files

#define cp_likely(x) __builtin_expect(!!(x), 1)
#define cp_unlikely(x) __builtin_expect(!!(x), 0)

uint16_t *cp_pixels;
size_t cp_tick = 0;

// system-specific
void *cp_allocate(size_t size);
void cp_free(void *pointer);
void cp_copyMemory(void *source, size_t size, void *destination);
void cp_setMemory(uint8_t value, size_t count, void *target);
void cp_init();
void cp_quit();
void cp_setTargetFPS(int fps);
void cp_refreshScreen();
void cp_finishFrame();
bool cp_isKeyDown(uint32_t key);

int cp_getRenderWidth();
int cp_getRenderHeight();
int cp_getPixelCount();
size_t cp_getTick();

// ---------- IMPLEMENTATION ----------

#ifdef SYSTEM_CALC
#include <_cp_base_calc.c>
#endif

#ifdef SYSTEM_PC
#include <_cp_base_pc.c>
#endif

int cp_getRenderWidth() {
    return 320;
}

int cp_getRenderHeight() {
    return 528;
}

int cp_getPixelCount() {
    return cp_getRenderWidth() * cp_getRenderHeight();
}

size_t cp_getTick() {
    return cp_tick;
}

#endif