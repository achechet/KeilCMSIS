#include "stm32f10x.h"                  // Device header
#include "tim6.h"

void TIM6_Config(void) 
		{
			/******************** STEPS TO FOLLOW ************************ 
				0x4000 1000 - 0x4000 13FF TIM6 timer	APB1 Bus
			
				1. Enable Timer clock
						Bit 4 TIM6EN: Timer 6 clock enable
									Set and cleared by software.
									0: Timer 6 clock disabled
									1: Timer 6 clock enabled
				2. Set the prescalar and the ARR
				3. Enable the Timer and wait for the update Flag to set
			**************************************************************/
			
			// 1. Enable Timer clock
			RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
			
			// 2. Set the prescalar and the ARR
			TIM6->PSC = 24-1;
			TIM6->ARR = 0xffff;
			
			// 3. Enable the Timer and wait for the update Flag to set
			TIM6->CR1 |= TIM_CR1_CEN;							// start timer
			while(!(TIM6->SR & (1<<0)));
		}

void Delay_ms(uint16_t ms) 
		{
			uint16_t i;
			for (i=0 ; i < ms; i++) 
				{
					Delay_us(1000);
				}
		}

void Delay_us(uint16_t us) 
		{
			/****************** STEPS TO FOLLOW **************************
				1. Reset the Counter
				2. Wait for the Counter to reach the entered value. As each take 1 us
					 the total waiting time will be the required us delay
			**************************************************************/
			
			// 1. Reset the Counter
			TIM6->CNT = 0;
			while (TIM6->CNT < us);			
		}
		
