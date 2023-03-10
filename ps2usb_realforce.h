#pragma once

#include "quantum.h"

#define XXX KC_NO

void matrix_init_user(void);

/*
 * IBM Terminal keyboard 6110345(122keys)/1392595(102keys)
 * http://geekhack.org/showthread.php?10737-What-Can-I-Do-With-a-Terminal-Model-M
 * http://www.seasip.info/VintagePC/ibm_1391406.html
 *
 * Keymap array:
 *     8 bytes
 *   +---------+
 *  0|         |
 *  :|         | 0x00-0x87
 *  ;|         |
 * 17|         |
 *   +---------+
 */

/*
 * RealForce 91 (JIS)
 */
#define LAYOUT_91( \
    k76,      k05, k06, k04, k0C, k03, k0B, k02, k0A, k01, k09, k78, k07,       k7F, k7E, k00, \
\
    k0E, k16, k1E, k26, k25, k2E, k36, k3D, k3E, k46, k45, k4E, k55, k6A, k66,  k39, k2F, k5E, \
    k0D, k15, k1D, k24, k2D, k2C, k35, k3C, k43, k44, k4D, k54, k5B,            k37, k27, k56, \
    k58, k1C, k1B, k23, k2B, k34, k33, k3B, k42, k4B, k4C, k52, k5D,     k5A,                  \
    k12, k1A, k22, k21, k2A, k32, k31, k3A, k41, k49, k4A, k51, k59,                 k4F,      \
    k14, k19, k11, k67,           k29,           k64, k13, k0F, k1F, k17,       k53, k3F, k47  \
) { \
    { k00, k01, k02, k03, k04, k05, k06, k07 }, \
    { XXX, k09, k0A, k0B, k0C, k0D, k0E, k0F }, \
    { XXX, k11, k12, k13, k14, k15, k16, k17 }, \
    { XXX, k19, k1A, k1B, k1C, k1D, k1E, k1F }, \
    { XXX, k21, k22, k23, k24, k25, k26, k27 }, \
    { XXX, k29, k2A, k2B, k2C, k2D, k2E, k2F }, \
    { XXX, k31, k32, k33, k34, k35, k36, k37 }, \
    { XXX, k39, k3A, k3B, k3C, k3D, k3E, k3F }, \
    { XXX, k41, k42, k43, k44, k45, k46, k47 }, \
    { XXX, k49, k4A, k4B, k4C, k4D, k4E, k4F }, \
    { XXX, k51, k52, k53, k54, k55, k56, XXX }, \
    { k58, k59, k5A, k5B, XXX, k5D, k5E, XXX }, \
    { XXX, XXX, XXX, XXX, k64, XXX, k66, k67 }, \
    { XXX, XXX, k6A, XXX, XXX, XXX, XXX, XXX }, \
    { XXX, XXX, XXX, XXX, XXX, XXX, k76, XXX }, \
    { k78, XXX, XXX, XXX, XXX, XXX, k7E, k7F }  \
}

/*
 * RealForce 89 (JIS)
 */
#define LAYOUT_89( \
    k76,      k05, k06, k04, k0C, k03, k0B, k02, k0A, k01, k09, k78, k07,       k7F, k7E, k00, \
\
    k0E, k16, k1E, k26, k25, k2E, k36, k3D, k3E, k46, k45, k4E, k55, k6A, k66,  k39, k2F, k5E, \
    k0D, k15, k1D, k24, k2D, k2C, k35, k3C, k43, k44, k4D, k54, k5B,            k37, k27, k56, \
    k58, k1C, k1B, k23, k2B, k34, k33, k3B, k42, k4B, k4C, k52, k5D,     k5A,                  \
    k12, k1A, k22, k21, k2A, k32, k31, k3A, k41, k49, k4A, k51, k59,                 k4F,      \
    k14,      k11, k67,           k29,           k64, k13, k0F,      k17,       k53, k3F, k47  \
) { \
    { k00, k01, k02, k03, k04, k05, k06, k07 }, \
    { XXX, k09, k0A, k0B, k0C, k0D, k0E, k0F }, \
    { XXX, k11, k12, k13, k14, k15, k16, k17 }, \
    { XXX, XXX, k1A, k1B, k1C, k1D, k1E, XXX }, \
    { XXX, k21, k22, k23, k24, k25, k26, k27 }, \
    { XXX, k29, k2A, k2B, k2C, k2D, k2E, k2F }, \
    { XXX, k31, k32, k33, k34, k35, k36, k37 }, \
    { XXX, k39, k3A, k3B, k3C, k3D, k3E, k3F }, \
    { XXX, k41, k42, k43, k44, k45, k46, k47 }, \
    { XXX, k49, k4A, k4B, k4C, k4D, k4E, k4F }, \
    { XXX, k51, k52, k53, k54, k55, k56, XXX }, \
    { k58, k59, k5A, k5B, XXX, k5D, k5E, XXX }, \
    { XXX, XXX, XXX, XXX, k64, XXX, k66, k67 }, \
    { XXX, XXX, k6A, XXX, XXX, XXX, XXX, XXX }, \
    { XXX, XXX, XXX, XXX, XXX, XXX, k76, XXX }, \
    { k78, XXX, XXX, XXX, XXX, XXX, k7E, k7F }  \
}
