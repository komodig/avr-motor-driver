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
#include <stdint.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#define F_CPU 12000000UL

#include <util/delay.h>
#include "gpio.h"
#include "7seg.h"
#include "pwm.h"
#include "usart.h"

#define DELAY_MAX 127

#define PINCOUNT 11
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

    DDRB |= (1 << PB2 | 1 << PB1 | 1 << PB0);
    DDRD |= (1 << PD7 | 1 << PD6 | 1 << PD5 | 1 << PD4 | 1 << PD3 | 1 << PD2);

    for(x = 1; x < 3; x++)
    {
        /* turn LEDs off */
        PORTB &= ~(1 << PB2 | 1 << PB1 | 1 << PB0);
        PORTD &= ~(1 << PD7 | 1 << PD6 | 1 << PD5 | 1 << PD4 | 1 << PD3 | 1 << PD2);
        _delay_ms(111);
        /* turn LEDs on */
        PORTB |= (1 << PB2 | 1 << PB1 | 1 << PB0);
        PORTD |= (1 << PD7 | 1 << PD6 | 1 << PD5 | 1 << PD4 | 1 << PD3 | 1 << PD2);
        _delay_ms(111);
    }

    /* turn LEDs off */
    PORTB &= ~(1 << PB2 | 1 << PB1 | 1 << PB0);
    PORTD &= ~(1 << PD7 | 1 << PD6 | 1 << PD5 | 1 << PD4 | 1 << PD3 | 1 << PD2);
}

void test_7_seg(void)
{
    for(int x = 0; x < PINCOUNT; x++)
    {
        set_7seg_pin(outpins + x);
        my_delay(300);
        reset_7seg_pin(outpins + x);
        my_delay(300);
    }
}


void test_pwm(void)
{
    uint8_t x;

    config_pwm(5);
    for(x = 35; x < 55; x++)
    {
        set_pwm_percent(x);
        my_delay(1000);
    }
    for(x = 55; x > 35; x--)
    {
        set_pwm_percent(x);
        my_delay(1000);
    }
    disable_pwm();
}


int main(void)
{
    test_output();

    init_output(&outpins[0], PB5, &PORTB, &DDRB); // Yg
    init_output(&outpins[1], PB4, &PORTB, &DDRB); // Yr
    /* PB3 is reserved for PWM out */
    init_output(&outpins[2], PB2, &PORTB, &DDRB); // Ag
    init_output(&outpins[3], PB1, &PORTB, &DDRB); // Ar
    init_output(&outpins[4], PB0, &PORTB, &DDRB); // notYg
    init_output(&outpins[5], PD7, &PORTD, &DDRD); // Fg
    init_output(&outpins[6], PD6, &PORTD, &DDRD); // Ir
    init_output(&outpins[7], PD5, &PORTD, &DDRD); // Ig
    init_output(&outpins[8], PD4, &PORTD, &DDRD); // Cr
    init_output(&outpins[9], PD3, &PORTD, &DDRD); // Cg
    init_output(&outpins[10], PD2, &PORTD, &DDRD); // Fr

    test_7_seg();
    my_delay(500);
    test_7_seg();
    my_delay(500);

    usart_init(19200);
    sei();
    usart_write_str("welcome to avr-uno!\r\n");

    //usart_write_str("test pwm\r\n");
    config_pwm(0);
    /* e.g. set PWM for 50% duty cycle by ocra2_val = 128 */

    while(1)
    {
        test_pwm();
        my_delay(3000);
    }

    return 0;
}

