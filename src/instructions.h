/*******************************************************************************
 *	instructions.h
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
 
#ifndef __INSTRUCTIONS
#define __INSTRUCTIONS
 
#include "types.h"
#include "chip8.h"
 
#define RCA 	0x0000
#define CLRS	0x00E0
#define BL		0x00EE
#define JMP		0x1000
#define CALL	0x2000
#define SKEQ	0x3000
#define SKNE	0x4000
#define SKEV	0x5000
#define SETV	0x6000


int exec_instr(struct chip8_cpu *cpu, uint16 instr );


#endif /* __INSTRUCTIONS */
