/**
* @file: rcc_config.h
* @brief: STM32F100RB 
*/

#ifndef RCC_CONFIG_H
#define RCC_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/********* INLUDES ****************************************/
#include "stm32f10x.h"                  // Device header

void SysClockConfig (void);
void PORTC_9_INIT (void);
void Systick_Init(void);
void delay_mS(uint32_t mS);

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


/********* PRIVATE FUNCTIONS ******************************/



/**********************************************************/






#ifdef __cplusplus
}
#endif

#endif
/************************* END OF FILE *********************/

