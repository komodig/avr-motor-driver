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
#include <stdio.h>
#include "gpio.h"
#include "pwm.h"
#include "7seg.h"
#include "usart.h"

#define PWM_MIN 34
#define PWM_MAX 45

#define DEFAULT_TURNS 300
#define MIN_READ_OUTS 5

#define HIGH 1
#define LOW  0
#define NONE -1

#define RISE 1
#define FALL 0


uint8_t outbuf[BUFFER_SIZE];
uint32_t turns = 0;


uint8_t x_period(void)
{
    /*
     * a period means pwm rise+fall from min to max and back again
     */
    static uint8_t x = PWM_MAX, v = RISE;

    if(v == RISE)
        x++;
    else
        x--;

    if(x >= PWM_MAX && v == RISE)
        v = FALL;

    if(x <= PWM_MIN && v == FALL)
        v = RISE;

    return x;
}


uint8_t x_period_limited(uint32_t turns, uint32_t limit)
{
    if(turns >= limit-5)
    {
        return 0;
    }
    if(turns >= limit-80)
    {
        snprintf(outbuf, BUFFER_SIZE, "pwm ramp down %d%%\r\n", PWM_MIN);
        usart_write_str(outbuf);
        return PWM_MIN;
    }

    return x_period();
}


int get_pin_state(pinconf_t *sensor_io)
{
    static uint16_t old_state = NONE, high = 0, low = 0;
    int val = read_pin(sensor_io);

    if(val)
        high++;
    else
        low++;

    if(high == 0xffff)
        usart_write_str("ERROR! high-state counter overflow!\r\n");
    if(low == 0xffff)
        usart_write_str("ERROR! low-state counter overflow!\r\n");

    // do not consider the pin has a state with less than 5 read-outs
    if(high >= MIN_READ_OUTS && old_state != HIGH)
    {
        //usart_write_str("pin toggle!\r\n");
        old_state = HIGH;
        high = low = 0;
        return HIGH;
    }

    if(low >= 5 && old_state != LOW)
    {
        if(old_state == HIGH)
        {
            turns++;
        }
        //usart_write_str("pin toggle!\r\n");
        old_state = LOW;
        high = low = 0;
        return LOW;
    }

    return NONE;
}


uint8_t display_scheduler(uint16_t x)
{
    /*
     * assume 1 function call every 1 millisecond
     * the resetting cases are to switch display
     * off for most cycles to reduce power
     * consumption and keep the temperature of
     * the voltage regulator low.
     */
    static uint8_t millisecs = 0;

    switch(millisecs++)
    {
        case 0:
            display_7seg_number_digit(0, x);
            break;
        case 1:
            display_7seg_number_digit(1, x);
            break;
        case 2:
            display_7seg_number_digit(2, x);
            break;
        case 3:
            display_7seg_number_digit(3, x);
            break;
        case 4:
            reset_all_7seg_pins();
            break;
        case 5:
            reset_all_7seg_pins();
            break;
        case 6:
            reset_all_7seg_pins();
            break;
        case 7:
            reset_all_7seg_pins();
            break;
        case 8:
            reset_all_7seg_pins();
            break;
        case 9:
            reset_all_7seg_pins();
            break;
        default:
            millisecs = 0;
            break;
    }

    return millisecs;
}


uint8_t tasks(pinconf_t *sensor_io)
{
    static uint16_t loops = 0;
    static uint16_t x = PWM_MIN;

    /*
     * functions that are called every loop (presumably 1 loop each millisecond)
     */
    get_pin_state(sensor_io); // most atomic function
    display_scheduler(x);     // time scale down by factor 10

    /*
     * functions called in much bigger intervals
     */
    if(loops == 0)
    {
        x = x_period_limited(turns, DEFAULT_TURNS);
        set_pwm_percent(x);

        snprintf(outbuf, BUFFER_SIZE, "pwm %d%%\r\n", x);
        usart_write_str(outbuf);
    }
    if(loops >= 700)
    {
        x = turns;

        if(loops == 700)
        {
            snprintf(outbuf, BUFFER_SIZE, "%d turns\r\n", x);
            usart_write_str(outbuf);
        }
    }

    if(loops >= 2700)
        loops = 0;
    else
        loops++;
}

