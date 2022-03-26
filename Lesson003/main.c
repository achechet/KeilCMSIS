/*
* PC9 Green
*/
#include "stm32f10x.h"                  // Device header
#define SYSCLOCK 24000000U							// макрос нашей системной частоты
#define TIM_EnableIT_UPDATE(TIMx) TIMx->DIER |= TIM_DIER_UIE 	// макрос разрешения прерывания
#define TIM_EnableCounter(TIMx)		TIMx->CR1 |= TIM_CR1_CEN		// макрос включения таймера

#define PIN_C9_ON()  GPIOC->BSRR |= GPIO_BSRR_BS9;
#define PIN_C9_OFF() GPIOC->BSRR |= GPIO_BSRR_BR9;

static __IO uint32_t SysTick_CNT = 0;
static __IO uint32_t tim3_cnt = 0;

void PORTC_9_INIT (void);
void Systick_Init(void);
void delay_mS(uint32_t mS);
void SysTick_Handler(void);
static void TIM3_Init(void); 
void TIM3_IRQHandler (void);

static void TIM3_Init(void) 
		{
			RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
			NVIC_EnableIRQ (TIM3_IRQn);
			WRITE_REG(TIM3->PSC, 2399);			// 24 000 000 / (2399 + 1) = 10 000 за 1 сек
			WRITE_REG(TIM3->ARR, 10000);		// 10 000 это получится задержка в 1 сек
		}

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
			SysTick->VAL = SYSCLOCK/1000-1;
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
			TIM3_Init();
			TIM_EnableIT_UPDATE(TIM3);
			TIM_EnableCounter(TIM3);
			
			while(1) 
				{
//					GPIOC->BSRR |= GPIO_BSRR_BS9;
//					delay_mS(1000);
//					GPIOC->BSRR |= GPIO_BSRR_BR9;
//					delay_mS(1000);					
				}		
		}

		void TIM3_IRQHandler (void) 
		{
			if(READ_BIT(TIM3->SR, TIM_SR_UIF))
			{
				CLEAR_BIT(TIM3->SR, TIM_SR_UIF);
				
				switch(tim3_cnt) 
					{
					case 0: PIN_C9_ON() break;
					case 1: PIN_C9_OFF() break;						
					}
					tim3_cnt++;
					if(tim3_cnt>>1) tim3_cnt = 0;
			}
		}
