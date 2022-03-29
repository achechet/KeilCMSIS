#include "stm32f10x.h"                  // Device header
#include "ds18b20.h"
#include "tim6.h"

int main(void) 
	{
		TIM6_Config();
		ds18b20_PortInit();

		
		while(1) 
			{
				//ds18b20_SendBit(0);
				//ds18b20_SendBit(1);
				ds18b20_Test(); 
				Delay_us(500);
				ds18b20_SendByte(0xCC);
				ds18b20_SendByte(0x4E);
				ds18b20_SendByte(0x64);
				ds18b20_SendByte(0x9E);
				ds18b20_SendByte(0x7F);
				ds18b20_SendByte(0xCC);
				ds18b20_SendByte(0x44);		
				Delay_ms(1);
				ds18b20_SendByte(0xBE);
				ds18b20_Reset();
				//Delay_us(50);
				ds18b20_ReadByte();
				Delay_ms(10);
			}
	}
