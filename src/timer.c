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
#include <avr/interrupt.h>
#include "led_patterns.h"
#include "timer.h"
#include "led_patterns.h"


/* signal handler for timer interrupt TOV0 */
ISR(TIMER0_OVF_vect)
{
    ISR_OVF_led_patterns();
}

ISR(TIMER0_COMPA_vect)
{
    ISR_CMP_led_patterns();
}

inline void timer_comparator_set(uint8_t val)
{
    OCR0A = val;
}

void timer_init(void)
{
    /*enable timer overflow interrupt and timer compare interrupt*/
    TIMSK0 |= (1 << TOIE0 | 1 << OCIE0A);
    /*set timer counter initial value*/
    TCNT0=0x00;
    /* set default comparison value */
    timer_comparator_set(10);
}

void timer_start(uint16_t prescaler)
{
    switch(prescaler)
    {
        case 0:
            TCCR0B |= (1 << CS00);              /* no prescaler */
            break;
        default:
        case 1:
            TCCR0B |= (1 << CS01);              /* prescaler 8 */
            break;
        case 2:
            TCCR0B |= (1 << CS01 | 1 << CS00);  /* prescaler 64 */
            break;
        case 3:
            TCCR0B |= (1 << CS02);              /* prescaler 256 */
            break;
        case 4:
            TCCR0B |= (1 << CS02 | 1 << CS00);  /* prescaler 1024 */
            break;
    }
}

void timer_stop(void)
{
    TCCR0B &= ~(1 << CS00 | 1 << CS01 | 1 << CS02);
    TCNT0=0x00;
}

void timer_restart(uint16_t prescaler)
{
    timer_stop();
    timer_start(prescaler);
}

