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
#include "pwm.h"

/* Here is an example code of 8-bit Timer2 Fast PWM Mode (8KHz) at 16MHz clock */
void config_pwm(uint16_t ocra2_val)
{
    /* PB3 as output */
    DDRB |= (1 << PB3);
    /* e.g. set PWM for 50% duty cycle by ocra2_val = 128 */
    OCR2A = ocra2_val & 0xFF;
    OCR2B = (ocra2_val >> 8) & 0xFF;
     /* set non-inverting mode */
    TCCR2A |= (1 << COM2A1);
    /* set fast PWM Mode */
    TCCR2A |= (1 << WGM21) | (1 << WGM20);
    /* set prescaler to 8 and starts PWM */
    TCCR2B |= (1 << CS21);
}


void disable_pwm(void)
{
        TCCR2A = TCCR2B = OCR2A = OCR2B = 0x00;
}


