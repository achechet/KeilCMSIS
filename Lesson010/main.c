#include "stm32f4xx.h"                  // Device header

uint32_t Flash_Write_Data (uint32_t StartPageAddress, uint32_t *Data, uint16_t numberofwords);
void Flash_Read_Data (uint32_t StartPageAddress, uint32_t *RxBuf, uint16_t numberofwords);

void Flash_Read_Data (uint32_t StartPageAddress, uint32_t *RxBuf, uint16_t numberofwords)
{
	while (1)
	{

		*RxBuf = *(__IO uint32_t *)StartPageAddress;
		StartPageAddress += 4;
		RxBuf++;
		if (!(numberofwords--)) break;
	}
}

uint32_t Flash_Write_Data (uint32_t StartPageAddress, uint32_t *Data, uint16_t numberofwords)
{

	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError;
	int sofar=0;

	  /* Unlock the Flash to enable the flash control register access *************/
	   HAL_FLASH_Unlock();

	   /* Erase the user Flash area*/

	  uint32_t StartPage = GetPage(StartPageAddress);
	  uint32_t EndPageAdress = StartPageAddress + numberofwords*4;
	  uint32_t EndPage = GetPage(EndPageAdress);

	   /* Fill EraseInit structure*/
	   EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	   EraseInitStruct.PageAddress = StartPage;
	   EraseInitStruct.NbPages     = ((EndPage - StartPage)/FLASH_PAGE_SIZE) +1;

	   if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	   {
	     /*Error occurred while page erase.*/
		  return HAL_FLASH_GetError ();
	   }

	   /* Program the user Flash area word by word*/

	   while (sofar<numberofwords)
	   {
	     if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartPageAddress, Data[sofar]) == HAL_OK)
	     {
	    	 StartPageAddress += 4;  // use StartPageAddress += 2 for half word and 8 for double word
	    	 sofar++;
	     }
	     else
	     {
	       /* Error occurred while writing data in Flash memory*/
	    	 return HAL_FLASH_GetError ();
	     }
	   }

	   /* Lock the Flash to disable the flash control register access (recommended
	      to protect the FLASH memory against possible unwanted operation) *********/
	   HAL_FLASH_Lock();

	   return 0;
}

int main(void) {
          char *data = "hello FLASH from ControllerTech\
                      This is a test to see how many words can we work with";

        uint32_t data2[] = {0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9};

        uint32_t Rx_Data[30];

        char string[100];

        int number = 123;

        float val = 123.456;

        float RxVal;

          Flash_Write_Data(0x08004410 , (uint32_t *)data2, 9);
          Flash_Read_Data(0x08004410 , Rx_Data, 10);


          int numofwords = (strlen(data)/4)+((strlen(data)%4)!=0);
          Flash_Write_Data(0x08004810 , (uint32_t *)data, numofwords);
          Flash_Read_Data(0x08004810 , Rx_Data, numofwords);
          Convert_To_Str(Rx_Data, string);


          Flash_Write_NUM(0x08005C10, number);
          RxVal = Flash_Read_NUM(0x08005C10);

          Flash_Write_NUM(0x08012000, val);
          RxVal = Flash_Read_NUM(0x08012000);
    while(1) {
        
    }
}

