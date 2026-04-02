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

#include <avr/io.h>
#include "gpio.h"
#include "usart.h"

void init_output(pinconf_t *outpin,
        uint8_t gpio,
        volatile uint8_t *port,
        volatile uint8_t *ddreg,
        uint8_t *letters)
{
    outpin->pin = gpio;
    outpin->port = port;
    outpin->state = 0;
    outpin->dir_reg = ddreg;
    outpin->letters = letters;

    *ddreg |= (1 << gpio); /* configure gpio as output */
}

inline void set_pin(pinconf_t *gpio)
{
    *gpio->port |= (1 << gpio->pin);
}

inline void reset_pin(pinconf_t *gpio)
{
    *gpio->port &= ~(1 << gpio->pin);
}

inline uint8_t is_letter(pinconf_t *outpin, letters_t letter_needed)
{
    uint8_t *letter = outpin->letters;
    while(*letter != '\0')
    {
        if(letter_needed == *letter)
            return 1;

        ++letter;
    }
    return 0;
}
