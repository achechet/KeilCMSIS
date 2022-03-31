#include "stm32f10x.h"                  // Device header
#include "ds18b20.h"
#include "tim6.h"

/* USER CODE BEGIN PV */
#define PORT  GPIOC   //������� ����, � �������� ���������� �������
#define PIN 7

		uint8_t ds_buff[9];
		uint8_t i,temp,b;
		uint16_t raw_temper;
		float temper;

int main(void) 
	{
		TIM6_Config();
		ds18b20_PortInit();

		while(1) 
			{
				ds18b20_Reset();
				ds18b20_SendByte(0xCC);
				ds18b20_SendByte(0x44);
				Delay_us(1000);
				ds18b20_Reset();
				ds18b20_SendByte(0xCC);	
				ds18b20_SendByte(0xBE);		
				for(i=0; i<9; i++ )           //��������� 9 ���� � ������
				ds_buff[i] = ds18b20_ReadByte();
//				temp = ds_buff[1];
//				temp =temp <<8;
//				temp |= ds_buff[0];
				raw_temper = ((uint16_t) ds_buff[1]<<8)|ds_buff[0];
				temper = ds18b20_Convert(raw_temper);
				Delay_ms(10);
			}
	}
