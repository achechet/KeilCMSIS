/**
* @file: tim6.h
* @brief: STM32F100RB 
*/

#ifndef TIM6_H
#define TIM6_H

#ifdef __cplusplus
extern "C" {
#endif

/********* INLUDES ****************************************/


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

void TIM6_Config(void);
void Delay_us(uint16_t us);
void Delay_ms(uint16_t ms);

/**********************************************************/



#ifdef __cplusplus
}
#endif

#endif
/************************* END OF FILE *********************/

