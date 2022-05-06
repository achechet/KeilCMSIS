#include "stm32f10x.h"                  // Device header
#include "max7219.h"
#include "spi1.h"
#include "rcc_config.h"


uint8_t data[1] = {0x09};
		
int main(void) 
		{
			SPI_PortInit();
			SPI_Config();
			SPI_Enable();
			CS_Enable();
			max7219_Transmit(0x01, 0x01);
			CS_Disable();
			while(1) 
				{
				
				}
		}
