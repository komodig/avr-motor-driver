/*
 * Copyright (C) 2017-2026 Lutz Ballaschke
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

#include <avr/io.h>
#include "gpio.h"
#include "7seg.h"

/*
 *
 * 7-segment LED display driver.
 * works like gpio but everythin inverted
 * because the circuit has common +5V
 * and needs to be set 0V to be illuminated.
 *
 */

inline void set_7seg_pin(pinconf_t *gpio)
{
    *gpio->port &= ~(1 << gpio->pin);   /* inverted pgio */
}


inline void reset_7seg_pin(pinconf_t *gpio)
{
    *gpio->port |= (1 << gpio->pin);    /* inverted pgio */
}


void reset_7seg_pins(pinconf_t *base_addr, uint8_t pin_count)
{
    for(int i = 0; i < pin_count; i++)
        reset_7seg_pin(base_addr + i);
}


void set_7seg_digit(uint8_t digit)
{
    reset_7seg_pins(outpins, PINCOUNT);

    switch(digit)
    {
        case 0:
            set_7seg_pin(outpins + 1);
            set_7seg_pin(outpins + 2);
            set_7seg_pin(outpins + 3);
            set_7seg_pin(outpins + 4);
            set_7seg_pin(outpins + 5);
            set_7seg_pin(outpins + 6);
            set_7seg_pin(outpins + 7);
        case 1:
            set_7seg_pin(outpins + 1);
            set_7seg_pin(outpins + 2);
        case 2:
            set_7seg_pin(outpins + 1);
        case 3:
            set_7seg_pin(outpins + 1);
        case 4:
            set_7seg_pin(outpins + 1);
        case 5:
            set_7seg_pin(outpins + 1);
        case 6:
            set_7seg_pin(outpins + 1);
        case 7:
            set_7seg_pin(outpins + 1);
        case 8:
            set_7seg_pin(outpins + 1);
        case 9:
            set_7seg_pin(outpins + 1);
    }
}


void set_7seg_4digit_number(uint16_t number)
{
    reset_7seg_pins(outpins, PINCOUNT);
}


