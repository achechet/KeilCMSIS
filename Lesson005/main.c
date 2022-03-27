#include "stm32f10x.h"                  // Device header

void SysClockConfig(void);
void GPIO_Config(void);
void delay(uint32_t ms);

void delay(volatile uint32_t ms) 
		{
		while(ms--);
		}

void SysClockConfig(void) 
		{
			/* 
				1. Enable HSE and wait for the HSE to become Ready
				2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
				3. Configure the FLASH PREFETCH and the LATENCY Related Settings
				4. Confugure the PRESCALE HCLK, PCLK1, PCLK2
				5. Configure the MAIN PLL
				6. Enable the PLL and wait for it to become ready
				7. Select the Clock Source and wait for it to be set
			***********************************************************/

			//1. Enable HSE and wait for the HSE to become Ready
			RCC->CR |= RCC_CR_HSEON;
			while(!(RCC->CR & RCC_CR_HSERDY));
			
			// 2. Set the POWER INTERFACE CLOCK ENABLE and VOLTAGE REGULATOR
			// The voltage regulator is always enabled after Reset.
			RCC->APB1ENR |= RCC_APB1ENR_PWREN; 
				
			// 3. Configure the FLASH PREFETCH and the LATENCY Related Settings
			// Scip
			
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

void GPIO_Config(void) 
			{
				/* PC9 Green
					1. Enable the GPIO CLOCK
					2. Set the Pin as OUTPUT
					3. Configure the OUTPUT MODE
				*/
				
				// 1. Enable the GPIO CLOCK
				RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
				
				// 2. Set the Pin as OUTPUT
// #define  GPIO_CRH_MODE9_0                    ((uint32_t)0x00000010)        /*!< Bit 0 */
// #define  GPIO_CRH_MODE9_1                    ((uint32_t)0x00000020)        /*!< Bit 1 */	
				GPIOC->CRH &= ~GPIO_CRH_MODE9;
				// GPIOC->CRH |= GPIO_CRH_MODE9_0;
				GPIOC->CRH |= GPIO_CRH_MODE9_1;
// #define  GPIO_CRH_CNF9_0                     ((uint32_t)0x00000040)        /*!< Bit 0 */
// #define  GPIO_CRH_CNF9_1                     ((uint32_t)0x00000080)        /*!< Bit 1 */						
				GPIOC->CRH &= ~GPIO_CRH_CNF9_0;
			}
		
int main(void) 
	{
		SysClockConfig();
		GPIO_Config();
		while(1) 
			{
				GPIOC->BSRR |= GPIO_BSRR_BS9;
				delay(2000000);
				GPIOC->BSRR |= GPIO_BSRR_BR9;
				delay(2000000);
			}
	}
