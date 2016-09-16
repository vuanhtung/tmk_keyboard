/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

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

/*
 * scan matrix
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "i2c.h"
#include "serial.h"
#include "split-util.h"
#include "pro-micro.h"
#include "config.h"

#ifndef DEBOUNCE
#   define DEBOUNCE	5
#endif

#define ERROR_DISCONNECT_COUNT 5

static uint8_t debouncing = DEBOUNCE;
static uint8_t error_count = 0;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static matrix_row_t read_cols(void);
static void init_cols(void);
static void unselect_rows(void);
static void select_row(uint8_t row);

inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

void matrix_init(void)
{
    debug_enable = true;
    debug_matrix = true;
    debug_mouse = true;
    // initialize row and col
    unselect_rows();
    init_cols();

    TX_RX_LED_INIT;

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }
}

uint8_t _matrix_scan(void)
{
    // Right hand is stored after the left in the matirx so, we need to offset it
    int offset = isLeftHand ? 0 : (ROWS_PER_HAND);

    for (uint8_t i = 0; i < ROWS_PER_HAND; i++) {
        select_row(i);
        _delay_us(30);  // without this wait read unstable value.
        matrix_row_t cols = read_cols();
        if (matrix_debouncing[i+offset] != cols) {
            matrix_debouncing[i+offset] = cols;
            debouncing = DEBOUNCE;
        }
        unselect_rows();
    }

    if (debouncing) {
        if (--debouncing) {
            _delay_ms(1);
        } else {
            for (uint8_t i = 0; i < ROWS_PER_HAND; i++) {
                matrix[i+offset] = matrix_debouncing[i+offset];
            }
        }
    }

    return 1;
}

// Get rows from other half over i2c
int i2c_transaction(void) {
    int slaveOffset = (isLeftHand) ? (ROWS_PER_HAND) : 0;

    int err = i2c_master_start(SLAVE_I2C_ADDRESS + I2C_WRITE);
    if (err) goto i2c_error;

    // start of matrix stored at 0x00
    err = i2c_master_write(0x00);
    if (err) goto i2c_error;

    // Start read
    err = i2c_master_start(SLAVE_I2C_ADDRESS + I2C_READ);
    if (err) goto i2c_error;

    if (!err) {
        int i;
        for (i = 0; i < ROWS_PER_HAND-1; ++i) {
            matrix[slaveOffset+i] = i2c_master_read(I2C_ACK);
        }
        matrix[slaveOffset+i] = i2c_master_read(I2C_NACK);
        i2c_master_stop();
    } else {
i2c_error: // the cable is disconnceted, or something else went wrong
        i2c_reset_state();
        return err;
    }

    return 0;
}

int serial_transaction(void) {
    int slaveOffset = (isLeftHand) ? (ROWS_PER_HAND) : 0;

    if (serial_update_buffers()) {
        return 1;
    }

    for (int i = 0; i < ROWS_PER_HAND; ++i) {
        matrix[slaveOffset+i] = serial_slave_buffer[i];
    }
    return 0;
}

uint8_t matrix_scan(void)
{
    int ret = _matrix_scan();



#ifdef USE_I2C
    if( i2c_transaction() ) {
#else
    if( serial_transaction() ) {
#endif
        // turn on the indicator led when halves are disconnected
        TXLED1;

        error_count++;

        if (error_count > ERROR_DISCONNECT_COUNT) {
            // reset other half if disconnected
            int slaveOffset = (isLeftHand) ? (ROWS_PER_HAND) : 0;
            for (int i = 0; i < ROWS_PER_HAND; ++i) {
                matrix[slaveOffset+i] = 0;
            }
        }
    } else {
        // turn off the indicator led on no error
        TXLED0;
        error_count = 0;
    }

    return ret;
}

void matrix_slave_scan(void) {
    _matrix_scan();

    int offset = (isLeftHand) ? 0 : (MATRIX_ROWS / 2);

#ifdef USE_I2C
    for (int i = 0; i < ROWS_PER_HAND; ++i) {
        /* i2c_slave_buffer[i] = matrix[offset+i]; */
        i2c_slave_buffer[i] = matrix[offset+i];
    }
#else
    for (int i = 0; i < ROWS_PER_HAND; ++i) {
        serial_slave_buffer[i] = matrix[offset+i];
    }
#endif
}

bool matrix_is_modified(void)
{
    if (debouncing) return false;
    return true;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    print("\nr/c 0123456789ABCDEF\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        phex(row); print(": ");
        pbin_reverse16(matrix_get_row(row));
        print("\n");
    }
}

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += bitpop16(matrix[i]);
    }
    return count;
}


/* Column pin configuration
 * col: 0   1   2   3   4   5
 * pin: F6  F7  B1  B3  B2  B6
 */
static void  init_cols(void)
{
    const uint8_t MASK_COL_B = (1<<1 | 1<<3 | 1<<2 | 1<<6);
    const uint8_t MASK_COL_F = (1<<6 | 1<<7);

    // Input with pull-up(DDR:0, PORT:1)
    DDRB  &= ~MASK_COL_B;
    PORTB |=  MASK_COL_B;

    DDRF  &= ~MASK_COL_F;
    PORTF |=  MASK_COL_F;
}

static matrix_row_t read_cols(void)
{
    return (PINF&(1<<6) ? 0 : (1<<0)) |
           (PINF&(1<<7) ? 0 : (1<<1)) |
           (PINB&(1<<1) ? 0 : (1<<2)) |
           (PINB&(1<<3) ? 0 : (1<<3)) |
           (PINB&(1<<2) ? 0 : (1<<4)) |
           (PINB&(1<<6) ? 0 : (1<<5));
}

/* Row pin configuration
 * row: 0  1  2  4
 * pin: D7 E6 B4 B5
 */
static void unselect_rows(void)
{
    // Hi-Z(DDR:0, PORT:0) to unselect
    const uint8_t MASK_ROW_B = (1<<4) | (1<<5);
    const uint8_t MASK_ROW_D = (1<<7);
    const uint8_t MASK_ROW_E = (1<<6);

    DDRB  &= ~MASK_ROW_B;
    PORTB &= ~MASK_ROW_B;

    DDRD  &= ~MASK_ROW_D;
    PORTD &= ~MASK_ROW_D;

    DDRE  &= ~MASK_ROW_E;
    PORTE &= ~MASK_ROW_E;
}

static void select_row(uint8_t row)
{
    // Output low(DDR:1, PORT:0) to select
    switch (row) {
        case 0:
            DDRD  |= (1<<7);
            PORTD &= ~(1<<7);
            break;
        case 1:
            DDRE  |= (1<<6);
            PORTE &= ~(1<<6);
            break;
        case 2:
            DDRB  |= (1<<4);
            PORTB &= ~(1<<4);
            break;
        case 3:
            DDRB  |= (1<<5);
            PORTB &= ~(1<<5);
            break;
    }
}
