#include "joystick.h"

void joy_init(void)
{
    /* Configure the Joystick pins, i.e. port G 7(press), 8(down), 11(up), 13(left), 14(right) */
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
}

uint8_t joy_scan(void)
{
	// All keys of joy are tested and double keys case is also reported//
	/* Joystick pins, port G 7(press), 8(down), 11(up), 13(left), 14(right)
	   cods returned by function
	   0x01 - up, 0x02 - down, 0x03 - left, 0x06 - right, 0x10 - select
	   0x11 - up+select, 0x12 - down+select, 0x13 - left+select, 0x16 - right+select
	   0x04 - up+left, 0x07 - up+select, 0x05- down+left, 0x08 - down+right
	   0x14 - up+left+select, 0x15 - down+left+select, 0x16 - up+right+select, 0x18 - down+right+select
	   	 */
	uint8_t key_code;
	key_code=0;
	if (!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11)) key_code=0x01;
	if (!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_8)) key_code+=0x02;
	if (!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_13)) key_code+=0x03;
	if (!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_14)) key_code+=0x06;
	if (!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7)) key_code=+0x10;
	return key_code;
   	}
