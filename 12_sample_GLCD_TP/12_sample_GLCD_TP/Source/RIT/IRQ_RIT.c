/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include <stdio.h>

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int down=0;

extern pacman_direction;

void RIT_IRQHandler (void)
{					
	/** static int up=0;
	static int position=0;	
	
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
		/* Joytick UP pressed 
		up++;
		switch(up){
			case 1:
				LED_Off(position);
				LED_On(0);
				position = 0;
				break;
			case 60:	//3sec = 3000ms/50ms = 60
				LED_Off(position);
				LED_On(7);
				position = 7;
				break;
			default:
				break;
		}
	}
	else{
			up=0;
	}
	
	/* button management 
	if(down>=1){ 
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	/* KEY1 pressed 
			switch(down){				
				case 2:				/* pay attention here: please see slides 19_ to understand value 2 
				if( position == 7){
					LED_On(0);
					LED_Off(7);
					position = 0;
				}
				else{
					LED_Off(position);
					LED_On(++position);
				}
					break;
				default:
					break;
			}
			down++;
		}
		else {	/* button released 
			down=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection 
		}
	}
/*	else{
			if(down==1)
				down++;
	} **/
	
	static int select = 0;
	static int up = 0;
	static int down = 0;
	static int left = 0;
	static int right = 0;
	
	if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){
		down++;
		switch(down){
			case 1:
				//Action
				pacman_direction = 2;
			break;
			default:
				break;
		}
	}
	else {
		down=0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){
		left++;
		switch(left){
			case 1:
				//Action
				pacman_direction = 1;
			break;
			default:
				break;
		}
	}
	else {
		left=0;
	}
		
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){
		up++;
		switch(up){
			case 1:
				//Action
				pacman_direction = 4;
			break;
			default:
				break;
		}
	}
	else {
		up=0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){
		right++;
		switch(right){
			case 1:
				//Action
				pacman_direction = 3;
			break;
			default:
				break;
		}
	}
	else {
		right=0;
	}
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
