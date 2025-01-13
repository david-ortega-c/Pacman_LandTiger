/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include <time.h>
#include <stdio.h>

#define MAZE_WIDTH  30   // Each cell corresponds to an 8x8 pixel block (224/8 = 28)
#define MAZE_HEIGHT 31   

// Define tile types
#define EMPTY        0
#define WALL         1
#define FOOD         2
#define POWER_PELLET 3
#define Y_OFFSET_MAZE 16

uint8_t time_left = 60;
uint8_t score = 0;
uint8_t pacman_position[2];

uint8_t maze[MAZE_HEIGHT][MAZE_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 2, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 2, 1, 1, 2, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 2, 1},
		{1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1},
		{1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
		{1, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 1},
		{1, 0, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 0, 1},
		{1, 2, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 2, 1},
		{1, 2, 2, 0, 2, 0, 2, 0, 2, 2, 2, 2, 2, 0, 2, 2, 0, 2, 2, 2, 2, 2, 0, 2, 0, 2, 0, 2, 2, 1},
		{1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1},
		{1, 0, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 0, 1},
		{1, 2, 1, 1, 1, 2, 2, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 2, 2, 2, 1, 1, 1, 2, 1},
		{1, 0, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 2, 0, 2, 2, 0, 2, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0, 1},
		{1, 2, 1, 1, 1, 2, 0, 2, 0, 2, 0, 2, 1, 1, 1, 1, 1, 1, 2, 0, 2, 0, 2, 0, 2, 1, 1, 1, 2, 1},
		{1, 2, 2, 0, 2, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 2, 0, 2, 2, 1},
		{1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}, //Middle Row
		{1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1},
		{1, 2, 2, 0, 2, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 2, 0, 2, 2, 1},
		{1, 2, 1, 1, 1, 2, 0, 2, 0, 2, 0, 2, 1, 1, 1, 1, 1, 1, 2, 0, 2, 0, 2, 0, 2, 1, 1, 1, 2, 1},
		{1, 0, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 2, 2, 0, 0, 2, 2, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0, 1},
		{1, 2, 1, 1, 1, 2, 2, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 2, 2, 2, 1, 1, 1, 2, 1},
		{1, 0, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 0, 1},
		{1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1},
		{1, 2, 2, 0, 2, 0, 2, 0, 2, 2, 2, 2, 2, 0, 2, 2, 0, 2, 2, 2, 2, 2, 0, 2, 0, 2, 0, 2, 2, 1},
		{1, 2, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 2, 1},
		{1, 0, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 0, 1},
		{1, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 1},
		{1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
		{1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1},
		{1, 2, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 2, 1, 1, 2, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 2, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};


void writeHeader(){
	
	char str[100];
	sprintf(str ,  " Score : %d  Timer : %d s  ", score, time_left);
	GUI_Text(0,0, (uint8_t *) str, White, Black);
}

void drawMaze() {
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            if (maze[y][x] == WALL) {
                // Draw a block of 8x8 pixels for the wall
                for (int py = 0; py < 8; py++) {
                    for (int px = 0; px < 8; px++) {
                        LCD_SetPoint(x * 8 + px, y * 8 + py + Y_OFFSET_MAZE, Blue);
                    }
                }
            }
						// Draw a 2x2 block for food 
						else if (maze[y][x] == FOOD) {
							for (int py = 0; py < 2; py++) {
                    for (int px = 0; px < 2; px++) {
                        LCD_SetPoint(x * 8 + px + 3, y * 8 + py + Y_OFFSET_MAZE+3, White);
                    }
                }
						}
        }
    }
}

//pacman direction
// 1: left
// 2: down
// 3: right
// 4: up
uint8_t pacman_direction;


//Pacman is rendered in the Matrix where it the coordinate is a position in the maze matrix
void renderPacman(uint16_t Xpos,uint16_t Ypos){
	if (maze[Ypos][Xpos] == EMPTY){
		for (int py = 0; py < 6; py++) {
				for (int px = 0; px < 6; px++) {
						LCD_SetPoint(Xpos * 8 + px + 1 , Ypos * 8 + py + Y_OFFSET_MAZE + 1, Yellow);
				}
		}
	}
}



static uint8_t power_positions[6][2];

void renderPower(uint16_t Xpos,uint16_t Ypos){
	for (int py = 0; py < 4; py++) {
		for (int px = 0; px < 4; px++) {
				LCD_SetPoint(Xpos * 8 + px + 2 , Ypos * 8 + py + Y_OFFSET_MAZE + 2, White);
		}
	}

}

void placePower(){
	
	
	int min_x = 0 , maxx = MAZE_WIDTH;
	int min_y = 0 , maxy = MAZE_HEIGHT;
	//In this case we can use only maze width and height 
	int powers_placed = 0;
	// Seed the random number generator with the current time
  enable_timer(0);
	
	while (powers_placed < 6){
		unsigned int seed = LPC_TIM0->TC;  // Timer Counter value
		srand(seed);
		int random_number_y = min_y + rand() % (maxy - min_y + 1);
		int random_number_x = min_x + rand() % (maxx - min_x + 1);
		if (maze[random_number_y][random_number_x]==FOOD){
			power_positions[powers_placed][0] = random_number_y;
			power_positions[powers_placed][1] = random_number_x;
			renderPower(random_number_x, random_number_y);
			powers_placed++;
			//GUI_Text(0,0, (uint8_t *) " Score : 00  Timer : 60 s  ", White, Black);
		}
	}
	disable_timer(0);
		
}

void initializePacman(){
	pacman_position[0] = 15; // This is the X coordinate
	pacman_position[1] = 19; // This is the Y coordinate
	renderPacman(pacman_position[0],pacman_position[1]);
	placePower();
	pacman_direction = 1;
	//printf("%d ", maze[20][15]);
}

void startCountdown(){
	init_timer(0, 0x17D7840);
	enable_timer(0);
}

void erasePacman(){
		for (int py = 0; py < 8; py++) {
				for (int px = 0; px < 8; px++) {
						LCD_SetPoint(pacman_position[0] * 8 + px , pacman_position[1] * 8 + py + Y_OFFSET_MAZE + 1, Black);
				}
		}
}

void movePacman(){
	//erasePacman(pacman_position[0], pacman_position[1]);
	switch (pacman_direction){
		case 1:
			if (maze[pacman_position[1]][pacman_position[0]-1] != WALL){
				erasePacman();
				pacman_position[0]--;
				renderPacman(pacman_position[0],pacman_position[1]);
			}
		case 2:
			if (maze[pacman_position[1]-1][pacman_position[0]] != WALL){
				erasePacman();
				pacman_position[1]--;
				renderPacman(pacman_position[0],pacman_position[1]);
			}
		case 3:
			if (maze[pacman_position[1]][pacman_position[0]+1] != WALL){
				erasePacman();
				pacman_position[0]++;
				renderPacman(pacman_position[0],pacman_position[1]);
			}
		case 4:
			if (maze[pacman_position[1]+1][pacman_position[0]] != WALL){
				erasePacman();
				pacman_position[1]++;
				renderPacman(pacman_position[0],pacman_position[1]);
			}
		default:
			break;
	}
		
}

void startGame(){
	init_timer(1,0x393870);
	enable_timer(1);

}





