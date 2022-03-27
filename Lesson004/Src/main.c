/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
uint8_t ds_buff[9];                  //сюда принимаем данные 
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */
#define PORT  GPIOB   //указать порт, к которому подключены датчики
#define PIN 9
#define TIMER TIM3    //задаем таймер, используемый для формирования задержек
#define SKIP_ROM_ADR 0xCC // пропуск адресации
#define CONVERT_TEMP 0x44 // запускает преобразование температуры
#define READ_DATA_COMAND 0xBE //команда чтения содержимого памяти

 uint16_t	temp;
// uint32_t temp;
 char znak =  '-' ;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
//*********************************************************************************************
//function  импульс сброса                                                                   //
//argument  маска порта                                                                      //
//return    0 - устройство обнаружен, 1 - не обнаружено, 2 - к.з. на линии                   //
//*********************************************************************************************
uint8_t ds_reset_pulse(uint16_t PinMask)
{
   uint16_t result;   
 
   if((PORT->IDR & PinMask)==0)  return 2;         //проверить линию на отсутствие замыкания
   PORT->ODR &= ~PinMask;                          //потянуть шину к земле
   TIMER->CNT=0;
   while(TIMER->CNT<480){};                        //ждать 480 микросекунд
   PORT->ODR |=  PinMask;                          //отпустить шину
   while(TIMER->CNT<550){};                        //ждать 70 микросекунд
   result =  PORT->IDR & PinMask;                 //прочитать шину 
   while(TIMER->CNT<960){};                        //дождаться окончания инициализации
   if(result) return 1;                            //датчик не обнаружен
   return 0;                                       //датчик обнаружен      
}


//*********************************************************************************************
//function  передача бита                                                                    //
//argument  значение передаваемого бита,маска порта                                          //
//return    none                                                                             //
//*********************************************************************************************
void ds_write_bit(uint8_t bit,uint16_t PinMask)
{
   TIMER->CNT=0;
   PORT->ODR &= ~PinMask;                          //потянуть шину к земле
   while(TIMER->CNT<2){};                          //ждать 1 микросекунду
   if(bit) PORT->ODR |=  PinMask;                  //если передаем 1, то отпускаем шину
   while(TIMER->CNT<60){};                         //задержка 60 микросекунд 
   PORT->ODR |=  PinMask;                          //отпускаем шину 
}

//*********************************************************************************************
//function  чтение бита                                                                      //
//argument  маска порта                                                                      //
//return    прочитанный бит                                                                  //
//*********************************************************************************************
uint16_t ds_read_bit(uint16_t PinMask)
{
   uint16_t result;
 
  TIMER->CNT=0;
   PORT->ODR &= ~PinMask;                          //потянуть шину к земле
   while(TIMER->CNT<2){};
   PORT->ODR |=  PinMask;                          //отпускаем шину  
   while(TIMER->CNT<15){};                         //задержка 15 микросекунд
   result     =  PORT->IDR & PinMask;              //прочитать шину
   while(TIMER->CNT<60){};                         //оставшееся время 
   return result;                                  //возвратить результат
}
//*********************************************************************************************
//function  запись байта                                                                     //
//argument  передаваемый байт,маска порта                                                    //
//return    none                                                                             //
//*********************************************************************************************
void ds_write_byte(uint8_t byte, uint16_t PinMask)
{
   uint8_t i;
   for(i=0;i<8;i++) ds_write_bit(byte&(1<<i), PinMask);
}
//*********************************************************************************************
//function  чтение байта                                                                     //
//argument  маска порта                                                                      //
//return    прочитанный байт                                                                 //
//*********************************************************************************************
uint8_t ds_read_byte(uint16_t PinMask)
{
   uint8_t i,result = 0;
   for(i=0;i<8;i++) 
   if(ds_read_bit(PinMask)) result |= 1<<i; 
   return result;
}
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();

  /* USER CODE BEGIN 2 */
HAL_TIM_Base_Start(&htim3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

ds_reset_pulse(1<<PIN);          //послать импульс сброса                                       /
ds_write_byte(SKIP_ROM_ADR,1<<PIN);//разрешить доступ к датчику не используя адрес
ds_write_byte(CONVERT_TEMP,1<<PIN);      //запустить преобразование
HAL_Delay (1000);               //дать время для измерения
ds_reset_pulse(1<<PIN);          //послать импульс сброса              
ds_write_byte(SKIP_ROM_ADR,1<<PIN);//разрешить доступ к датчику не используя адрес
ds_write_byte(READ_DATA_COMAND,1<<PIN);      //команда, заставляющая датчик выдать 9 байт своей памяти
for(int i=0; i<9; i++ )           //прочитать 9 байт в массив
ds_buff[i] = ds_read_byte(1<<PIN);
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////		
 
//  ds_buff[1]=255;//255 проверка отрицательных температур
//	ds_buff[0]=94;//94//248

//		temp = ds_buff[1];
//		temp = temp<<8;
//		temp |= ds_buff[0];
//		//переводим в градусы
//		temp *= 0.0625;//0.0625 градуса на единицу данных	
//		if(temp > 1000)//если температура <0    1111 1111 1111 1000 = 65528
//	{
//		temp = 4096 - temp;
//	  znak =  '-';
//	}else znak =  '+';
	
	
		temp = ds_buff[1];
		temp = temp<<8;
		temp |= ds_buff[0];
		temp =temp>>4;//отбрасываем дробную чать
		if(temp > 1000)//если температура <0    
	{
		temp = 4096 - temp;
	  znak =  '-';
	}else znak =  '+';
	
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}

/* TIM3 init function 
*  Настройка таймера Делитель 7, счет вверх, период 1000
* 
*/
void MX_TIM3_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 7;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim3);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
Выход настроен как Open Drain
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOD_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
