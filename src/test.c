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
#include "tasks.h"
#include "pwm.h"
#include "usart.h"

#define DELAY_MAX 127

pinconf_t outpins[PINCOUNT];
pinconf_t addrpins[ADDRCOUNT];

uint8_t outbuf[BUFFER_SIZE];
static uint8_t trigger = 0;
pinconf_t sensor_io;


void my_delay(uint16_t ms)
{
    while(ms)
    {
        _delay_ms(1);
        --ms;
    }
}


void test_pwm(uint8_t val)
{
    set_pwm_percent(val);
    for(int i = 0; i < 200; i++)
    {
        display_7seg_4digit_number(val);
        my_delay(1);
    }
    set_pwm_percent(0);
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
    for(int x = 0; x < 9999; x+=73)
        display_7seg_4digit_number(x);
}


ISR (INT0_vect)
{
    trigger = 1;
}


void INT0_init(void)
{
    // Configure INT0 to trigger on falling edge
    EICRA |= (1 << ISC01);
    EICRA &= ~(1 << ISC00);

    // enable INT0
    EIMSK |= (1 << INT0);
}


int main(void)
{
    uint8_t pin_state;
    // stackpointer init
    SP = RAMEND;

    // configure interrupt for sensor pin
    //INT0_init();

    usart_init(19200);

    // enable global interrupts
    sei();

    usart_write_str("\r\n*********************************");
    usart_write_str("\r\n**                             **");
    usart_write_str("\r\n**     welcome to avr-uno!     **");
    usart_write_str("\r\n**                             **");
    usart_write_str("\r\n*********************************\r\n");

    usart_write_str("\r\ntesting 7segment display\r\n");
    init_7seg();
    test_7seg();

    usart_write_str("pwm setup\r\n");
    config_pwm(0);
    //usart_write_str("test pwm\r\n");
    //test_pwm(42);

    // sensor pin init
    init_input(&sensor_io, PD2, &PIND, &PORTD, &DDRD);

    usart_write_str("running 7seg display + tasks\r\n");
    while(1)
    {
        tasks(&sensor_io);
        _delay_ms(1);
    }

    return 0;
}

