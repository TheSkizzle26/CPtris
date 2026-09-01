#ifndef CMT
#define CMT

// Compare Match Timer on the Classpad-II

#include <stdint.h>

// ------------ INTERFACE -------------

typedef enum {
    CMT_COUNTER_SIZE_32_BIT = 0x0,
    CMT_COUNTER_SIZE_16_BIT = 0x1
} cmt_CounterSize;

typedef enum {
    CMT_MATCH_MODE_ONE_SHOT     = 0x0, // timer will only run once
    CMT_MATCH_MODE_FREE_RUNNING = 0x1  // timer will restart after clearing flags
} cmt_MatchMode;

typedef enum {
    CMT_REQUEST_DISABLE = 0x0, // disable DMA transfer request and internal interrupt request
    CMT_REQUEST_DMA     = 0x1, // enable DMA transfer request
    CMT_REQUEST_IIR     = 0x2  // enable internal interrupt request
} cmt_Request;

typedef enum {
    // these are all RCLK (Real CLoK?)
    CMT_CLOCK_SELECT_DIV_8   = 0x4,
    CMT_CLOCK_SELECT_DIV_32  = 0x5,
    CMT_CLOCK_SELECT_DIV_128 = 0x6
} cmt_ClockSelect;

// CMT start register
typedef union {
    struct {
        uint16_t _reserved0 : 10;
        uint16_t start      : 1;
        uint16_t _reserved1 : 5;
    };

    uint16_t raw;
} cmt_StartRegister;

// CMT control/status register
typedef union {
    struct {
        uint16_t matchFlag              : 1;
        uint16_t overflowFlag           : 1;
        uint16_t writeStateFLag         : 1;
        uint16_t _reserved0             : 3;
        cmt_CounterSize counterSize     : 1;
        cmt_MatchMode matchMode         : 1;
        uint16_t outputInterruptEnable  : 1;
        uint16_t _reserved1             : 1;
        cmt_Request matchRequest        : 2;
        uint16_t _reserved2             : 1;
        cmt_ClockSelect clockSelect     : 3;
    };

    uint16_t raw;
} cmt_ControlRegister;

#define CMT_START ((volatile cmt_StartRegister *)0xA44A0000)
#define CMT_CONTROL ((volatile cmt_ControlRegister *)0xA44A0060)
#define CMT_COUNT ((volatile uint32_t *) 0xA44A0064)
#define CMT_TARGET ((volatile uint32_t *) 0xA44A0068)
#define CMT_TICKS_PER_SECOND 3600000

// ---------- IMPLEMENTATION ----------

void cmt_set(const uint32_t constant, const cmt_MatchMode cmm, const cmt_Request cmr) {
    CMT_START->start = 0;
    *CMT_TARGET = constant;
    *CMT_COUNT = 0;

    cmt_ControlRegister tempControl = { .raw = 0 };
    tempControl.counterSize = CMT_COUNTER_SIZE_32_BIT;
    tempControl.matchMode = cmm;
    tempControl.matchRequest = cmr;
    tempControl.clockSelect = CMT_CLOCK_SELECT_DIV_8;

    CMT_CONTROL->raw = tempControl.raw;
}

void cmt_start() {
    CMT_CONTROL->matchFlag = 0;
    CMT_START->start = 1;
}

void cmt_stop() {
    CMT_START->start = 0;
    *CMT_COUNT = 0;
    CMT_CONTROL->raw = 0;
}

void cmt_wait() {
    while (!CMT_CONTROL->matchFlag) { }
}

#endif