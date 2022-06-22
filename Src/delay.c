/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "delay.h"

/** @addtogroup Template_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Delay Configuration
  * @param  None
  * @retval None
  */
void Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision 			= TIM_CKD_DIV4;
	TIM_TimeBaseInitStruct.TIM_CounterMode 				= TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Prescaler 					= 1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter 	= 0;
	TIM_TimeBaseInitStruct.TIM_Period 						= 41;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
	TIM_Cmd(TIM6, ENABLE);
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
}
/**
  * @brief  Delay microsecond
  * @param  None
  * @retval None
  */

void delay_us(uint32_t DelayTime){
	while (DelayTime){
		if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET){
			TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
			DelayTime--;	
		}
	}
}
/**
  * @brief  Delay milisecond
  * @param  None
  * @retval None
  */
void delay_ms(uint32_t DelayTime){
	while (DelayTime){
		delay_us(1000);
		DelayTime--;
	}
}

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
