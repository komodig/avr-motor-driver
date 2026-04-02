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

static uint8_t trigger = 0, direction = RISE;
pinconf_t sensor_io;
uint16_t turns = 0;


void my_delay(uint16_t ms)
{
    while(ms)
    {
        _delay_ms(1);
        --ms;
    }
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
        display_7seg_4digit_number(x, &sensor_io);
}


void test_pwm(void)
{
    uint8_t v, w, x, t, pin_state, pin_toggle;
    uint8_t outbuf[32];
    uint8_t min = 35, max = 53;
    /*
     * each display_7seg_4digit_number call causes 7ms delay
     * multiplied with delay here results in something like
     * 7 * 50 = 350ms loop time
     */
    uint16_t delay = 40;

    usart_write_str("--------------> test_pwm func\r\n");
    config_pwm(5);
    x = min;
    v = RISE;
    while(1)
    {
        set_pwm_percent(x);
        snprintf(outbuf, 32, "pwm %d%%\r\n", x);
        usart_write_str(outbuf);

        for(t = 0; t < delay; t++)
        {
            pin_toggle = display_7seg_4digit_number(x, &sensor_io);
            pin_state = read_pin(&sensor_io);

            if(trigger > 0)
            {
                trigger = 0;  // does nothing atm.
                //usart_write_str("IRQ trigger!\r\n");
            }
            if(pin_toggle > 0)
            {
                pin_toggle = 0;
                usart_write_str("pin toggle!\r\n");
                snprintf(outbuf, 32, "turns: %d state:%d %d\r\n", turns, sensor_io.state, pin_state);
                usart_write_str(outbuf);
            }
        }

        // in the past it was one for-loop for each direction raise/fall
        // the v, x logic allows power rise and fall cycle in a single while-loop
        if(v == RISE)
            x++;
        else
            x--;

        if(x >= max && v == RISE)
            v = FALL;

        if(x < min && v == FALL)
            break;
    }
    disable_pwm();
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
    // stackpointer init
    SP = RAMEND;
    // sensor pin init
    init_input(&sensor_io, PD2, &PIND, &PORTD, &DDRD);

    // configure interrupt for sensor pin
    INT0_init();

    usart_init(19200);

    // enable global interrupts
    sei();

    usart_write_str("\r\nwelcome to avr-uno!\r\n");

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

