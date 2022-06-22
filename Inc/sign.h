/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SIGN_H
#define __SIGN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/*Defines*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define GPIO_LED_GREEN	GPIO_Pin_12
#define GPIO_LED_ORANGE	GPIO_Pin_13
#define GPIO_LED_RED 		GPIO_Pin_14
#define GPIO_LED_BLUE 	GPIO_Pin_15

/* Exported variables*/
/* Exported functions ------------------------------------------------------- */
void LEDConfig(void);

#endif /* __SIGN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
