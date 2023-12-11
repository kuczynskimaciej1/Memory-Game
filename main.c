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

void USART1_Configuration(void);
void USART2_Configuration(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void NVIC_Configuration(void);
void USART_SendText(USART_TypeDef *USARTx, char *str);

int main(void)
{
	delay_init();
	adc_init();
	joy_init();
	LCD_Initialization();

	srand(set_Seed());
	uint16_t score = 0;
	uint16_t movement_Speed = 10000;
	uint16_t display_Time = 5000;
	uint8_t *drawn_Sequence;
	uint8_t *player_Sequence;

	LCD_Clear(Black);
	initial_Screen();
	delay_ms(10000);
	LCD_Clear(Black);
	while (1)
	{
		drawn_Sequence = generate_Sequence(drawn_Sequence);
		display_Sequence(drawn_Sequence, display_Time);
		player_Sequence = take_Movements(player_Sequence, movement_Speed);
		bool comparison = compare_Sequences(drawn_Sequence, player_Sequence);
		if (comparison == false)
		{
			end_Game(score);
			break;
		}
		else
		{
			score++;
			movement_Speed = movement_Speed * 0.7;
			display_Time = display_Time * 0.7;
			display_Score(score);
		}
	}
}
