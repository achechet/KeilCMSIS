#include "rcc_config.h"
#define SYSCLOCK 24000000U							// макрос нашей системной частоты

__IO uint32_t SysTick_CNT = 0;

void SysClockConfig (void) 
		{
			/*
				1. ENABLE HSE and wait for the HSE to becomeReady
				2. Set the POWER ENABLE CLOCK 
			*/
			
			// 1. ENABLE HSE and wait for the HSE to becomeReady
			RCC->CR |= (1<<0); 							// HSION: Internal high-speed clock enable
			while (!(RCC->CR) & (1<<1));		// HSIRDY: Internal high-speed clock ready flag
			// 2. Set the POWER ENABLE CLOCK 
			RCC->APB1ENR |= (1<<28);				// PWREN: Power interface clock enable
			// PWR->CR |= PWR_CR_PVDE;			// ???
			// 3. Configure the FLASH PREFETCH and the LATENCY Related Settings
			// Skipping
			
			// 4. Confugure the PRESCALE HCLK, PCLK1, PCLK2
			RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
			RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;
			RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
			
			// 5. Configure the MAIN PLL
			RCC->CFGR |= RCC_CFGR_MCO_HSE;
			RCC->CFGR |= RCC_CFGR_PLLMULL13;
			
			// 6. Enable the PLL and wait for it to become ready
			RCC->CR |= RCC_CR_PLLON;
			while(!(RCC->CR & RCC_CR_PLLRDY));
			
			// 7. Select the Clock Source and wait for it to be set
			RCC->CFGR |= RCC_CFGR_SWS_PLL;
			while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
		}

void Systick_Init() 
		{
			SysTick->LOAD &= ~SysTick_LOAD_RELOAD_Msk;
			SysTick->LOAD = SYSCLOCK/(1000-1);
			SysTick->VAL &= ~SysTick_VAL_CURRENT_Msk;
			SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
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
