/*
* PC9 Green
*/
#include "stm32f10x.h"                  // Device header

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
			
			while(1) 
				{
					GPIOC->BSRR |= GPIO_BSRR_BS9;
					GPIOC->BSRR |= GPIO_BSRR_BR9;
				}		
		}
	
		