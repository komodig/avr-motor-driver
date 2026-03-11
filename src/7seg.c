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
#define F_CPU 12000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "gpio.h"
#include "7seg.h"

/*
 *    _a_
 *   |   |
 *  f|   |b
 *   |_g_|
 *   |   |
 *  e|   |c
 *   |___|
 *     d
 *
 */
void init_7seg(void)
{
    init_output(&outpins[0], PB5, &PORTB, &DDRB);  // g
    init_output(&outpins[1], PB4, &PORTB, &DDRB);  // f
    /* PB3 is reserved for PWM out */
    init_output(&outpins[2], PB2, &PORTB, &DDRB);  // e
    init_output(&outpins[3], PB1, &PORTB, &DDRB);  // d
    init_output(&outpins[4], PB0, &PORTB, &DDRB);  // c
    init_output(&outpins[5], PD7, &PORTD, &DDRD);  // b
    init_output(&outpins[6], PD6, &PORTD, &DDRD);  // a

    init_output(&addrpins[3], PD2, &PORTD, &DDRD); // d1
    init_output(&addrpins[2], PD3, &PORTD, &DDRD); // d2
    init_output(&addrpins[1], PC4, &PORTC, &DDRC); // d3
    init_output(&addrpins[0], PC5, &PORTC, &DDRC); // d4
}

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


void display_7seg_digit(uint8_t digit)
{
    reset_7seg_pins(outpins, PINCOUNT);

    switch(digit)
    {
        default:
        case 0:
            set_7seg_pin(outpins + 1);
            set_7seg_pin(outpins + 2);
            set_7seg_pin(outpins + 3);
            set_7seg_pin(outpins + 4);
            set_7seg_pin(outpins + 5);
            set_7seg_pin(outpins + 6);
            set_7seg_pin(outpins + 7);
            break;
        case 1:
            set_7seg_pin(outpins + 4);
            set_7seg_pin(outpins + 5);
            break;
        case 2:
            set_7seg_pin(outpins + 0);
            set_7seg_pin(outpins + 2);
            set_7seg_pin(outpins + 3);
            set_7seg_pin(outpins + 5);
            set_7seg_pin(outpins + 6);
            break;
        case 3:
            set_7seg_pin(outpins + 0);
            set_7seg_pin(outpins + 3);
            set_7seg_pin(outpins + 4);
            set_7seg_pin(outpins + 5);
            set_7seg_pin(outpins + 6);
            break;
        case 4:
            set_7seg_pin(outpins + 0);
            set_7seg_pin(outpins + 1);
            set_7seg_pin(outpins + 4);
            set_7seg_pin(outpins + 5);
            break;
        case 5:
            set_7seg_pin(outpins + 0);
            set_7seg_pin(outpins + 1);
            set_7seg_pin(outpins + 3);
            set_7seg_pin(outpins + 4);
            set_7seg_pin(outpins + 6);
            break;
        case 6:
            set_7seg_pin(outpins + 0);
            set_7seg_pin(outpins + 1);
            set_7seg_pin(outpins + 2);
            set_7seg_pin(outpins + 3);
            set_7seg_pin(outpins + 4);
            set_7seg_pin(outpins + 6);
            break;
        case 7:
            set_7seg_pin(outpins + 4);
            set_7seg_pin(outpins + 5);
            set_7seg_pin(outpins + 6);
            break;
        case 8:
            set_7seg_pin(outpins + 0);
            set_7seg_pin(outpins + 1);
            set_7seg_pin(outpins + 2);
            set_7seg_pin(outpins + 3);
            set_7seg_pin(outpins + 4);
            set_7seg_pin(outpins + 5);
            set_7seg_pin(outpins + 6);
            break;
        case 9:
            set_7seg_pin(outpins + 0);
            set_7seg_pin(outpins + 1);
            set_7seg_pin(outpins + 3);
            set_7seg_pin(outpins + 4);
            set_7seg_pin(outpins + 5);
            set_7seg_pin(outpins + 6);
            break;
    }
}


void display_7seg_4digit_number(uint16_t number)
{
    /*
     *  causes 1 + 6 ms delay
     */
    reset_7seg_pins(outpins, PINCOUNT);
    reset_7seg_pins(addrpins, ADDRCOUNT);

    for(int d = 0; d < ADDRCOUNT; d++)
    {
        set_7seg_pin(addrpins + d);
        display_7seg_digit(number % 10);
        number /=10;
        _delay_ms(1);
        reset_7seg_pins(addrpins, ADDRCOUNT);
        _delay_ms(6);
    }
}


