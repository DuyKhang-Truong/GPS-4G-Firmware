/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.h 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Header for main.c module
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPS_HANDLER_H
#define __GPS_HANDLER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "delay.h"
#include "gps_config.h"
#include "access_GUI.h"
#include "sign.h"


/*Defines*/

#define P0x 45.5678
#define P0y 39.2345
#define V0	0.012

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
typedef struct
{
	uint8_t cnt;
	double temp;
    
} AnalysisParam;

typedef struct 
{
	double lat;
	double lon;
	uint16_t degLat;
	uint16_t degLon;
	double minLat;
	double minLon;
	double sog;
	
	uint8_t Lat[10];
	uint8_t Lon[11];
} Co_ordinates;

typedef struct
{
	double Xn1;
	double Xn;
	double Yn1;
	double Yn;
	double xn1;
	double xn;
	double xn_1;
	double yn1;
	double yn;
	double yn_1;
	double Vn1;
	double Vn;
	double Vn_1;
	double D;
	
	double temp;
	uint8_t cntMsg;
	bool initOk;
	
} MethodParam;


/* Exported macro ------------------------------------------------------------*/
/* Exported variables*/

#define RXBUFFERSIZEGPS 52
#define TXBUFFERSIZEGPS 52

extern uint8_t txBufferGPS[TXBUFFERSIZEGPS];
extern uint8_t rxBufferGPS[RXBUFFERSIZEGPS];

extern Co_ordinates	rawLtnInfo;

/* Exported functions ------------------------------------------------------- */
void UART_GPS_Config (uint32_t baudrate);
void UART_WriteChar (uint8_t ch);
void UART_Transmit (uint8_t *str, uint8_t dataLength);
void checkDataIn (uint8_t *rxBuffer);
void reEnableDMARx (uint8_t *rxBuffer);
void checkSpareDataAfterEnDMARx (uint8_t *rxBuffer);
void estimatePos(MethodParam *methodParam, Co_ordinates	rawLtnInfo, Co_ordinates *desireLtnInfo);

void GPRMC_analysis(uint8_t *rawData, AnalysisParam GPSMsgParam, Co_ordinates	*rawLtnInfo, MethodParam *methodParam);
void GPGLL_analysis(uint8_t *rawData, AnalysisParam GPSMsgParam, Co_ordinates	*rawLtnInfo);

#endif /* __GPS_HANDLER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
