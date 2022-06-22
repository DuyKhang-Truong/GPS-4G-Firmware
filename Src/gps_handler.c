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
#include "gps_handler.h"

/** @addtogroup Template_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
		/**USART**/
			#define 	GPS_TX_PIN  				   	GPIO_Pin_8			
			#define 	GPS_TX_PORT 				  	GPIOD
			#define 	GPS_TX_CLK							RCC_AHB1Periph_GPIOD			
			#define 	GPS_TX_SOURCE				 		GPIO_PinSource8				
			#define		GPS_TX_CLK_Cmd		  	 	RCC_AHB1PeriphClockCmd			

			#define 	GPS_RX_PIN  				   	GPIO_Pin_9
			#define 	GPS_RX_PORT 				  	GPIOD
			#define 	GPS_RX_CLK							RCC_AHB1Periph_GPIOD
			#define 	GPS_RX_SOURCE				 		GPIO_PinSource9
			#define		GPS_RX_CLK_Cmd		  	 	RCC_AHB1PeriphClockCmd
			
			#define		GPS_USART								USART3
			#define 	GPS_USART_CLK						RCC_APB1Periph_USART3
			#define		GPS_USART_CLK_Cmd		 		RCC_APB1PeriphClockCmd
			#define 	GPS_USART_BAUDRATE			115200
			#define		GPS_USART_AF						GPIO_AF_USART3
			#define 	GPS_USART_IRQn					USART3_IRQn
			#define 	GPS_USART_IRQPreemptionPriority		0x00
			#define 	GPS_USART_IRQSubPriority					0x00
			
			#define   GPS_DMA_Channel					DMA_Channel_4
			#define   GPS_DMA_Stream					DMA1_Stream1
			#define		GPS_DMA_CLK_Cmd		  		RCC_AHB1PeriphClockCmd
			#define 	GPS_DMA_CLK							RCC_AHB1Periph_DMA1
			#define		GPS_DMA_Stream_IRQn			DMA1_Stream1_IRQn
			#define 	GPS_DMA_IRQPreemptionPriority		0x02
			#define 	GPS_DMA_IRQSubPriority					0x01
			
			#define   GPS_DMA_TX_Channel			DMA_Channel_4
			#define   GPS_DMA_TX_Stream				DMA1_Stream3
			#define		GPS_DMA_TX_Stream_IRQn			DMA1_Stream3_IRQn
			
			#define		GPS_DMA_Stream_IRQHandler			DMA1_Stream1_IRQHandler
			#define		GPS_USART_IRQHandler					USART3_IRQHandler
			
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint8_t txBufferGPS[];
uint8_t rxBufferGPS[];

//double P0[2] = {45.5678, 39.2345};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
AnalysisParam	GPSMsgParam;
Co_ordinates	rawLtnInfo;
Co_ordinates	desireLtnInfo;
MethodParam		methodParam = {0, 0, 0, 0, 0, 0, P0x, 0, 0, P0y, 0, 0, V0, 0, 0, 0, false};
/**
  * @brief  Uart Configuration
  * @param  baudrate
  * @retval None
  */
void UART_GPS_Config (uint32_t baudrate)
{

 /* GPIO Configuration for UART3*/

 GPIO_InitTypeDef 	GPIO_InitStructure;
	
 GPS_TX_CLK_Cmd(GPS_TX_CLK, ENABLE);	
 GPIO_InitStructure.GPIO_Pin = GPS_TX_PIN;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPS_TX_PORT, &GPIO_InitStructure);
 GPIO_PinAFConfig(GPS_TX_PORT, GPS_TX_SOURCE, GPS_USART_AF);
	
 GPS_RX_CLK_Cmd(GPS_RX_CLK, ENABLE);	
 GPIO_InitStructure.GPIO_Pin = GPS_RX_PIN;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPS_RX_PORT, &GPIO_InitStructure);
	
 GPIO_PinAFConfig(GPS_RX_PORT, GPS_RX_SOURCE, GPS_USART_AF);
	
 GPS_USART_CLK_Cmd(GPS_USART_CLK, ENABLE);
 USART_InitTypeDef 						USART_InitStructure;
 USART_InitStructure.USART_BaudRate = baudrate;
 USART_InitStructure.USART_WordLength = USART_WordLength_8b;
 USART_InitStructure.USART_StopBits = USART_StopBits_1;
 USART_InitStructure.USART_Parity = USART_Parity_No;
 USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
 USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 USART_Init(GPS_USART, &USART_InitStructure);
 /* Enable USART */
 USART_Cmd(GPS_USART, ENABLE);
 /* Enable UART3 DMA */
 
 GPS_DMA_CLK_Cmd(GPS_DMA_CLK, ENABLE);
 USART_DMACmd(GPS_USART, USART_DMAReq_Tx, ENABLE);
 /* DMA1 Stream3 Channe4 for UART3 Tx configuration */
 DMA_InitTypeDef 		DMA_InitStructure;
 DMA_InitStructure.DMA_Channel = GPS_DMA_TX_Channel;
 DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&GPS_USART->DR;
 DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)txBufferGPS;
 DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
 DMA_InitStructure.DMA_BufferSize = TXBUFFERSIZEGPS;										// khong quan trong vi khi GPS thi ta dung NDTR de xac dinh byte GPS roi
 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		// khong tang dia chi ngoai vi
 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 						// tang dia chi bo nho de GPS uart
 DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
 DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
 DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 											
 
 DMA_InitStructure.DMA_Priority = DMA_Priority_High;
 DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
 DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
 DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
 DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
 DMA_Init(GPS_DMA_TX_Stream, &DMA_InitStructure);
// DMA_Cmd(GPS_DMA_TX_Stream, ENABLE);
 
 /* DMA1 Stream1 Channe4 for UART3 Rx configuration */
 USART_DMACmd(GPS_USART,USART_DMAReq_Rx,ENABLE);
 DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&GPS_USART->DR;
 DMA_InitStructure.DMA_Memory0BaseAddr 			= (uint32_t)rxBufferGPS;
 DMA_InitStructure.DMA_DIR 									= DMA_DIR_PeripheralToMemory;
 DMA_InitStructure.DMA_BufferSize 					= RXBUFFERSIZEGPS;
 DMA_InitStructure.DMA_PeripheralInc 				= DMA_PeripheralInc_Disable;
 DMA_InitStructure.DMA_MemoryInc 						= DMA_MemoryInc_Enable;
 DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;
 DMA_InitStructure.DMA_MemoryDataSize 			= DMA_MemoryDataSize_Byte;
 DMA_InitStructure.DMA_Mode 								= DMA_Mode_Normal;
 
 DMA_InitStructure.DMA_Priority 						= DMA_Priority_High;
 DMA_InitStructure.DMA_FIFOMode 						= DMA_FIFOMode_Disable;
 DMA_InitStructure.DMA_FIFOThreshold 				= DMA_FIFOThreshold_HalfFull;
 DMA_InitStructure.DMA_MemoryBurst 					= DMA_MemoryBurst_Single;
 DMA_InitStructure.DMA_PeripheralBurst 			= DMA_PeripheralBurst_Single;
 DMA_Init(GPS_DMA_Stream, &DMA_InitStructure);
 DMA_Cmd(GPS_DMA_Stream, ENABLE);

 
 //	//Interrupt
	NVIC_InitTypeDef															NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = GPS_DMA_IRQPreemptionPriority;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority	  		= GPS_DMA_IRQSubPriority;
	NVIC_InitStruct.NVIC_IRQChannel 									= GPS_DMA_Stream_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd								= ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority	  		= 2;
	NVIC_InitStruct.NVIC_IRQChannel 									= DMA1_Stream3_IRQn;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_DMACmd(GPS_USART, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);	
	DMA_ITConfig(GPS_DMA_Stream, DMA_IT_TC, ENABLE);
//	DMA_ITConfig(DMA1_Stream3, DMA_IT_TC, ENABLE);

//	gpsConfig();
}
/**
  * @brief  Write a character onto uart line 
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void UART_WriteChar (uint8_t ch)
{
	USART_SendData(GPS_USART, ch);
	while (USART_GetFlagStatus(GPS_USART, USART_FLAG_TXE) == RESET);
	
}
/**
  * @brief  Write a string onto uart line
  * @param  None
  * @retval None
  */
void UART_Transmit (uint8_t *str, uint8_t dataLength)
{
	uint8_t cnt = 0;
	while (cnt != dataLength)
		{
			while (USART_GetFlagStatus(GPS_USART, USART_FLAG_TXE) == RESET);
			USART_SendData(GPS_USART, *str);
			str++;
			cnt++;
		}
	
}

/**
  * @brief  Check_Data
  * @param  None
  * @retval None
  */

void checkDataIn (uint8_t *rxBufferGPS)
{
//	uint8_t temp = 0;
//	if ((rxBufferGPS[0]	== '$')
//	 && (rxBufferGPS[3]	== 'R')
//	 && (rxBufferGPS[4]	== 'M')
//	 && (rxBufferGPS[5]	== 'C')
//	 && (rxBufferGPS[17]	== 'A')
//	 && (rxBufferGPS[30]	== 'N')
//	 && (rxBufferGPS[44]	== 'E'))
	if ((rxBufferGPS[0]	== '$')
	 && (rxBufferGPS[3]	== 'G')
	 && (rxBufferGPS[4]	== 'L')
	 && (rxBufferGPS[5]	== 'L')
	 && (rxBufferGPS[18]	== 'N')
	 && (rxBufferGPS[32]	== 'E')
	 && (rxBufferGPS[44]	== 'A'))
	{
//		GPRMC_analysis(rxBufferGPS, GPSMsgParam, &rawLtnInfo, &methodParam);
		GPGLL_analysis(rxBufferGPS, GPSMsgParam, &rawLtnInfo);
		sendGPSDataToGUI();
		DMA_Cmd(GPS_DMA_Stream, ENABLE);
		checkSpareDataAfterEnDMARx (rxBufferGPS);
		
	}
	else
	{	
		DMA_Cmd(GPS_DMA_Stream, ENABLE);
		checkSpareDataAfterEnDMARx (rxBufferGPS);
//		GPIO_ToggleBits(GPIOD, GPIO_LED_GREEN);
	}
}

/**
  * @brief  Change position to receive data from DMA
  * @param  None
  * @retval None
  */

void reEnableDMARx (uint8_t *rxBufferGPS)
{
	memset(rxBufferGPS, 0x00, RXBUFFERSIZEGPS);
	DMA_Cmd(GPS_DMA_Stream, DISABLE);
	DMA_ClearITPendingBit(GPS_DMA_Stream, DMA_IT_TCIF1);
	DMA_MemoryTargetConfig(GPS_DMA_Stream, (uint32_t)rxBufferGPS, (uint32_t)&GPS_USART->DR);
	DMA_Cmd(GPS_DMA_Stream, ENABLE);
	
}

/**
  * @brief  Check data after re-enable DMA Rx
  * @param  None
  * @retval None
  */

void checkSpareDataAfterEnDMARx (uint8_t *rxBufferGPS)
{
	delay_ms(6);
	if (rxBufferGPS[0] != '$')
		reEnableDMARx(rxBufferGPS);
}


/**
  * @brief  Interrupt handler for UART DMA
  * @param  None
  * @retval None
  */

//RX
void GPS_DMA_Stream_IRQHandler (void)
{
	if (DMA_GetITStatus(GPS_DMA_Stream, DMA_IT_TCIF1) == SET)
	{
		DMA_ClearITPendingBit(GPS_DMA_Stream, DMA_IT_TCIF1);		
		checkDataIn (rxBufferGPS);
//		GPIO_ToggleBits(GPIOD, GPIO_LED_BLUE);
		
	}
}

/**
  * @brief  GPRMC analysis
  * @param  None
  * @retval None
  */
void GPRMC_analysis(uint8_t *rawData, AnalysisParam GPSMsgParam, Co_ordinates	*rawLtnInfo, MethodParam *methodParam)
{
//	uint8_t str[] = "$GPRMC,050646.60,A,1047.94711,N,10644.59942,E,0.025,0.00,030422,,,A*65";
	
	if (methodParam->cntMsg < 0x02)
		methodParam->cntMsg++;
	else;
	
	//Lat, Lon
	rawLtnInfo->degLat = (uint16_t)(rawData[19] - 0x30)*10 + (uint16_t)(rawData[20] - 0x30);
	
	rawLtnInfo->degLon = (uint16_t)(rawData[32] - 0x30)*100 + (uint16_t)(rawData[33] - 0x30)*10 + (uint16_t)(rawData[34] - 0x30);
	
	GPSMsgParam.temp = (double)((rawData[24] - 0x30)*10000 
														+ (rawData[25] - 0x30)*1000 
														+ (rawData[26] - 0x30)*100 
														+ (rawData[27] - 0x30)*10 
														+ (rawData[28] - 0x30))/100000;
										 
	rawLtnInfo->minLat = (double)(rawData[21] - 0x30)*10 + (double)(rawData[22] - 0x30) + GPSMsgParam.temp;
										 
	GPSMsgParam.temp = (double)((rawData[38] - 0x30)*10000 
														+ (rawData[39] - 0x30)*1000 
														+ (rawData[40] - 0x30)*100 
														+ (rawData[41] - 0x30)*10 
														+ (rawData[42] - 0x30))/100000;
										 
	rawLtnInfo->minLon =(double)(rawData[35] - 0x30)*10 + (double)(rawData[36] - 0x30) + GPSMsgParam.temp;									 
	
	//Vel
	for (int i = 0; i < 8; i++)
	{
		if(rawData[i + 46] != '.')
			GPSMsgParam.cnt++;
		else
			break;
	}
	
	if (GPSMsgParam.cnt == 0x01)
	{
		GPSMsgParam.temp = (double)((rawData[46] - 0x30)*1000
															+ (rawData[48] - 0x30)*100
															+ (rawData[49] - 0x30)*10
															+ (rawData[50] - 0x30))/1000;
	}
	else if (GPSMsgParam.cnt == 0x02)
	{
		GPSMsgParam.temp = (double)((rawData[46] - 0x30)*10000
															+ (rawData[47] - 0x30)*1000
															+ (rawData[49] - 0x30)*100
															+ (rawData[50] - 0x30)*10
															+ (rawData[51] - 0x30))/1000;
	}
	
	rawLtnInfo->sog = GPSMsgParam.temp;

}

void GPGLL_analysis(uint8_t *rawData, AnalysisParam GPSMsgParam, Co_ordinates	*rawLtnInfo)
{
	for (int i = 0; i < 10; i++)
	{
		rawLtnInfo->Lat[i] = rawData[i + 7];
	}
	
	for (int i = 0; i < 11; i++)
	{
		rawLtnInfo->Lon[i] = rawData[i + 20];
	}
	
	if(rawLtnInfo->Lon[0] == '1')
	{
		GPIO_SetBits(GPIOD, GPIO_LED_BLUE);
	}
	
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
