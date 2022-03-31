/**
* @file: ds18b20.h
* @brief: STM32F100RB GPIOC Pin9 user example 1-WIRE driver
*/

#ifndef DS18B20_H
#define DS18B20_H

#ifdef __cplusplus
extern "C" {
#endif

/********* INLUDES ****************************************/
#include "stm32f10x.h"                  // Device header


/********* DEFINES ****************************************/
#ifndef TYPES
#define TYPES

// Определения типов для 32-разрядных процессоров
#define int8_t          signed char
#define uint8_t         unsigned char
#define int16_t         signed short int
#define uint16_t        unsigned short int
#define int32_t         signed int
#define uint32_t        unsigned int

#endif

#define SWI_FLOAT		GPIOC->BSRR |= GPIO_BSRR_BS7
#define SWI_LOW			GPIOC->BSRR |= GPIO_BSRR_BS7 << 16

/********* PRIVATE FUNCTIONS ******************************/

void ds18b20_PortInit(void);
void ds18b20_SendBit(int logic);
void ds18b20_SendByte(uint8_t ch);
uint8_t ds18b20_ReadBit(void);
uint8_t ds18b20_ReadByte(void);
void ds18b20_Test(void);
uint8_t ds18b20_Reset(void);
uint8_t ds18b20_GetSign(uint16_t dt);
float ds18b20_Convert(uint16_t dt);

/**********************************************************/



#ifdef __cplusplus
}
#endif

#endif
/************************* END OF FILE *********************/

