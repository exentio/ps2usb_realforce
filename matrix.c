/*
Copyright 2011 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include <stdbool.h>
#include "print.h"
#include "util.h"
#include "debug.h"
#include "ps2.h"
#include "matrix.h"
#include <ch.h>
#include <hal.h>

#define print_matrix_row(row)  print_bin_reverse8(matrix_get_row(row))
#define print_matrix_header()  print("\nr/c 01234567\n")
#define ROW_SHIFTER ((uint8_t)1)


static void matrix_make(uint8_t code);
static void matrix_break(uint8_t code);


/*
 * Matrix Array usage:
 * 'Scan Code Set 3' is assigned into 18x8 cell matrix.
 *
 *    8bit wide
 *   +---------+
 *  0|         |
 *  :|         | 0x00-0x87
 *  ;|         |
 * 17|         |
 *   +---------+
 */
static uint8_t matrix[MATRIX_ROWS];
#define ROW(code)      (code>>3)
#define COL(code)      (code&0x07)

// https://github.com/tinkerBOY-git/vial-qmk/blob/dev/keyboards/converter/tinkerboy/ibmpc_usb/matrix.c
static uint8_t cs2_e0code(uint8_t code) {
    switch(code) {
        case 0x11: return 0x0F; // right alt
        case 0x14: return 0x17; // right control
        case 0x1F: return 0x19; // left GUI
        case 0x27: return 0x1F; // right GUI
        case 0x2F: return 0x5C; // apps
        case 0x69: return 0x27; // end
        case 0x6B: return 0x53; // cursor left
        case 0x6C: return 0x2F; // home
        case 0x70: return 0x39; // insert
        case 0x71: return 0x37; // delete
        case 0x72: return 0x3F; // cursor down
        case 0x74: return 0x47; // cursor right
        case 0x75: return 0x4F; // cursor up
        case 0x7A: return 0x56; // page down
        case 0x7D: return 0x5E; // page up
        case 0x7C: return 0x7F; // Print Screen
        case 0x7E: return 0x00; // Control'd Pause

        default: return (code & 0x7F);
    }
}

__attribute__ ((weak))
void matrix_init_user(void) {
}

void matrix_init(void)
{
    debug_enable = true;
    //debug_matrix = true;
    //debug_keyboard = true;
    //debug_mouse = false;

    ps2_host_init();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) matrix[i] = 0x00;

    matrix_init_user();
    return;
}

uint8_t matrix_scan(void)
{
    // scan code reading states
    static enum {
        READY,
        E0_CODE,
        F0_BREAK,
        E0_F0_BREAK,

        // welcome to the circus
        PAUSE_E1,
        PAUSE_E1_14,
        PAUSE_E1_F0,
        PAUSE_E1_F0_14,
        PAUSE_E1_F0_14_F0,

    } state = READY;

    uint8_t code;
    if ((code = ps2_host_recv())) {
        debug("r"); debug_hex(code); debug(" ");
    }
    switch (state) {
        case READY:
            switch (code) {
                case 0x00:
                    break;
                case 0xE0:
                    state = E0_CODE;
                    break;
                case 0xE1:
                    state = PAUSE_E1;
                    break;
                case 0xF0:
                    state = F0_BREAK;
                    debug(" ");
                    break;
                default:    // normal key make
                    state = READY;
                    debug("\n");
                    if (code == 0x83) { // F7
                        matrix_make(0x02);
                        break;
                    }
                    if (code == 0x84) { // Alt+PrintScr
                        matrix_make(0x7F);
                        break;
                    }
                    if (code < 0x88) {
                        matrix_make(code);
                    } else {
                        debug("unexpected scan code at READY: "); debug_hex(code); debug("\n");
                    }
            }
            break;
        case E0_CODE:
            switch (code) {
                case 0x00:
                    break;
                case 0x12:  // Caused by PrintScr
                case 0x59:  // idk but another dude ignores this too lol
                    state = READY;
                    break;
                case 0xF0:
                    state = E0_F0_BREAK;
                    debug(" ");
                    break;
                default:
                    if (code < 0x88)
                        matrix_make(cs2_e0code(code));
                    state = READY;
                    debug("\n");
            }
            break;
        case F0_BREAK:    // Break code
            switch (code) {
                case 0x00:
                    break;
                default:
                    state = READY;
                    debug("\n");
                    if (code == 0x83) { // F7
                        matrix_break(0x02);
                        break;
                    }
                    if (code == 0x84) { // Alt+PrintScr
                        matrix_break(0x7F);
                        break;
                    }
                    if (code < 0x88) {
                        matrix_break(code);
                    } else {
                        debug("unexpected scan code at READY: "); debug_hex(code); debug("\n");
                    }
            }
            break;
        case E0_F0_BREAK:
            switch (code) {
                case 0x00:
                    break;
                case 0x12:
                case 0x59:
                    state = READY;
                    break;
                default:
                    if (code < 0x88)
                        matrix_break(cs2_e0code(code));
                    state = READY;
                    debug("\n");
            }
            break;

        // sigh
        case PAUSE_E1:
            switch (code) {
                case 0x00:
                    break;
                case 0x14:
                    state = PAUSE_E1_14;
                    break;
                case 0xF0:
                    state = PAUSE_E1_F0;
                    break;
                default:
                    state = READY;
            }
            break;
        case PAUSE_E1_14:
            switch (code) {
                case 0x00:
                    break;
                case 0x77:
                    matrix_make(0x00);
                    state = READY;
                    break;
                default:
                    state = READY;
            }
            break;
        case PAUSE_E1_F0:
            switch (code) {
                case 0x00:
                    break;
                case 0x14:
                    state = PAUSE_E1_F0_14;
                    break;
                default:
                    state = READY;
            }
            break;
        case PAUSE_E1_F0_14:
            switch (code) {
                case 0x00:
                    break;
                case 0xF0:
                    state = PAUSE_E1_F0_14_F0;
                    break;
                default:
                    state = READY;
            }
            break;
        case PAUSE_E1_F0_14_F0:
            switch (code) {
                case 0x00:
                    break;
                case 0x77:
                    matrix_break(0x00);
                    state = READY;
                    break;
                default:
                    state = READY;
            }
            break;

    }
    return 1;
}

inline
uint8_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

inline
static void matrix_make(uint8_t code)
{
    if (!matrix_is_on(ROW(code), COL(code))) {
        matrix[ROW(code)] |= 1<<COL(code);
    }
}

inline
static void matrix_break(uint8_t code)
{
    if (matrix_is_on(ROW(code), COL(code))) {
        matrix[ROW(code)] &= ~(1<<COL(code));
    }
}

bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix_get_row(row) & (1<<col));
}

void matrix_print(void)
{
#if (MATRIX_COLS <= 8)
    print("r/c 01234567\n");
#elif (MATRIX_COLS <= 16)
    print("r/c 0123456789ABCDEF\n");
#elif (MATRIX_COLS <= 32)
    print("r/c 0123456789ABCDEF0123456789ABCDEF\n");
#endif

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {

#if (MATRIX_COLS <= 8)
        xprintf("%02X: %08b%s\n", row, bitrev(matrix_get_row(row)),
#elif (MATRIX_COLS <= 16)
        xprintf("%02X: %016b%s\n", row, bitrev16(matrix_get_row(row)),
#elif (MATRIX_COLS <= 32)
        xprintf("%02X: %032b%s\n", row, bitrev32(matrix_get_row(row)),
#endif
#ifdef MATRIX_HAS_GHOST
        matrix_has_ghost_in_row(row) ?  " <ghost" : ""
#else
        ""
#endif
        );
    }
}

#ifdef MATRIX_HAS_GHOST
__attribute__ ((weak))
bool matrix_has_ghost_in_row(uint8_t row)
{
    matrix_row_t matrix_row = matrix_get_row(row);
    // No ghost exists when less than 2 keys are down on the row
    if (((matrix_row - 1) & matrix_row) == 0)
        return false;

    // Ghost occurs when the row shares column line with other row
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        if (i != row && (matrix_get_row(i) & matrix_row))
            return true;
    }
    return false;
}
#endif
