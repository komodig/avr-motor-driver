/*
 * Copyright (C) 2017-2021 Lutz Ballaschke
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _GPIO_H_
#define _GPIO_H_

typedef enum colored_letter {
    F_RED = 1,
    C_RED,
    K_RED,
    K2_RED,
    B_RED,
    A_RED,
    R_RED,
    Y_RED,
    F_GREEN,
    C_GREEN,
    K_GREEN,
    K2_GREEN,
    B_GREEN,
    A_GREEN,
    R_GREEN,
    Y_GREEN
} letters_t;

typedef struct {
    uint8_t pin;
    volatile uint8_t *port;
    uint8_t state;
    volatile uint8_t *dir_reg;
    uint8_t *letters;

} pinconf_t;

extern void init_output(pinconf_t *outpin,
                        uint8_t gpio,
                        volatile uint8_t *port,
                        volatile uint8_t *ddreg,
                        uint8_t *letters);
extern void set_pin(pinconf_t *gpio);
extern void reset_pin(pinconf_t *gpio);
extern uint8_t is_letter(pinconf_t *outpin, letters_t letter_needed);

#endif // _GPIO_H_
