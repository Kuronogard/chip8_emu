/*******************************************************************************
 *	chip8.h
 *
 *  chip8 emulator main file
 *
 *
 *  This file is part of chip8_emu
 * 
 *  Copyright (C) 2015  Dennis Pinto
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ********************************************************************************/

#ifndef __CHIP8
#define __CHIP8


#include "types.h"
#include <stdlib.h>
#include <stdio.h>

#define CLK_HZ 60

#define SCREEN_HEIGHT (32)
#define SCREEN_WIDTH (64) 

#define MEM_SIZE (4096)
#define NUM_REGISTERS (16)
#define CARRY_VREG (15)

#define STACK_SIZE (16)
#define KEYPAD_SIZE (16)

#define SCREEN_SIZE (SCREEN_HEIGHT * SCREEN_WIDTH)

#define CHARSETS_SIZE (80)

#define L_INTERPRETER (0x000)
#define H_INTERPRETER (0x1FF)

#define L_FONTSET (0x050)
#define H_FONTSET (0x0A0)

#define L_MEMORY (0x200)
#define H_MEMORY (0xFFF)

#define PC_INIT_VALUE L_MEMORY



struct chip8_cpu {
	/* Index register */
	uint16 I;

	/* Program counter */
	uint16 pc;

	/* memory */
	byte memory[MEM_SIZE];

	/* general purpose registers */
	byte V[NUM_REGISTERS];

	/* screen memory, each pixel is represented with 1 byte*/
	byte screen[SCREEN_SIZE];

	/* timers */
	byte delay_timer;
	byte sound_timer;

	/* program control stack */
	uint16 stack[STACK_SIZE];

	/* stack pointer */
	uint16 sp;

	/* keypad status */
	byte keypad[KEYPAD_SIZE];
	
	/* flags */
	byte sound;
	byte graphic;
	byte pc_already_updated;
	
	FILE *log_file;
};


struct chip8_cpu *create_chip8_cpu();

void destroy_chip8_cpu(struct chip8_cpu *cpu);

int chip8_load_ROM(struct chip8_cpu *cpu, const char *file);

int chip8_emulate_cyle(struct chip8_cpu *cpu);

#endif /* __CHIP8 */
