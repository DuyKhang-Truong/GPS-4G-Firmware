/* Includes ------------------------------------------------------------------*/
#include "sign.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  LED Configuaration
  * @param  None
  * @retval None
  */
void LEDConfig(void)
{
	/*LED for indication that TIM7 is working*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef 									IC_GPIO_InitStruct;
	IC_GPIO_InitStruct.GPIO_Mode 			= GPIO_Mode_OUT;
	IC_GPIO_InitStruct.GPIO_OType 		= GPIO_OType_PP;
	IC_GPIO_InitStruct.GPIO_PuPd 			= GPIO_PuPd_NOPULL;
	IC_GPIO_InitStruct.GPIO_Speed			= GPIO_Speed_100MHz;
	IC_GPIO_InitStruct.GPIO_Pin 			= GPIO_LED_GREEN | GPIO_LED_ORANGE | GPIO_LED_RED | GPIO_LED_BLUE;
	GPIO_Init(GPIOD, &IC_GPIO_InitStruct);
	
}


/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
