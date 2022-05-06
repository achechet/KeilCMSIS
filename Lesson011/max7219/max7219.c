#include "stm32f10x.h"                  // Device header
#include "max7219.h"
#include "spi1.h"
#include "rcc_config.h"

void _delay(uint32_t ms) 
		{
			while(ms--) {};
		}
					
void max7219_Init(void) 
		{
			max7219_Transmit(0x09, 0x00);			// без режима декодирования
			max7219_Transmit(0x0A, 0x0F);	    // максимальная яркость
			max7219_Transmit(0x0B, 0x07);			// все разряды
			max7219_Transmit(0x0C, 0x01);			// Выведем из режима сна
			max7219_Transmit(0x0F, 0x01);			// Display test
			_delay(1000);
			max7219_Transmit(0x0F, 0x00);			// Display test OFF
		}
		
void max7219_Transmit(uint8_t adress, uint8_t data) 
		{
			uint8_t tx_buffer[1] = {0};
			CS_SET;
			tx_buffer[0] = adress; 		
			SPI_Trasmit(tx_buffer, 1);
			tx_buffer[0] = data; 
			SPI_Trasmit(tx_buffer, 1);
			CS_RESET;			
		}
