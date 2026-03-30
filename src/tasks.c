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

#define PWM_MIN 32
#define PWM_MAX 50

#define DEFAULT_TURNS 500
#define DEFAULT_TURNS_BY_20 (DEFAULT_TURNS/20)

#define MIN_READ_OUTS 5

#define HIGH 1
#define LOW  0
#define NONE -1

#define RISE 1
#define FALL 0

#define PWM   1
#define TURNS 2

#define UPDATE_X 0
#define FREEZE_X 1


uint8_t outbuf[BUFFER_SIZE];
uint16_t turns = 0;


uint8_t x_period(int freeze)
{
    /*
     * a period means pwm rise+fall from max to min and back again
     * alternating triangular progression
     */
    static uint8_t x = PWM_MAX, v = RISE;

    if(freeze)
        return x;

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


uint8_t x_period_or_shutdown(uint16_t turns, uint16_t limit, int freeze)
{
    /*
     * high priority shutdown conditions override regular x period calculation
     *
     * @param freeze to not change x but just return it by x_period()
     */
    if(turns >= limit - 1)
    {
        return 0;
    }
    else if(turns >= limit - 5)
    {
        return PWM_MIN;
    }
    else if(turns >= limit - DEFAULT_TURNS_BY_20)
    {
        return PWM_MIN + 1;
    }
    else
    {
        return x_period(freeze);
    }
}


int update_pin_state(pinconf_t *sensor_io)
{
    /*
     * function that should be the only
     * with write permisson on turn count
     */
    static int16_t pin_state = NONE, high = 0, low = 0;
    int val = read_pin(sensor_io);

    if(val)
        high++;
    else
        low++;

    if(high == 0xffff)
        usart_write_str("WARNING! high-state counter overflow!\r\n");
    if(low == 0xffff)
        usart_write_str("WARNING! low-state counter overflow!\r\n");

    // do not consider the pin has a state with less than 5 read-outs
    if(high >= MIN_READ_OUTS && pin_state != HIGH)
    {
        if(val < 1)
            usart_write_str("ERROR val != state !\r\n");

        //usart_write_str("pin toggle lo->hi !\r\n");
        pin_state = HIGH;
        high = low = 0;
        return HIGH;
    }
    else if(low >= MIN_READ_OUTS && pin_state != LOW)
    {
        if(val > 0)
            usart_write_str("ERROR val != state !\r\n");

        if(pin_state == HIGH)
        {
            turns++;
        }
        //usart_write_str("pin toggle hi->lo !\r\n");
        pin_state = LOW;
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
    static uint16_t x = PWM_MAX;
    static uint8_t display = PWM;
    /*
     * functions that are called every loop (presumably 1 loop each millisecond)
     */
    update_pin_state(sensor_io); // most atomic function
    if(display == TURNS)
    {
        x = turns;
    }
    display_scheduler(x);     // time scale down by factor 10

    /*
     * functions called in much bigger intervals
     */
    if(loops == 0)
    {
        if(display == PWM)
        {
            x = x_period_or_shutdown(turns, DEFAULT_TURNS, UPDATE_X);
            snprintf(outbuf, BUFFER_SIZE, "pwm %d%%\r\n", x);
        }
        else
            x = x_period_or_shutdown(turns, DEFAULT_TURNS, FREEZE_X);

        set_pwm_percent(x);

        if(display == TURNS)
        {
            snprintf(outbuf, BUFFER_SIZE, "%d turns\r\n", turns);
        }

        usart_write_str(outbuf);
    }
    else if(loops % 100 == 0)
    {
        x = x_period_or_shutdown(turns, DEFAULT_TURNS, FREEZE_X);  // freeze allows this to run frequently just not to miss shutdown condition
        set_pwm_percent(x);
    }

    // condition value should be more dynamic
    if(loops >= 666)
    {
        loops = 0;
        /* alternating display of PWM power cycle and turn count */
        if(display == PWM)
            display = TURNS;
        else
            display = PWM;
    }
    else
        loops++;
}

