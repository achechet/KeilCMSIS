#include "stm32f10x.h"                  // Device header
#include "tim6.h"
#include "ds18b20.h"

void ds18b20_PortInit(void) 
		{
					{
			/****************** STEPS TO FOLLOW **************************
				1. Enable GPIOC clock
			  2. Set the PIN PC7 as output for 1-WIRE
				3. Congigure the output mode
			**************************************************************/
			
			// 1. Enable GPIOC clock
			RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
			
			// 2. Set the PIN PC7 as output and pp 3. too
			// Open Drain & 50 Mhz
			GPIOC->CRL &= ~GPIO_CRL_MODE7_0;
			GPIOC->CRL |= GPIO_CRL_MODE7;
			GPIOC->CRL &= ~GPIO_CRL_CNF7_0;		
			GPIOC->CRL |= GPIO_CRL_CNF7_0; 		
		}
		}
void ds18b20_SendBit(int logic) 
		{
			if(logic == 0)
					{
						SWI_LOW;
						Delay_us(60);
						SWI_FLOAT;
						Delay_us(15);
					}
			else
					{
						SWI_LOW;
						Delay_us(5);
						SWI_FLOAT;
						Delay_us(60);
					}
			}
void ds18b20_SendByte(uint8_t ch) 
		{
			uint8_t i;
			for (i = 0; i < 8 ; i++)
				ds18b20_SendBit(ch & (0x01 << i));
		}

uint8_t ds18b20_ReadBit(void) 
		{
			uint8_t bit = 0;
			SWI_LOW;
			Delay_us(2);
			SWI_FLOAT;
			Delay_us(13);
			bit = (GPIOC->IDR & GPIO_IDR_IDR7 ? 1 : 0);
			Delay_us(45);
			return bit;
		}

uint8_t ds18b20_ReadByte(void) 	
		{
			uint8_t data, i = 0;
			for (i = 0; i <= 7; i++)
			data += ds18b20_ReadBit() << i;
			return data;
		}
		
void ds18b20_Reset(void) 
		{
			SWI_LOW;
			Delay_us(60);
			SWI_FLOAT;
			Delay_us(50);		
		}		
void ds18b20_Test(void) 
		{
			SWI_LOW;
			Delay_us(720);
			SWI_FLOAT;
			Delay_us(10);
		}
