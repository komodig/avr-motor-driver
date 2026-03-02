#
# Copyright (C) 2017-2021 Lutz Ballaschke
#
# This is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# <http://www.gnu.org/licenses/>.
#

#!/bin/bash

#
#if [ $# -eq 1 ] ; then
#    echo "arg: $1"
#    FILE=$1
#else
    FILE="test"
#fi

echo "args: $#"
if [[ $# -eq 0 || $# -eq 1 && $1 = "probe" ]] ; then
    avrdude -y -pm328p -cstk500v2 -P/dev/ttyUSB0 -v
elif [[ $# -eq 1 && $1 = "interactive" ]] ; then
    avrdude -y -pm328p -cstk500v2 -P/dev/ttyUSB0 -t
elif [[ $# -eq 1 && $1 = "signat" ]] ; then
    avrdude -y -pm328p -cstk500v2 -P/dev/ttyUSB0 -u 
elif [[ $# -eq 1 && $1 = "write" ]] ; then
    avrdude -y -pm328p -cstk500v2 -P/dev/ttyUSB0 -v -U flash:w:$FILE.hex
elif [[ $# -eq 1 && $1 = "erase" ]] ; then
    avrdude -y -pm328p -cstk500v2 -P/dev/ttyUSB0 -v -e 
elif [[ $# -eq 1 && $1 = "wfuses" ]] ; then
# works: ext.cryst    avrdude -y -pm328p -cstk500v2 -P/dev/ttyUSB0 -v -U lfuse:w:0xcc:m -U hfuse:w:0xd1:m  
#      avrdude -y -pm328p -cstk500v2 -P/dev/ttyUSB0 -v -U lfuse:w:0xc4:m -U hfuse:w:0xd1:m
      avrdude -y -pm328p -cstk500v2 -P/dev/ttyUSB0 -v -U lfuse:w:0xef:m -U hfuse:w:0xd7:m   # high crystal
# btm222 board:   avrdude -y -pm328p -cstk500v2 -P/dev/ttyUSB0 -v -U lfuse:w:0xff:m -U hfuse:w:0xdf:m  
# int.clk  avrdude -y -pm328p -cstk500v2 -P/dev/ttyUSB0 -v -U lfuse:w:0xc4:m -U hfuse:w:0xd1:m  
else
    echo "parameters: probe, interactive, signat, write, wfuses, erase or (none)"
fi
# fusebits: external oscillator: 1111

