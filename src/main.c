/*******************************************************************************
 *	types.h
 *
 *  typede finitions
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

#include "chip8.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdlib.h>
#include <signal.h>


int close_client = 0;

void draw_graphics(SDL_Texture *texture, byte *gfx_memory, int len) {
	Uint32 p_screen[SCREEN_SIZE];
	int i;
	
	for(i = 0; i < SCREEN_SIZE ; i++) {
		p_screen[i] = ((Uint32)gfx_memory[i])? 0xFFFFFFFF: 0x00000000;
	}
	
	SDL_UpdateTexture(texture, NULL, p_screen, SCREEN_WIDTH*sizeof(Uint32));
}


void check_keyboard_events(struct chip8_cpu *cpu) {
	SDL_Event event;

	while(SDL_PollEvent(&event)) {

		if( (event.type != SDL_KEYDOWN) && (event.type != SDL_KEYUP) ) {
			continue;
		}
		switch(event.key.keysym.sym) {
			case SDLK_1:
				cpu->keypad[1] = (event.key.state == SDL_PRESSED)? 0xff: 0x00;
				break;
			case SDLK_2:
				cpu->keypad[2] = (event.key.state == SDL_PRESSED)? 0xff: 0x00;
				break;
			case SDLK_3:
				cpu->keypad[3] = (event.key.state == SDL_PRESSED)? 0xff: 0x00;
				break;
			case SDLK_4:
				cpu->keypad[12] = (event.key.state == SDL_PRESSED)? 0xff: 0x00;
				break;
			case SDLK_q:
				cpu->keypad[4] = (event.key.state == SDL_PRESSED)? 0xff: 0x00;
				break;
			case SDLK_w:
				cpu->keypad[5] = (event.key.state == SDL_PRESSED)? 0xff: 0x00;
				break;
			case SDLK_e:
				cpu->keypad[6] = (event.key.state == SDL_PRESSED)? 0xff: 0x00;
				break;
			case SDLK_r:
				cpu->keypad[13] = (event.key.state == SDL_PRESSED)? 0xff: 0x00;
				break;
			case SDLK_a:
				cpu->keypad[7] = (event.key.state == SDL_PRESSED)? 0xff: 0x00;
				break;
			case SDLK_s:
				cpu->keypad[8] = (event.key.state == SDL_PRESSED)? 0xff: 0x00;
				break;
			case SDLK_d:
				cpu->keypad[9] = (event.key.state == SDL_PRESSED)? 0xff: 0x00;
				break;
			case SDLK_f:
				cpu->keypad[14] = (event.key.state == SDL_PRESSED)? 0xff: 0x00;
				break;
			case SDLK_z:
				cpu->keypad[10] = (event.key.state == SDL_PRESSED)? 0xff: 0x00;
				break;
			case SDLK_x:
				cpu->keypad[0] = (event.key.state == SDL_PRESSED)? 0xff: 0x00;
				break;
			case SDLK_c:
				cpu->keypad[11] = (event.key.state == SDL_PRESSED)? 0xff: 0x00;
				break;
			case SDLK_v:
				cpu->keypad[15] = (event.key.state == SDL_PRESSED)? 0xff: 0x00;
				break;
		}
	}
}

void play_sound() {
	printf("BEEP\n");
	
}


void print_cpu_info(struct chip8_cpu *cpu) {
	int i, j;
	
/*	for( i = 0 ; i < MEM_SIZE/4 ; i++) {
		for( j = 0 ; j < 
		printf("M[0x%04x]: %02x\tM[0x%04x]: %02x\tM[0x%04x]: %02x\tM[0x%04x]: %02x", 
				i, cpu->memory[i], i+1, cpu->memory[i+1], i+3, cpu->memory[i+3], i+4, cpu->memory[i+4]);  
	}
*/
	for( i = 0 ; i < NUM_REGISTERS/8 ; i++ ) {
		for( j = 0 ; j < 8 ; j++){
			printf("V[%02d]:%02x  ", (i*8+j), cpu->V[i*8 + j]);  
		}
		printf("\n");
	}

	for( i = 0 ; i < STACK_SIZE/8 ; i++ ) {
		for( j = 0 ; j < 8 ; j++){
			printf("ST[%02d]:%02x  ", (i*8+j), cpu->stack[i*8 + j]);  
		}
		printf("\n");
	}

	for( i = 0 ; i < KEYPAD_SIZE/8 ; i++ ) {
		for( j = 0 ; j < 8 ; j++){
			printf("KP[%02d]:%02x  ", (i*8+j), cpu->keypad[i*8 + j]);  
		}
		printf("\n");
	}
	
	printf("\nSP:%02d  PC:%02d  I:%02d  sound:%d  graphic:%d  delay_t:%d  sound_t:%d\n",
		cpu->sp, cpu->pc, cpu->I, cpu->sound, cpu->graphic, cpu->delay_timer, cpu->sound_timer);
	
	printf("\n");
	for( i = 0 ; i < SCREEN_HEIGHT ; i++) {
		for ( j = 0 ; j < SCREEN_WIDTH ; j++) {
			printf("%c", (cpu->screen[i*SCREEN_WIDTH + j])? '8': '-');
		}
		printf("\n");
	}
}

void stop_client(int signal) {
	close_client = 1;
}

int main(int argc, char **argv) {
	
	struct chip8_cpu *cpu;
	SDL_Window *window;
	SDL_Texture *texture;
	SDL_Renderer *renderer;
	int ret_value;
	
	if (argc < 2) {
		printf("You have to specify a rom \nusage: %s <rom_path>\n", argv[0]);
		return 0;
	}
	
	// Configure signal handling
	if (signal(SIGINT, stop_client) == SIG_ERR) {
		printf("Could not attach SIGINT handler");
		return -1;
	}
	if (signal(SIGTERM, stop_client) == SIG_ERR) {
		printf("Could not attach SIGTERM handler");
		return -1;
	}
	if (signal(SIGABRT, stop_client) == SIG_ERR) {
		printf("Could not attach SIGABRT handler");
		return -1;
	}

	SDL_SetMainReady();
	
	// init cpu
	cpu = create_chip8_cpu();
	chip8_load_ROM(cpu, argv[1]);
	
	/*
	int i, j;
	printf("Loaded program\n");
	for( i = 200 ; i < 1000 ; i++) {
			printf("M[0x%04x]: %02x \n", 
				i, cpu->memory[i]);  
	}
	getchar();
	*/
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		fprintf(stderr, "Could not init SDL: %s\n", SDL_GetError());
		destroy_chip8_cpu(cpu);
		return -1;
	}

	SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_OPENGL, &window, &renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	while(!close_client) {

		//write(1,"\E[H\E[2J",7);
		ret_value = chip8_emulate_cyle(cpu);
		if (ret_value != 0) {
			fprintf(stderr,"Error executing the last instruction\n");
			close_client = 1;
			continue;
		}
		
		if (cpu->graphic) {
			draw_graphics(texture, cpu->screen, SCREEN_SIZE);
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);
		}
		
		if (cpu->sound)
			play_sound();

		check_keyboard_events(cpu);

		//print_cpu_info(cpu);
		//getchar();
		usleep(1000);
		// wait for next cycle
	}
	
	destroy_chip8_cpu(cpu);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}



