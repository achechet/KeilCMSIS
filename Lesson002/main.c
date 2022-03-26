/*
* PC9 Green
*/
#include "stm32f10x.h"                  // Device header
#define SYSCLOCK 24000000U							// макрос нашей системной частоты

__IO uint32_t SysTick_CNT = 0;

void PORTC_9_INIT (void);
void Systick_Init(void);
void delay_mS(uint32_t mS);

void Systick_Init() 
		{
			SysTick->LOAD &= ~SysTick_LOAD_RELOAD_Msk;
			SysTick->LOAD = SYSCLOCK/(1000-1);
			SysTick->VAL &= ~SysTick_VAL_CURRENT_Msk;
			SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
		}

void SysTick_Handler(void) 
		{
			if(SysTick_CNT > 0) SysTick_CNT--;
		}

void delay_mS(uint32_t mS) 
		{
			SysTick->VAL &= ~SysTick_VAL_CURRENT_Msk;
			SysTick->VAL = SYSCLOCK/(1000-1);
			SysTick_CNT = mS;
			while (SysTick_CNT);
		}
		
void PORTC_9_INIT (void) 
		{
			RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
			
			GPIOC->CRH &= ~GPIO_CRH_MODE9_0;
			GPIOC->CRH |= GPIO_CRH_MODE9_1;
			GPIOC->CRH &= ~GPIO_CRH_CNF9;
		}

int main(void) 
		{
			PORTC_9_INIT();
			Systick_Init();
			
			while(1) 
				{
					GPIOC->BSRR |= GPIO_BSRR_BS9;
					delay_mS(1000);
					GPIOC->BSRR |= GPIO_BSRR_BR9;
					delay_mS(1000);					
				}		
		}
