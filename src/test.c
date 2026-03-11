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
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#define F_CPU 12000000UL

#include <util/delay.h>
#include "gpio.h"
#include "7seg.h"
#include "pwm.h"
#include "usart.h"

#define DELAY_MAX 127

#define RISE 1
#define FALL 0

pinconf_t outpins[PINCOUNT];
pinconf_t addrpins[ADDRCOUNT];

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


void test_7seg(void)
{
    for(int x = 0; x <= 9; x++)
    {
        display_7seg_digit(x);

        for(int j = 0; j < 10; j++)
        {
            for(int i = 0; i < ADDRCOUNT; i++)
            {
                set_7seg_pin(addrpins + i);
                _delay_ms(1);
                reset_7seg_pins(addrpins, ADDRCOUNT);
                _delay_ms(6); // switched-off for some time to reduce power consumption
            }
        }
    }
    for(int x = 0; x < 9999; x+=57)
        display_7seg_4digit_number(x);
}


void test_pwm(void)
{
    uint8_t x, t;
    uint8_t pbuf[8], outbuf[32];
    uint16_t delay = 40;

    config_pwm(5);
    for(x = 35; x < 55; x++)
    {
        set_pwm_percent(x);
        snprintf(outbuf, 32, "pwm %s%%\r\n", itoa(x, pbuf, 10));
        usart_write_str(outbuf);

        for(t = 0; t < delay; t++)
            display_7seg_4digit_number(x);
    }
    for(x = 55; x > 35; x--)
    {
        set_pwm_percent(x);
        snprintf(outbuf, 32, "pwm %s%%\r\n", itoa(x, pbuf, 10));
        usart_write_str(outbuf);

        for(t = 0; t < delay; t++)
            display_7seg_4digit_number(x);
    }
    disable_pwm();
}


int main(void)
{
    usart_init(19200);
    sei();
    usart_write_str("welcome to avr-uno!\r\n");

    usart_write_str("\r\ntesting gpio\r\n");
    test_output();

    usart_write_str("\r\ntesting 7segment display\r\n");
    init_7seg();
    reset_7seg_pins(addrpins, ADDRCOUNT);
    reset_7seg_pins(outpins, PINCOUNT);
    test_7seg();


    usart_write_str("testing pwm\r\n");
    config_pwm(0);
    /* e.g. set PWM for 50% duty cycle by ocra2_val = 128 */

    while(1)
    {

        test_pwm();
        my_delay(3000);
    }

    return 0;
}

