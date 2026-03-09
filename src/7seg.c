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

void reset_7seg_pins(pinconf_t *gpio)
{
    for(int j = 0; j < ADDRCOUNT; j++)
        reset_7seg_pin(addrpins + j);
}
