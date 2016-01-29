/*******************************************************************************
 *	chip8.c
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

#include <stdlib.h>
#include <stdio.h>
#include "chip8.h"
#include "types.h"


const byte chip8_fontset[CHARSETS_SIZE] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


struct chip8_cpu *create_chip8_cpu() {
	int i;
	struct chip8_cpu *cpu;
	
	cpu = malloc( sizeof(struct chip8_cpu) );
	// reset cpu registers
	cpu->pc = PC_INIT_VALUE;
	cpu->I = 0;
	cpu->sp = 0;
	
	cpu->pc_already_updated = 0;
	cpu->graphic = 0;
	cpu->sound = 0;
	
	// reset timers
	cpu->delay_timer = 0;
	cpu->sound_timer = 0;
	
	cpu->log_file = fopen("exec_program.log", "w");
	if ( cpu->log_file == NULL) {
		printf("Could not create log file\n");
	}
	
	// clear memory and registers
	for ( i = 0 ; i < MEM_SIZE ; i++ ) cpu->memory[i] = 0;
	for ( i = 0 ; i < NUM_REGISTERS ; i++ ) cpu->V[i] = 0;
	for ( i = 0 ; i < SCREEN_SIZE ; i++ ) cpu->screen[i] = 0;
	for ( i = 0 ; i < STACK_SIZE ; i++ ) cpu->stack[i] = 0;	
	for ( i = 0 ; i < KEYPAD_SIZE ; i++) cpu->keypad[i] = 0;
	
	
	for ( i = 0 ; i < CHARSETS_SIZE ; i++ ) cpu->memory[i] = chip8_fontset[i];
	
	return cpu;
}


int chip8_emulate_cyle(struct chip8_cpu *cpu) {

	uint16 opcode;
	int ret_code;
	
	cpu->graphic = 0;
	cpu->pc_already_updated = 0;

	// fetch
	if (cpu->pc+1 >= MEM_SIZE)
		return -1;
		
	opcode = cpu->memory[cpu->pc] << 8  | cpu->memory[cpu->pc+1];	

	// execute
	ret_code = exec_instr(cpu, opcode);
	
	if (!cpu->pc_already_updated)
		cpu->pc += 2;

	// update timers
	if (cpu->delay_timer > 0)
		cpu->delay_timer--;
		
	if (cpu->sound_timer > 0)
		cpu->sound_timer--;
	else
		cpu->sound = 0;

	return ret_code;
}


void destroy_chip8_cpu(struct chip8_cpu *cpu) {
	fclose(cpu->log_file);
	free(cpu);
}


int chip8_load_ROM(struct chip8_cpu *cpu, const char *file) {
	FILE *rom_file;
	//byte c;
	int i = 0;
	byte * mem_ptr;
	
	mem_ptr = &cpu->memory[L_MEMORY];
	rom_file = fopen(file, "rb");
	if( rom_file == NULL) {
		printf("Could not open file\n");
		return -1;
	}
	while( fread(mem_ptr, 0xFFF-0x200, 1, rom_file) ) {
		mem_ptr++;
	}
	//i = 0x200;
	//while ( ((c = fgetc(rom_file)) != EOF) &&  i < MEM_SIZE) {
	//	cpu->memory[i] = c;
	//	i++;
	//}
	
	fclose(rom_file);
	 
	return 0;
}
