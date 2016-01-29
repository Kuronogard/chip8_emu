/*******************************************************************************
 *	instructions.c
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

//#include <stdlib.h>
//#include <stdio.h>

//#define DEBUG
//#define DEBUG_TRACE

#include "debug_def.h"
#include "types.h"
#include "instructions.h"
#include "chip8.h"

#define XPARAM(instr)	((instr & 0x0F00) >> 8)
#define YPARAM(instr)	((instr & 0x00F0) >> 4)
#define LCONST(instr)	(instr & 0x0FFF)
#define MCONST(instr)	(instr & 0x00FF)
#define SCONST(instr)	(instr & 0x000F)

#define CARRY_BYTE(a, b)	(a > (0xff - b))
#define BORRO_BYTE(a, b)	(a < b)
#define XOR_CLEAR(old, a)	(((old ^ a) & old) != old)
#define PIXEL(value, bit)	((value & (0x1 << (bit))) >> (bit))
#define SCREEN_COLISION(cpu, x, y, value, pixel_off)	cpu->V[CARRY_VREG] = (cpu->screen[(y)*SCREEN_WIDTH + (x)] & PIXEL((value), pixel_off))? 1 : cpu->V[CARRY_VREG]
#define SCREEN_UPDATE(cpu, x, y, value, pixel_off)	cpu->screen[(y)*SCREEN_WIDTH + (x)] ^= PIXEL((value), pixel_off)	


int instr_rca(struct chip8_cpu *cpu, uint16 addr) {
	fprintf(stderr, "Not implemented\n");
	return -1;
}

int instr_scrolld(struct chip8_cpu *cpu, byte n) {
	DEBUG_TRACE_PRINT();
	fprintf(stderr, "Not implemented\n");
	return -1;
}

int instr_scrollr(struct chip8_cpu *cpu) {
	DEBUG_TRACE_PRINT();
	fprintf(stderr, "Not implemented\n");
	return -1;
}

int instr_scrolll(struct chip8_cpu *cpu) {
	DEBUG_TRACE_PRINT();
	fprintf(stderr, "Not implemented\n");
	return -1;
}

int instr_quit(struct chip8_cpu *cpu) {
	DEBUG_TRACE_PRINT();
	fprintf(stderr, "Not implemented\n");
	return -1;
}

int instr_chip8_gf_mode(struct chip8_cpu *cpu) {
	DEBUG_TRACE_PRINT();
	fprintf(stderr, "Not implemented\n");
	return -1;
}

int instr_schip_gf_mode(struct chip8_cpu *cpu) {
	DEBUG_TRACE_PRINT();
	fprintf(stderr, "Not implemented\n");
	return -1;
}

int instr_clr(struct chip8_cpu *cpu) {
	DEBUG_TRACE_PRINT();
	int i;
	for (i = 0 ; i < SCREEN_SIZE ; i++) {
		cpu->screen[i] = 0;
	}
	cpu->graphic = 1;
	return 0;
}

int instr_bl(struct chip8_cpu *cpu) {
	DEBUG_TRACE_PRINT();
	if ((cpu->sp < 1) || (cpu->sp > STACK_SIZE))
		return -1;
	cpu->sp--;
	cpu->pc = cpu->stack[cpu->sp];
	cpu->pc_already_updated = 1;
	return 0;
}

int instr_jmp(struct chip8_cpu *cpu, uint16 addr) {
	DEBUG_TRACE_PRINT();
	if (addr >= MEM_SIZE)
		return -1;
	cpu->pc = addr;
	cpu->pc_already_updated = 1;
	return 0;
}

int instr_call(struct chip8_cpu *cpu, uint16 addr) {
	DEBUG_TRACE_PRINT();
	if (cpu->sp >= STACK_SIZE)
		return -1;
	cpu->stack[cpu->sp] = cpu->pc+2;
	cpu->sp++;
	cpu->pc = addr;
	cpu->pc_already_updated = 1;
	return 0;
}

int instr_skipeq(struct chip8_cpu *cpu, byte Vx, byte cons) {
	DEBUG_TRACE_PRINT();
	if (Vx >= NUM_REGISTERS)
		return -1;
	if (cpu->V[Vx] == cons)
		cpu->pc += 2;
	return 0;
}

int instr_skipne(struct chip8_cpu *cpu, byte Vx, byte cons) {
	DEBUG_TRACE_PRINT();
	if (Vx >= NUM_REGISTERS)
		return -1;
	if (cpu->V[Vx] != cons)
		cpu->pc += 2;
	return 0;
}

int instr_skipveq(struct chip8_cpu *cpu, byte Vx, byte Vy) {
	DEBUG_TRACE_PRINT();
	if ((Vx >= NUM_REGISTERS) || (Vy >= NUM_REGISTERS))
		return -1;
	if (cpu->V[Vx] == cpu->V[Vy])
		cpu->pc += 2;
	return 0;
}

int instr_setvc(struct chip8_cpu *cpu, byte Vx, byte cons) {
	DEBUG_TRACE_PRINT();
	if (Vx >= NUM_REGISTERS)
		return -1;
	cpu->V[Vx] = cons;
	return 0;
}

int instr_addvc(struct chip8_cpu *cpu, byte Vx, byte cons) {
	DEBUG_TRACE_PRINT();
	if (Vx >= NUM_REGISTERS)
		return -1;
	cpu->V[Vx] = (cpu->V[Vx] + cons) & 0xFF;
	return 0;
}

int instr_setv(struct chip8_cpu *cpu, byte Vx, byte Vy) {
	DEBUG_TRACE_PRINT();
	if ((Vx >= NUM_REGISTERS) || (Vy >= NUM_REGISTERS))
		return -1;
	cpu->V[Vx] = cpu->V[Vy];
	return 0;
}

int instr_or(struct chip8_cpu *cpu, byte Vx, byte Vy) {
	DEBUG_TRACE_PRINT();
	if ((Vx >= NUM_REGISTERS) || (Vy >= NUM_REGISTERS))
		return -1;
	cpu->V[Vx] |= cpu->V[Vy];
	return 0;
}

int instr_and(struct chip8_cpu *cpu, byte Vx, byte Vy) {
	DEBUG_TRACE_PRINT();
	if ((Vx >= NUM_REGISTERS) || (Vy >= NUM_REGISTERS))
		return -1;
	cpu->V[Vx] &= cpu->V[Vy];
	return 0;
}

int instr_xor(struct chip8_cpu *cpu, byte Vx, byte Vy) {
	DEBUG_TRACE_PRINT();
	if ((Vx >= NUM_REGISTERS) || (Vy >= NUM_REGISTERS))
		return -1;
	cpu->V[Vx] ^= cpu->V[Vy];
	return 0;
}

int instr_add(struct chip8_cpu *cpu, byte Vx, byte Vy) {
	DEBUG_TRACE_PRINT();
	if ((Vx >= NUM_REGISTERS) || (Vy >= NUM_REGISTERS))
		return -1;
	if (cpu->V[Vx] > (0xFF - cpu->V[Vy]))
		cpu->V[CARRY_VREG] = 1;
	cpu->V[Vx] = (cpu->V[Vx] + cpu->V[Vy]) & 0xFF;
	return 0;	
}

int instr_suby(struct chip8_cpu *cpu, byte Vx, byte Vy) {
	DEBUG_TRACE_PRINT();
	if ((Vx >= NUM_REGISTERS) || (Vy >= NUM_REGISTERS))
		return -1;
	if (cpu->V[Vx] < cpu->V[Vy])
		cpu->V[CARRY_VREG] = 0;
	cpu->V[Vx] = (cpu->V[Vx] - cpu->V[Vy]) & 0xFF;
	return 0;
}

int instr_shiftr(struct chip8_cpu *cpu, byte Vx) {
	DEBUG_TRACE_PRINT();
	if (Vx >= NUM_REGISTERS)
		return -1;
	cpu->V[CARRY_VREG] = cpu->V[Vx] & 0x1;
	cpu->V[Vx] = cpu->V[Vx] > 1;
	return 0;
}

int instr_subx(struct chip8_cpu *cpu, byte Vx, byte Vy) {
	DEBUG_TRACE_PRINT();
	if ((Vx >= NUM_REGISTERS) || (Vy >= NUM_REGISTERS))
		return -1;
	if (cpu->V[Vy] < cpu->V[Vx])
		cpu->V[CARRY_VREG] = 0;
	cpu->V[Vx] = (cpu->V[Vy] - cpu->V[Vx]) & 0xFF;
	return 0;	
}

int instr_shiftl(struct chip8_cpu *cpu, byte Vx) {
	DEBUG_TRACE_PRINT();
	if (Vx >= NUM_REGISTERS)
		return -1;
	cpu->V[CARRY_VREG] = (cpu->V[Vx] & 0x8000) >> 15;
	cpu->V[Vx] = cpu->V[Vx] < 1;
	return 0;	
}

int instr_skipvne(struct chip8_cpu *cpu, byte Vx, byte Vy) {
	DEBUG_TRACE_PRINT();
	if ((Vx >= NUM_REGISTERS) || (Vy >= NUM_REGISTERS))
		return -1;
	if (cpu->V[Vx] != cpu->V[Vy])
		cpu->pc += 2;
	return 0;
}

int instr_seti(struct chip8_cpu *cpu, uint16 cons) {
	DEBUG_TRACE_PRINT();
	cpu->I = cons;
	return 0;
}

int instr_jmpa(struct chip8_cpu *cpu, uint16 cons) {
	DEBUG_TRACE_PRINT();
	cpu->pc = cpu->V[0] + cons;
	return 0;
}

int instr_rand(struct chip8_cpu *cpu, byte Vx, byte cons) {
	DEBUG_TRACE_PRINT();
	if (Vx >= NUM_REGISTERS)
		return -1;
	cpu->V[Vx] = cons & rand();
	return 0;
}

int instr_draw(struct chip8_cpu *cpu, byte Vx, byte Vy, byte height) {
	int i, j;
	uint16 screenX, screenY;
	byte value;
	DEBUG_TRACE_PRINT();
	
	if ((Vx >= NUM_REGISTERS) || (Vy >= NUM_REGISTERS))
		return -1;
	
	if ((cpu->I+height-1) >= MEM_SIZE)
		return -1;
	
	cpu->V[CARRY_VREG] = 0; 
	for( i = 0 ; i < height ; i++ ) {
		screenY = cpu->V[Vy] + i;
		value = cpu->memory[cpu->I + i];
		for ( j = 0 ; j < 8 ; j++) {
			screenX = cpu->V[Vx] + j;
			if ((screenY*SCREEN_WIDTH + screenX) >= MEM_SIZE)
				return -1;
			if ((screenY*SCREEN_WIDTH + screenX) < 0)
				return -1;
			SCREEN_COLISION(cpu, screenX, screenY, value, 7-j);
			SCREEN_UPDATE(cpu, screenX, screenY, value, 7-j);
		}
	}

	cpu->graphic = 1;
	return 0;	
}

int instr_skipkey(struct chip8_cpu *cpu, byte Vx) {
	DEBUG_TRACE_PRINT();
	if ((Vx >= NUM_REGISTERS) || (cpu->V[Vx] >= KEYPAD_SIZE))
		return -1;
	if (cpu->keypad[cpu->V[Vx]])
		cpu->pc += 2;
	return 0;
}

int instr_skipnkey(struct chip8_cpu *cpu, byte Vx) {
	DEBUG_TRACE_PRINT();
	if ((Vx >= NUM_REGISTERS) || (cpu->V[Vx] >= KEYPAD_SIZE))
		return -1;
	if (!cpu->keypad[cpu->V[Vx]])
		cpu->pc += 2;
	return 0;	
}

int instr_getdelay(struct chip8_cpu *cpu, byte Vx) {
	DEBUG_TRACE_PRINT();
	if (Vx >= NUM_REGISTERS)
		return -1;
	cpu->V[Vx] = cpu->delay_timer;
	return 0;
}

int instr_waitkey(struct chip8_cpu *cpu, byte Vx) {
	DEBUG_TRACE_PRINT();
	int i = 0;
	if (Vx >= NUM_REGISTERS)
		return -1;
	while((cpu->keypad[i] == 0) && (i < KEYPAD_SIZE) )
		i++;
	if (i < KEYPAD_SIZE){
		cpu->V[Vx] = i;
	}
	else {
		cpu->pc_already_updated = 1; // so it does not update the program counter
	}
	return 0;
}

int instr_setdelay(struct chip8_cpu *cpu, byte Vx) {
	DEBUG_TRACE_PRINT();
	if (Vx >= NUM_REGISTERS)
		return -1;
	cpu->delay_timer = cpu->V[Vx];
	return 0;
}

int instr_setsound(struct chip8_cpu *cpu, byte Vx) {
	DEBUG_TRACE_PRINT();
	if (Vx >= NUM_REGISTERS)
		return -1;		
	cpu->sound_timer = cpu->V[Vx];
	cpu->sound = 1;
	return 0;
}

int instr_addi(struct chip8_cpu *cpu, byte Vx) {
	DEBUG_TRACE_PRINT();
	if (Vx >= NUM_REGISTERS)
		return -1;
	cpu->I = cpu->I + cpu->V[Vx];
	return 0;
}

int instr_setich(struct chip8_cpu *cpu, byte Vx) {
	DEBUG_TRACE_PRINT();
	if (Vx >= NUM_REGISTERS)
		return -1;
	cpu->I = cpu->V[Vx] * 5;
	return 0;
}

int instr_bcd(struct chip8_cpu *cpu, byte Vx) {
	DEBUG_TRACE_PRINT();
	if ((Vx >= NUM_REGISTERS) || ((cpu->I+2) >= MEM_SIZE))
		return -1;
	cpu->memory[cpu->I] = cpu->V[Vx]/100;
	cpu->memory[cpu->I+1] = (cpu->V[Vx]/10)%10;
	cpu->memory[cpu->I+2] = cpu->V[Vx]%10;
	return 0;
}

int instr_st(struct chip8_cpu *cpu, byte Vx) {
	DEBUG_TRACE_PRINT();
	int i;
	if ((Vx >= NUM_REGISTERS) || ((cpu->I + Vx) >= MEM_SIZE))
		return -1;
	for (i = 0 ; i <= Vx ; i++)
		cpu->memory[cpu->I + i] = cpu->V[i];
	return 0;
}

int instr_ld(struct chip8_cpu *cpu, byte Vx) {
	DEBUG_TRACE_PRINT();
	int i;
	if ((Vx >= NUM_REGISTERS) || ((cpu->I + Vx) >= MEM_SIZE))
		return -1;
	for (i = 0 ; i <= Vx ; i++)
		cpu->V[i] = cpu->memory[cpu->I + i];
	return 0;
}

int instr_st_hpflags(struct chip8_cpu *cpu, byte Vx) {
	DEBUG_TRACE_PRINT();
	fprintf(stderr, "Not implemented\n");
	return -1;
}

int instr_ld_hpflags(struct chip8_cpu *cpu, byte Vx) {
	DEBUG_TRACE_PRINT();
	fprintf(stderr, "Not implemented\n");
	return -1;
}



int exec_instr(struct chip8_cpu *cpu, uint16 instr ) {
	int i, j;
	fprintf(stderr, "executing %04x\n", instr);
	switch(instr & 0xF000) {
		case 0x0000:
			switch(instr) {
				case 0x00E0: // clears the screen
					return instr_clr(cpu);
				case 0x00EE: // returns from a subrutine
					return instr_bl(cpu);
				case 0x00FB:
					return instr_scrollr(cpu);
				case 0x00FC:
					return instr_scrolll(cpu);
				case 0x00FD:
					return instr_quit(cpu);
				case 0x00FE:
					return instr_chip8_gf_mode(cpu);
				case 0x00FF:
					return instr_schip_gf_mode(cpu);
				default:  // calls RCA1802 program at  (0x0FFF)
					switch(instr & 0x00F0) {
						case 0x00C0:
							return instr_scrolld(cpu, YPARAM(instr));
						default:
							return instr_rca(cpu, LCONST(instr));
					};
			};
		case 0x1000: // jumps to address (0xFFF)
			return instr_jmp(cpu, LCONST(instr));
		case 0x2000: // calls a subrutine (0xFFF)
			return instr_call(cpu, LCONST(instr));
		case 0x3000: // skips the next instr if V(0x0F00) equals (0x00FF)
			return instr_skipeq(cpu, XPARAM(instr), MCONST(instr));
		case 0x4000: // skips the next instr if V(0x0F00) does not equals (0x00FF)
			return instr_skipne(cpu, XPARAM(instr), MCONST(instr));
		case 0x5000: // skips the next instr if V(0x0F00) equals V(0x00F0)
			return instr_skipveq(cpu, XPARAM(instr), YPARAM(instr));
		case 0x6000: // sets V(0x0F00) to (0x00FF)
			return instr_setvc(cpu, XPARAM(instr), MCONST(instr));
		case 0x7000: // adds (0x00FF) to V(0x0F00)
			return instr_addvc(cpu, XPARAM(instr), MCONST(instr));
		case 0x8000:
			switch(instr & 0x000F) {
				case 0x0000: // sets V(0x0F00) to V(0x00F0)
					return instr_setv(cpu, XPARAM(instr), YPARAM(instr));
				case 0x0001: // sets V(0x0F00) to V(0x0F00) or V(0x00F0)
					return instr_or(cpu, XPARAM(instr), YPARAM(instr));			
				case 0x0002: // sets V(0x0F00) to V(0x0F00) and V(0x00F0)
					return instr_and(cpu, XPARAM(instr), YPARAM(instr));	
				case 0x0003: // sets V(0x0F00) to V(0x0F00) xor V(0x00F0)
					return instr_xor(cpu, XPARAM(instr), YPARAM(instr));	
				case 0x0004: // adds V(0x00F0) to V(0x0F00) and sets VF to 1 if carry or 0 if not
					return instr_add(cpu, XPARAM(instr), YPARAM(instr));
				case 0x0005: // subs V(0x00F0) to V(0x0F00) and sets VF to 0 if borrow or 1 if not
					return instr_suby(cpu, XPARAM(instr), YPARAM(instr));	
				case 0x0006: // shifts V(0x0F00) right by 1, VF is set to V(0x0F00) least significant bit before shift
					return instr_shiftr(cpu, XPARAM(instr));	
				case 0x0007: // sets V(0x0F00) to V(0x00F0) minus V(0x0F00), sets VF to 0 if borrow or 1 if not
					return instr_subx(cpu, XPARAM(instr), YPARAM(instr));
				case 0x000E: // shifts V(0x0F00) left by 1, VF is set to V(0x0F00) most significant bit before shift
					return instr_shiftl(cpu, XPARAM(instr));
				default:
					printf("Error: Ilegal instruction\n");
					return -1;
			};
		case 0x9000: // skips the next instr if V(0x0F00) does not equal V(0x00F0)
			return instr_skipvne(cpu, XPARAM(instr), YPARAM(instr));
		case 0xA000: // sets I to (0x0FFF)
			return instr_seti(cpu, LCONST(instr));
		case 0xB000: // jumps to (0x0FFF plus V0)
			return instr_jmpa(cpu, LCONST(instr));
		case 0xC000: // sets V(0x0F00) to the result of a bitwise AND between a random number and (0x00FF)
			return instr_rand(cpu, XPARAM(instr), MCONST(instr));
		case 0xD000:
			// draws sprite stored in memory location I, in position V(0x0F00), V(0x00F0). (0x000F)
			// is the number of 8bit rows that need to be drawn. if N is greater than 1, second line continues
			// at position V(0x0F00), V(0x00F0)+1, and so on. if qhen drawn, clears a pixe, VF is set to 1, othrewise is zero.
			return instr_draw(cpu, XPARAM(instr), YPARAM(instr), SCONST(instr));
		case 0xE000:
			switch(instr & 0x00FF) {
				case 0x009E: // skip the next instruction if the key in V(0x0F00) is pressed
					return instr_skipkey(cpu, XPARAM(instr));
				case 0x00A1: // skip the next instruction if the key in V(0xF00) is not pressed
					return instr_skipnkey(cpu, XPARAM(instr));
				default:
					printf("Error: Ilegal instruction\n");
					return -1;
			};
		case 0xF000:
			switch(instr & 0x00FF) {
				case 0x0007: // sets V(0x0F00) to the value of the delay timer
					return instr_getdelay(cpu, XPARAM(instr));
				case 0x000A: // a key press is awaited, and the stored in V(0x0F00)
					return instr_waitkey(cpu, XPARAM(instr));			
				case 0x0015: // sets the delay timer to V(0x0F00)
					return instr_setdelay(cpu, XPARAM(instr));
				case 0x0018: // sets the sound timer to V(0x0F00)
					return instr_setsound(cpu, XPARAM(instr));
				case 0x001E: // adds V(0x0F00) to I
					return instr_addi(cpu, XPARAM(instr));
				case 0x0029: // sets I to the location of the sprite for the char in V(0x0F00)
					return instr_setich(cpu, XPARAM(instr));
				case 0x0033:
					// stores the BCD representation of V(0x0F00), with the most significant
					// of three digits at the address in I, the middle digit at I plus 1
					// and the least significant digit at I plus 2.
					return instr_bcd(cpu, XPARAM(instr));
				case 0x0055: // stores V0 to V(0x0F00) in memory starting at address I
					return instr_st(cpu, XPARAM(instr));
				case 0x0065: // fills V0 to V(0x0F00) with values from memory starting at address I
					return instr_ld(cpu, XPARAM(instr));
				case 0x0075:
					return instr_st_hpflags(cpu, XPARAM(instr));
				case 0x0085:
					return instr_ld_hpflags(cpu, XPARAM(instr));
				
				default:
					printf("Error: Ilegal instruction\n");
					return -1;
			};
		default:
			printf("Error: Ilegal instruction\n");
			return -1;
	};
	
}
