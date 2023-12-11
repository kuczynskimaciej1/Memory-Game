/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               systick.c
** Descriptions:            ...
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-10-30
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_systick.h"
#include "stm32f10x_rcc.h"

/* Private variables ---------------------------------------------------------*/	 
static uint8_t  delay_fac_us = 0;
static uint16_t delay_fac_ms = 0;

/*******************************************************************************
* Function Name  : delay_init
* Description    :
* Input          : None
* Output         : None
* Return         : None
* Attention		 : SYSTICK 1/8
*******************************************************************************/
void delay_init(void)
{
    RCC_ClocksTypeDef RCC_ClocksStatus;

    RCC_GetClocksFreq(&RCC_ClocksStatus);
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	SysTick_ITConfig(DISABLE);
    delay_fac_us = RCC_ClocksStatus.HCLK_Frequency / 8000000;
    delay_fac_ms = RCC_ClocksStatus.HCLK_Frequency / 8000;      
}
					
/*******************************************************************************
* Function Name  : delay_us
* Description    : 初始化延迟函数 
* Input          : - Nus: 延时us
* Output         : None
* Return         : None
* Attention		 : 参数最大值为 0xffffff / (HCLK / 8000000)
*******************************************************************************/            
void delay_us(u32 Nus)
{ 
	FlagStatus	Status;
    SysTick_SetReload(delay_fac_us * Nus);
    SysTick_CounterCmd(SysTick_Counter_Clear);
    SysTick_CounterCmd(SysTick_Counter_Enable);
    do
    {
        Status = SysTick_GetFlagStatus(SysTick_FLAG_COUNT);
    }while (Status != SET);
    SysTick_CounterCmd(SysTick_Counter_Disable);
	SysTick_CounterCmd(SysTick_Counter_Clear);
}
/*******************************************************************************
* Function Name  : delay_ms
* Description    : 初始化延迟函数 
* Input          : - nms: 延时ms
* Output         : None
* Return         : None
* Attention		 : nms<=0xffffff*8*1000/SYSCLK,nms<=1864
*******************************************************************************/  
void delay_ms(uint16_t nms)
{    
    uint32_t temp = delay_fac_ms * nms;
    FlagStatus	Status;
    if (temp > 0x00ffffff)
    {
        temp = 0x00ffffff;
    }
    SysTick_SetReload(temp);		             /* 时间加载 */
    SysTick_CounterCmd(SysTick_Counter_Clear);	 /* 清空计数器 */
    SysTick_CounterCmd(SysTick_Counter_Enable);	 /* 开始倒数 */ 
    do
    {
        Status = SysTick_GetFlagStatus(SysTick_FLAG_COUNT);
    }while (Status != SET);				         /* 等待时间到达 */
    SysTick_CounterCmd(SysTick_Counter_Disable); /* 关闭计数器 */
	SysTick_CounterCmd(SysTick_Counter_Clear);	 /* 清空计数器 */
}
/*******************************************************************************
* Function Name  : Systick_on_ms
* Description    : SG 2016
* Input          : nms
* Output         : None
* Return         : flag
* Attention		 : nms<=1864
*******************************************************************************/
void Systick_on_ms(uint16_t nms)
{
    uint32_t temp = delay_fac_ms * nms;
    if (temp > 0x00ffffff)
    {
        temp = 0x00ffffff;
    }
    SysTick_SetReload(temp);
    SysTick_CounterCmd(SysTick_Counter_Clear);
    SysTick_CounterCmd(SysTick_Counter_Enable);
}
/*******************************************************************************
* Function Name  : Systick_off
* Description    : SG 2016
* Input          : nms
* Output         : None
* Return         : None
* Attention		 : None
* *******************************************************************************/
void Systick_off(void)
{
    SysTick_CounterCmd(SysTick_Counter_Disable);
    SysTick_CounterCmd(SysTick_Counter_Clear);
}
/*******************************************************************************
* Function Name  : Systick_check_time_out
* Description    : SG 2016
* Input          : nms
* Output         : None
* Return         : flag
* Attention		 : None
*******************************************************************************/
FlagStatus Systick_check_time_out(void)
{
	FlagStatus	Status;
	Status = SysTick_GetFlagStatus(SysTick_FLAG_COUNT);
	return Status;
}
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
