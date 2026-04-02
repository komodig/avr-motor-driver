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
#include <stdint.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#define F_CPU 12000000UL

#include <util/delay.h>
#include "gpio.h"
#include "pwm.h"
#include "usart.h"

#define DELAY_MAX 127

#define PINCOUNT 16
#define RISE 1
#define FALL 0


pinconf_t outpins[PINCOUNT];
static uint8_t level = 0, direction = RISE;


void my_delay(uint16_t ms)
{
    while(ms)
    {
        _delay_ms(1);
        --ms;
    }
}


void test_output(void)
{
    uint8_t x;

    DDRB |= (1 << PB3 | 1 << PB2 | 1 << PB1 | 1 << PB0);
    DDRD |= (1 << PD7 | 1 << PD6 | 1 << PD5 | 1 << PD4 | 1 << PD3 | 1 << PD2);

    for(x = 1; x < 3; x++)
    {
        /* turn LEDs off */
        PORTB &= ~(1 << PB3 | 1 << PB2 | 1 << PB1 | 1 << PB0);
        PORTD &= ~(1 << PD7 | 1 << PD6 | 1 << PD5 | 1 << PD4 | 1 << PD3 | 1 << PD2);
        _delay_ms(111);
        /* turn LEDs on */
        PORTB |= (1 << PB3 | 1 << PB2 | 1 << PB1 | 1 << PB0);
        PORTD |= (1 << PD7 | 1 << PD6 | 1 << PD5 | 1 << PD4 | 1 << PD3 | 1 << PD2);
        _delay_ms(111);
    }

    /* turn LEDs off */
    PORTB &= ~(1 << PB3 | 1 << PB2 | 1 << PB1 | 1 << PB0);
    PORTD &= ~(1 << PD7 | 1 << PD6 | 1 << PD5 | 1 << PD4 | 1 << PD3 | 1 << PD2);
}


void test_pwm(void)
{
    uint8_t x, y;

    usart_write_str("test pwm\r\n");
    for(y = 0; y < 3; y++)
    {
        for(x = 0; x < 128; x++)
        {
            config_pwm(x);
            _delay_ms(10);
        }
        for(x = 128; x > 0; x--)
        {
            config_pwm(x);
            _delay_ms(10);
        }
    }
    _delay_ms(300);
    disable_pwm();
}


int main(void)
{
    uint8_t x, y;

    test_output();

    init_output(&outpins[0], PD7, &PORTD, &DDRD);

    usart_init(19200);
    sei();
    usart_write_str("welcome to avr-uno!\r\n");

    x = 0;

    while(1)
    {
        set_pin(outpins + x);
        _delay_ms(300);
        reset_pin(outpins + x);
        _delay_ms(300);
        set_pin(outpins + x);
        _delay_ms(300);
        reset_pin(outpins + x);
        _delay_ms(1500);
    }

    return 0;
}

