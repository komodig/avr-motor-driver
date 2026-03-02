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

#ifndef _TIMER_H_
#define _TIMER_H_
extern void timer_init(void);
extern void timer_start(uint16_t prescaler);
extern void timer_stop(void);
extern void timer_restart(uint16_t prescaler);
extern void timer_comparator_set(uint8_t val);
extern uint32_t get_count(void);
#endif // _TIMER_H_
