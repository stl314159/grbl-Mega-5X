// Microstep control
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#include "microstep_control.h"
#include "grbl.h"


#if HAS_MICROSTEPS

/**
 * Software-controlled Microstepping
 */

void microstep_init() {
    MS_DDR(0, 1) |= (1<<MS_BIT(0, 1));
    MS_DDR(0, 2) |= (1<<MS_BIT(0, 2));

    MS_DDR(1, 1) |= (1<<MS_BIT(1, 1));
    MS_DDR(1, 2) |= (1<<MS_BIT(1, 2));

    MS_DDR(2, 1) |= (1<<MS_BIT(2, 1));
    MS_DDR(2, 2) |= (1<<MS_BIT(2, 2));

#if N_AXIS > 3
    MS_DDR(3, 1) |= (1<<MS_BIT(3, 1));
    MS_DDR(3, 2) |= (1<<MS_BIT(3, 2));
#endif

#if N_AXIS > 4
    MS_DDR(4, 1) |= (1<<MS_BIT(4, 1));
    MS_DDR(4, 2) |= (1<<MS_BIT(4, 2));
#endif

    static const uint8_t microstep_modes[] = MICROSTEP_MODES;
    uint8_t i;
    for (i = 0; i < N_AXIS; i++) {
        microstep_mode(i, microstep_modes[i]);
    }
}

void write(volatile uint8_t *port, uint8_t pin, uint8_t value) {
    if (value) {
        *port |= (1<<pin);
    } else {
        *port &= ~(1<<pin);
    }
}

void microstep_ms(const uint8_t driver, const int8_t ms1, const int8_t ms2) {
    if (ms1 == 0 || ms1 == 1) switch (driver) {
        case 0:
            write(&MS_PORT(0, 1), MS_BIT(0, 1), ms1);
            break;
        case 1:
            write(&MS_PORT(1, 1), MS_BIT(1, 1), ms1);
            break;
        case 2:
            write(&MS_PORT(2, 1), MS_BIT(2, 1), ms1);
            break;
        case 3:
            write(&MS_PORT(3, 1), MS_BIT(3, 1), ms1);
            break;
        case 4:
            write(&MS_PORT(4, 1), MS_BIT(4, 1), ms1);
            break;
    }
    if (ms2 == 0 || ms2 == 1) switch (driver) {
        case 0:
            write(&MS_PORT(0, 2), MS_BIT(0, 2), ms1);
            break;
        case 1:
            write(&MS_PORT(1, 2), MS_BIT(1, 2), ms1);
            break;
        case 2:
            write(&MS_PORT(2, 2), MS_BIT(2, 2), ms1);
            break;
        case 3:
            write(&MS_PORT(3, 2), MS_BIT(3, 2), ms1);
            break;
        case 4:
            write(&MS_PORT(4, 2), MS_BIT(4, 2), ms1);
            break;
    }
}

void microstep_mode(const uint8_t driver, const uint8_t stepping_mode) {
    switch (stepping_mode) {
        case 1: microstep_ms(driver, MICROSTEP1); break;
        case 2: microstep_ms(driver, MICROSTEP2); break;
        case 4: microstep_ms(driver, MICROSTEP4); break;
        case 16: microstep_ms(driver, MICROSTEP16); break;
    }
}

#endif // HAS_MICROSTEPS
