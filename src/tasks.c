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

#define BSIZE 32

#define PWM_MIN 34
#define PWM_MAX 38 

#define HIGH 1
#define LOW  0
#define NONE -1

#define RISE 1
#define FALL 0


uint8_t outbuf[BUFFER_SIZE];
uint16_t turns = 0;


uint8_t x_period(void)
{
    /*
     * a period means pwm rise+fall from min to max and back again
     */
    static uint8_t x = PWM_MIN, v = RISE;
    
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


int get_pin_state(pinconf_t *sensor_io)
{
    static uint16_t old_state = NONE, high = 0, low = 0;
    int val = read_pin(sensor_io);
    
    if(val & (1 << sensor_io->pin))
        high++;
    else
        low++;

    if(high == 0xffff)
        usart_write_str("ERROR! high-state counter overflow!\r\n");
    if(low == 0xffff)
        usart_write_str("ERROR! low-state counter overflow!\r\n");

    // do not consider the pin has a state before at least 5 read-outs
    // assume state if either high or low count 5 more measurements than the other
    //if(high >= 5 && high - low >= 5)
    if(high >= 5 && old_state != HIGH) 
    {
        /*
        usart_write_str("pin toggle!\r\n");
        snprintf(outbuf, BUFFER_SIZE, "high: %d low: %d state:%d %d\r\n", high, low, sensor_io->state, val);
        usart_write_str(outbuf);
        */
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
        /*
        usart_write_str("pin toggle!\r\n");
        snprintf(outbuf, BUFFER_SIZE, "high: %d low: %d state:%d %d turns: %d\r\n", high, low, sensor_io->state, val, turns);
        usart_write_str(outbuf);
        */

        old_state = LOW;
        high = low = 0;
        return LOW;
    }
   
    return NONE; 
}


uint8_t display_scheduler(uint16_t x)
{
    static uint8_t state = 0;
    
    switch(state++)
    {
        case 0:
            display_next_7seg_number_digit(0, x);
            break;
        case 1:
            display_next_7seg_number_digit(1, x);
            break;
        case 2:
            display_next_7seg_number_digit(2, x);
            break;
        case 3:
            display_next_7seg_number_digit(3, x);
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
            state = 0;
            break;
    }

    return state;
}


uint8_t tasks(pinconf_t *sensor_io)
{
    static uint16_t loops = 0;
    static uint16_t x = PWM_MIN;
    
    get_pin_state(sensor_io);
    display_scheduler(x);

    if(loops == 0)
    { 
        x = x_period();
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

