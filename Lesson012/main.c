#include "stm32f10x.h"
#include "TM1638.h"
#include <stdlib.h>
extern u8 digitToSegment[];
int i;
u8 digit_Mas[30];
const u8 start_level = 4;
u8 level = start_level;
u8 score=0;
u8 mode=1;
u8 read_Keys_Flag =0;
u16 delay_Count = 0;
u8 debonce_Counter;
u8 key;
u8 debag;
//=========================================================================//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//=========================================================================//
void delay(u16 delay){
   delay_Count=delay;
	 while(delay_Count){};
}
//=========================================================================//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//=========================================================================//
void SysTick_Handler(void)
{ 
	if (delay_Count!=0)delay_Count--;
  if(!read_Keys_Flag)read_Keys_Flag=1;
}
//=========================================================================//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//=========================================================================//
void SPI1_Init(){
GPIO_InitTypeDef GPIO_InitStructure;
SPI_InitTypeDef SPI_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//12
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA , &GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ; //SPI sck 12
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
GPIO_Init(GPIOA, &GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7; //mosi
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
GPIO_Init(GPIOA, &GPIO_InitStructure);
//Заполняем структуру с параметрами SPI модуля
SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx; 
SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // передаем по 8 бит
SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // Полярность и
SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; // фаза тактового сигнала
SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; // Управлять состоянием сигнала NSS аппаратно
SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; // Предделитель SCK
SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB; // Первым отправляется старший бит
SPI_InitStructure.SPI_CRCPolynomial = 0x7;
SPI_InitStructure.SPI_Mode = SPI_Mode_Master; // Режим - 
SPI_Init(SPI1, &SPI_InitStructure); //Настраиваем SPI1
SPI_Cmd(SPI1, ENABLE); // Включаем модуль SPI1....
SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
}
u8 get_Random(u8 min,u8 max){
//srand(SysTick->VAL);
return  min + rand() % max;
}
//=========================================================================//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//=========================================================================//
void debounce_Processing(){
     u8  data = read_Keys();
     u8  debonce_Threshold =15;
			 switch (data){
				 case 0x00:
				      debonce_Counter=0;
				      key = 0;
				 break;
				 case 0x01:
				      if(debonce_Counter<debonce_Threshold)debonce_Counter++;
				      else key = 1;
				 break;
				 case 0x02:
					    if(debonce_Counter<debonce_Threshold)debonce_Counter++;
				      else key = 2;
				 break;
				 case 0x04:
					    if(debonce_Counter<debonce_Threshold)debonce_Counter++;
				      else key = 3;
				 break;
				 case 0x08:
					    if(debonce_Counter<debonce_Threshold)debonce_Counter++;
				      else key = 4;
				 break;
				 case 0x10:
					    if(debonce_Counter<debonce_Threshold)debonce_Counter++;
				      else key = 5;
				 break;
				 case 0x20:
					    if(debonce_Counter<debonce_Threshold)debonce_Counter++;
				      else key = 6;
				 break;
				 case 0x40:
					    if(debonce_Counter<debonce_Threshold)debonce_Counter++;
				      else key = 7;
				 break;
				 case 0x80:
					    if(debonce_Counter<debonce_Threshold)debonce_Counter++;
				      else key = 8;
				 break;
			 }
}
//=========================================================================//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//=========================================================================//
int main(){

u8 data =0;
u8 rnd ;	
RCC_ClocksTypeDef clocks;	
SysTick_Config(72000);//1ms
SPI1_Init();	
delay(500);	
tm1638_Init(7);
RCC_GetClocksFreq(&clocks);	
for (i=0;i<4444;i++){
		  display_Digit(i,5);
		  display_Digit(i+i,1);}
			delay(500);
			tm1638_Clear(1);
	while(1){
	
		 
			//leds_Set(LED_1|LED_2|LED_3|LED_4|LED_5|LED_6|LED_7|LED_8);//LED_N<<LED_N
			//leds_Set(data);//255=all ledsn
				
				for (i=0;i<=level;i++){//генерируем массив светодиодов
				   digit_Mas[i]=get_Random(1,8);
					 rnd = get_Random(1,8);
				   if(mode==0)led_Set(digit_Mas[i],1);
					    else if (mode==1)segmentN_Set(rnd,digit_Mas[i]);
					 delay(500);
					 if(mode==0)led_Set(digit_Mas[i],0);
					   else if (mode==1)segmentN_Set(rnd,17);
					 delay(500);
				}
				for (i=0;i<=level;i++){//проверка совпадения ответа
				   while(!key){
					   if(read_Keys_Flag){ 
			         debounce_Processing();
		           read_Keys_Flag=0;
		         }
					 }
						  
					  data = key; 
						if(digit_Mas[i]==data){
							 key=0;
						   led_Set(data,1);
							 if (mode==1)segmentN_Set(digit_Mas[i],digit_Mas[i]);
               delay(500);
               led_Set(data,0);
							 if (mode==1)segmentN_Set(digit_Mas[i],17);
							    
						   if(level==i)score++;//увеличиваем счет
						 }else{
							 key=0;
							 for(i=1;i<9;i++){
							    delay(100);
									segmentN_Set(i, 16);//прочерки
							   } 
							 for(i=1;i<9;i++){
							    delay(100);
									segmentN_Set(i,17);
							   }
						   if (score!=0)score--;
						   if (score==0 && level>start_level )level--;	
               break;		//?					 
						 }
				   
				}
				 
				if (score>3){//если счет (кол-во правлиьных ответов)
					   level++;//увеличиваем уровень
					   score=0;
				    }
				 for(i=level;i>0;i--){
				    delay(300);
				    segmentN_Set(1,i);//
           }
				 delay(300);
				 segmentN_Set(1,17);
				 delay(800);
				 leds_Set(LED_1|LED_2|LED_3|LED_4|LED_5|LED_6|LED_7|LED_8);//LED_N<<LED_N 
				 delay(800);
				 leds_Set(0);//LED_N<<LED_N
				 segmentN_Set(1,17);
 
	}      

}
