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

#ifndef _UART_H
	#define _UART_H

#define BUFFER_SIZE	50

volatile unsigned char buffercounter;
char usart_rx_buffer[BUFFER_SIZE];
unsigned char rx_check;
volatile unsigned char get_char;

struct {
		volatile unsigned char usart_ready:1;
		volatile unsigned char usart_rx_ovl:1;
		}usart_status ;
		
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <avr/io.h>
//----------------------------------------------------------------------------

//Der Quarz auf dem Experimentierboard
#ifndef SYSCLK
//		#define SYSCLK 14745600UL
		#define SYSCLK 16000000UL
#endif //SYSCLK	
	
	#if defined (__AVR_ATmega8__)
		#define USR UCSRA
		#define UCR UCSRB
		#define UBRR UBRRL
        #define USART_RX USART_RXC_vect 
	#endif
	
void usart_init(unsigned long baudrate); 
void usart_write_char(char c);
void usart_write_str(char *str);
	
void usart_write_P (const char *Buffer,...);
#define usart_write(format, args...)   usart_write_P(PSTR(format) , ## args)
	
//----------------------------------------------------------------------------

#endif //_UART_H

