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

#ifndef _7SEG_H_
#define _7SEG_H_

#define ADDRCOUNT 4
#define PINCOUNT 7


extern pinconf_t outpins[PINCOUNT];
extern pinconf_t addrpins[ADDRCOUNT];

extern void set_7seg_pin(pinconf_t *gpio);
extern void reset_7seg_pin(pinconf_t *gpio);
extern void reset_7seg_pins(pinconf_t *gpio);

#endif // _7SEG_H_
