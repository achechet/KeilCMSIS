#include "stm32f10x.h"                  // Device header

void TIM6_Config(void);
void Delay_us(uint16_t us);
void Delay_ms(uint16_t ms);
void portPC9_Init(void);
uint8_t ds18b20_Reset(void);
uint8_t ds18b20_ReadBit(void);
uint8_t ds18b20_ReadByte(void);
void ds18b20_WriteBit(uint8_t bit);
void ds18b20_WriteByte(uint8_t dt);
uint8_t ds18b20_Init(void);

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

uint8_t ds18b20_Init(void) 
		{
			if(ds18b20_Reset()) return 1;
			ds18b20_WriteByte(0xCC);
			ds18b20_WriteByte(0x4E);
			ds18b20_WriteByte(0x64);
			ds18b20_WriteByte(0x9E);
			ds18b20_WriteByte(0x7F);
			return 0;
		}

void ds18b20_WriteByte(uint8_t dt) 
		{
			uint8_t i;
			for (i = 0; i < 8; i++)
			{
				ds18b20_WriteBit(dt >> i & 1);
				// Delay Protection
				Delay_us(5);
			}
		}

void ds18b20_WriteBit(uint8_t bit) 
		{
			GPIOC->ODR &= ~GPIO_ODR_ODR9;
			Delay_us(bit ? 3 : 65);
			GPIOC->ODR |= GPIO_ODR_ODR9;
			Delay_us(bit ? 65 : 3);
		}

uint8_t ds18b20_ReadByte(void) 
		{
			uint8_t data = 0;
			uint8_t i;
			for (i = 0; i < 7; i++)
			data += ds18b20_ReadBit() << i;
			return data;
		}

uint8_t ds18b20_ReadBit(void) 
		{
			uint8_t bit = 0;
			GPIOC->ODR &= ~GPIO_ODR_ODR9;
			Delay_us(2);
			GPIOC->ODR |= GPIO_ODR_ODR9;					// высокий уровень
			Delay_us(13);													// задержка на 15 микросекунд					
			bit = (GPIOC->IDR & GPIO_IDR_IDR9 ? 1 : 0);	// проверяем уровень			
			Delay_us(45);													// задержка на 40 микросекунд			
			return bit;
		}

uint8_t ds18b20_Reset(void) 
		{
			uint16_t status;
			GPIOC->ODR &= ~GPIO_ODR_ODR9;					// низкий уровень
			Delay_us(485);												// задержка на 480 микросекунд
			GPIOC->ODR |= GPIO_ODR_ODR9;					// высокий уровень
			Delay_us(65);													// задержка на 60 микросекунд
			status = GPIOC->IDR & GPIO_IDR_IDR9;	// проверяем уровень
			Delay_us(500);												// задержка на 480 микросекунд = чуть больше
			return (status ? 1 : 0);							// вернем результат
		}

void portPC9_Init(void) 
		{
			/****************** STEPS TO FOLLOW **************************
				1. Enable GPIOC clock
				2. Set the PIN PC9 as output
				3. Congigure the output mode
			**************************************************************/
		
			// 1. Enable GPIOC clock
			RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
			
			// 2. Set the PIN PC9 as output and pp 3. too
			// GPIO - 01 - Open Drain	- 01 Max. output speed 10 MHz
			// CNF9_0 & CNF9_1 - 01 -> CNF
				GPIOC->CRH &= ~GPIO_CRH_MODE9_0;
				GPIOC->CRH |= GPIO_CRH_MODE9_1;
				GPIOC->CRH &= ~GPIO_CRH_CNF9_0;				
//			GPIOC->CRH |= GPIO_CRH_MODE9;
//			GPIOC->CRH |= GPIO_CRH_CNF9_0;
//			GPIOC->CRH &= ~GPIO_CRH_MODE9_1;
		}
int main(void) 
		{
			TIM6_Config();
			portPC9_Init();
			ds18b20_Init();
			while(1) 
				{
					GPIOC->BSRR |= GPIO_BSRR_BR9;
					Delay_us(10000);								// 10 millusecond
					GPIOC->BSRR |= GPIO_BSRR_BS9;		
					Delay_us(10000);		
				}
		}
