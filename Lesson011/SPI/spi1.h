/**
* @file: spi1.h
* @brief: STM32F100RB 
*/

#ifndef SPI1_H
#define SPI1_H

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

#define CS_RESET				GPIOA->BSRR |= GPIO_BSRR_BS1
#define CS_SET			GPIOA->BSRR |= GPIO_BSRR_BS1 << 16

/********* PRIVATE FUNCTIONS ******************************/

void SPI_Config(void);
void SPI_PortInit(void);
void SPI_Enable(void);
void SPI_Disable(void);
void CS_Enable(void);
void CS_Disable(void);
void SPI_Trasmit(uint8_t *data, int size);

/**********************************************************/






#ifdef __cplusplus
}
#endif

#endif
/************************* END OF FILE *********************/

