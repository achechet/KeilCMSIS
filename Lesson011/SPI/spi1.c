#include "spi1.h"

void SPI_Config(void) 
		{
			/*************** STEPS TO FOLLOW ***************************
				1. Enable SPI Clock
						RCC_APB2ENR
						Bit 12 SPI1EN: SPI 1 clock enable
						Set and cleared by software.
						0: SPI 1 clock disabled
						1: SPI 1 clock enabled
				
				3. Configure the CR2
			************************************************************/
			// 1. Enable SPI Clock
			RCC->APB2ENR |= (1<<12);		
			
			// 2. Congfigure the Control Register
			SPI1->CR1 |= (1<<0);									// CPHA=1
			SPI1->CR1 |= (1<<1);									// CPOL=1 
			SPI1->CR1 |= (1<<2);									// Master MODE select
			SPI1->CR1 &= ~((1<<5)|(1<<4));				// 010 -> 5:4:3 bits
			SPI1->CR1 |= (1<<3);									// Prescale /4 when 24 MHz on Clock
			SPI1->CR1 &= ~(1<<7);									// MSB first 
			SPI1->CR1 |= (1<<8)|(1<<9);						// Software Slave management
			SPI1->CR1 &= ~(1<<10);								// RXONLY = 0 full-duplex
			SPI1->CR1 &= ~(1<<11);								// DFF=0 8 bit data
			SPI1->CR2 = 0;
		}
void SPI_PortInit(void) 
		{
			/****************** STEPS TO FOLLOW **************************
				1. Enable GPIOA clock
			  2. Set the PIN PA
				3. Congigure the output mode
			**************************************************************/
			
			// 1. Enable GPIOC clock
			RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
			
			// 2. Set the PIN PA5 (23-20), PA6 (27-24), PA7 (31-28) 
			// Alternative mode CNF = 10 & MODE = 01 (10 Mhz)
			// and Output mode for PA1 (7654)		CNF = 00 & MODE = 01 (10 Mhz)
						GPIOA->CRL = 0; 
						GPIOA->CRL |= (1<<28)|(1<<31)|(1<<26)|(1<<20)|(1<<23)|(1<<4); 	
		}
		
void CS_Enable(void) 
		{
			GPIOA->BSRR |= GPIO_BSRR_BR1;
		}
void CS_Disable(void)
		{
			GPIOA->BSRR |= GPIO_BSRR_BS1;
		}

void SPI_Enable(void) 
		{
			SPI1->CR1 |= (1<<6);				// SPE=1 Peripheral enable
		}

void SPI_Disable(void) 
		{
			SPI1->CR1 &= ~(1<<6);				// SPE=1 Peripheral disable		
		}		
void SPI_Trasmit(uint8_t *data, int size) 
		{
			/****************** STEPS TO FOLLOW ********************************
				1. Wait for the TXE bit to set in the Status Register
				2. Write data to the Data Register
				3. After the data has benn tranmitted, wait for the BSY bit to reset in Status Register
				4. Clear the Overrun flag by reading DR and SR
			********************************************************************/
			int i =0;
			uint8_t temp;
			while (i < size) 
				{
					while (!((SPI1->SR) & (1<<1))) {};
					// Wait for TXE bit to set -> This will indicate that the buffer is empty
					SPI1->DR = data[1];
						i++;
				}
			while(!((SPI1->SR) & (1<<1))) {};		// wait for the TXE bit to set -> This will indicate that buffer is empty
			while(((SPI1->SR) & (1<<7))) {}; 		// wait for BSY bit to Reset -> this will indicate that SPI is not busy in communication
			
				temp = SPI1->DR;
				temp = SPI1->SR;
		}		
