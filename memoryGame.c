#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "stm32f10x_usart.h"
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
// own files from my library
#include "AsciiLib.h"
#include "GLCD_SG.h"
#include "AsciiLib.h"
#include "joystick.h"
#include "gpio.h"
#include "adc.h"
#include "systick.h"
#include "stm32f10x_systick.h"
#include "touchPanel.h"
#include "memoryGame.h"
#include "adc.h"

uint16_t sequence_Length = 3;
uint16_t good_Moves_In_A_Row = 0;

void initial_Screen(void)
{
	GUI_Text(120, 120, "MEMORY GAME", White, Black);
	GUI_Text(60, 150, "Follow the moves I drew.", Pink, Black);
	delay_ms(10000);
}

int set_Seed(void)
{
	LCD_Clear(Black);
	int value = 0;
	size_t i;
	GUI_Text(120, 120, "RANDOMISATION", Green, Black);
	GUI_Text(60, 150, "Use the potentiometer randomly", White, Black);
	for(i =0; i<500; i++)
	{
		value = value+adc_scan();
		delay_ms(10);
	}
	char score_Display[4];
	sprintf(score_Display,"%d",value);
	GUI_Text(120, 180, score_Display, Pink, Black);
	delay_ms(1000);
	LCD_Clear(Black);
	return value;
}

uint8_t *generate_Sequence(uint8_t *drawn_Sequence)
{
	drawn_Sequence = (uint8_t *)malloc(sequence_Length);
	size_t i;
	for (i = 0; i < sequence_Length; ++i)
	{
		drawn_Sequence[i] = rand() % 4;
	}
	return drawn_Sequence;
}

void display_Sequence(uint8_t *drawn_Sequence, uint16_t display_Time)
{
	LCD_Clear(Black);
	size_t i;
	for (i = 0; i < sequence_Length; ++i)
	{
		if (drawn_Sequence[i] == 0) GUI_Text(140, 120, "UP", Pink, Black);
		else if (drawn_Sequence[i] == 1) GUI_Text(140, 120, "DOWN", Pink, Black);
		else if (drawn_Sequence[i] == 2) GUI_Text(140, 120, "LEFT", Pink, Black);
		else if (drawn_Sequence[i] == 3) GUI_Text(140, 120, "RIGHT", Pink, Black);

		delay_ms(display_Time);
		LCD_Clear(Black);
	}
}

uint8_t *take_Movements(uint8_t *player_Sequence, uint16_t movement_Speed)
{
	draw_Joystick();
	player_Sequence = (uint8_t *)malloc(sequence_Length);
	size_t i;
	for (i = 0; i < sequence_Length; ++i)
	{
		player_Sequence[i] = 4;
	}

	i = 0;
	for (i = 0; i < sequence_Length; ++i)
	{
		Systick_on_ms(movement_Speed); //nms<=1864
		do
		{
			switch (joy_scan())
			{
				case 0x01:	//up
					LCD_FillArea(241,81,269,109,Pink);
					player_Sequence[i] = 0;
					break;
				case 0x02:	//down
					LCD_FillArea(241,141,269,169,Pink);
					player_Sequence[i] = 1;
					break;
				case 0x03:	//left
					LCD_FillArea(211,111,239,139,Pink);
					player_Sequence[i] = 2;
					break;
				case 0x06:	//right
					LCD_FillArea(271,111,299,139,Pink);
					player_Sequence[i] = 3;
					break;
				default: {}
			}
		} while (Systick_check_time_out()!= SET);
		LCD_Clear(Black);
		draw_Joystick();
		Systick_off();
	}
	return player_Sequence;
}

bool compare_Sequences(uint8_t *drawn_Sequence, uint8_t *player_Sequence)
{
	size_t i;
	for (i = 0; i < sequence_Length; ++i)
	{
		if (drawn_Sequence[i] != player_Sequence[i])
			return false;
		good_Moves_In_A_Row++;
	}
	return true;
}

void end_Game(uint16_t score)
{
	LCD_Clear(Black);
	char score_Display[4];
	sprintf(score_Display,"%d",score);
	char moves_Display[4];
	sprintf(moves_Display,"%d",good_Moves_In_A_Row);
	GUI_Text(120, 80, "WRONG MOVE!", Red, Black);
	GUI_Text(120, 100, "You lose.", White, Black);
	GUI_Text(120, 120, "Your score: ", White, Black);
	GUI_Text(120, 140, score_Display, Pink, Black);
	GUI_Text(120, 160, "Good moves in a row: ", White, Black);
	GUI_Text(120, 180, moves_Display, Red, Black);
	delay_ms(10000);
	good_Moves_In_A_Row = 0;
}

void display_Score(uint16_t score)
{
	LCD_Clear(Black);
	char score_Display[4];
	sprintf(score_Display,"%d",score);
	GUI_Text(120, 80, "GOOD MOVE!", Green, Black);
	GUI_Text(120, 100, "Next level.", White, Black);
	GUI_Text(120, 120, "Your score: ", White, Black);
	GUI_Text(120, 140, score_Display, Pink, Black);
	delay_ms(10000);
	good_Moves_In_A_Row = 0;
	sequence_Length++;
}

void draw_Joystick(void)
{
	GUI_Text(10, 110, "Repeat the sequence", White, Black);
	LCD_DrawLine(210, 80, 300, 80, Green);
	LCD_DrawLine(210, 110, 300, 110, Green);
	LCD_DrawLine(210, 140, 300, 140, Green);
	LCD_DrawLine(210, 170, 300, 170, Green);
	LCD_DrawLine(210, 80, 210, 170, Green);
	LCD_DrawLine(240, 80, 240, 170, Green);
	LCD_DrawLine(270, 80, 270, 170, Green);
	LCD_DrawLine(300, 80, 300, 170, Green);
}
