/* Includes ------------------------------------------------------------------*/
#include "LTE_handler.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** 
	* @brief   GPIO define 
*/
	#define USART2_INIT_BAUDRATE							115200
	#define USART2_RCC_GPIO_PORT							RCC_AHB1Periph_GPIOD
	#define USART2_RCC_APBPeriph_UARTx						RCC_APB1Periph_USART2
	#define USART2_UARTx									USART2
	#define USART2_GPIOx									GPIOD
	#define USART2_GPIO_AF									GPIO_AF_USART2
	#define USART2_GPIO_Pin_RX								GPIO_Pin_6
	#define USART2_GPIO_Pin_TX								GPIO_Pin_5
	#define USART2_GPIO_PinSourceRX							GPIO_PinSource6
	#define USART2_GPIO_PinSourceTX							GPIO_PinSource5
/** 
* @brief   DMA define 
*/

			#define 	LTE_TX_PIN  				   	GPIO_Pin_2
			#define 	LTE_TX_PORT 				  	GPIOA
			#define 	LTE_TX_CLK							RCC_AHB1Periph_GPIOA	
			#define 	LTE_TX_SOURCE				 		GPIO_PinSource2			
			#define		LTE_TX_CLK_Cmd		  	 	RCC_AHB1PeriphClockCmd			

			#define 	LTE_RX_PIN  				   	GPIO_Pin_3
			#define 	LTE_RX_PORT 				  	GPIOA
			#define 	LTE_RX_CLK							RCC_AHB1Periph_GPIOA
			#define 	LTE_RX_SOURCE				 		GPIO_PinSource3
			#define		LTE_RX_CLK_Cmd		  	 	RCC_AHB1PeriphClockCmd
			
			#define		LTE_USART								USART2
			#define 	LTE_USART_CLK						RCC_APB1Periph_USART2
			#define		LTE_USART_CLK_Cmd		 		RCC_APB1PeriphClockCmd
			#define 	LTE_USART_BAUDRATE			115200
			#define		LTE_USART_AF						GPIO_AF_USART2
			#define 	LTE_USART_IRQn					USART2_IRQn
			#define 	LTE_USART_IRQPreemptionPriority		0x03
			#define 	LTE_USART_IRQSubPriority					0x04
			
			#define   LTE_DMA_Channel					DMA_Channel_4
			#define   LTE_DMA_Stream					DMA1_Stream5
			#define		LTE_DMA_CLK_Cmd		  		RCC_AHB1PeriphClockCmd
			#define 	LTE_DMA_CLK							RCC_AHB1Periph_DMA1
			#define		LTE_DMA_Stream_IRQn			DMA1_Stream5_IRQn
			#define 	LTE_DMA_IRQPreemptionPriority		0x01
			#define 	LTE_DMA_IRQSubPriority					0x01
			
			#define		LTE_DMA_Stream_IRQHandler			DMA1_Stream5_IRQHandler
			#define		LTE_USART_IRQHandler					USART2_IRQHandler
			#define		LTE_DMA_RX_FLAG								DMA_IT_TCIF5
			
			#define   LTE_DMA_TX_Channel			DMA_Channel_4
			#define   LTE_DMA_TX_Stream				DMA1_Stream6
			#define		LTE_DMA_TX_Stream_IRQn			DMA1_Stream6_IRQn
			#define		LTE_DMA_TX_FLAG						DMA_FLAG_TCIF6
			
			/**TIMER**/
			#define LTE_TIM_IT									TIM7
			#define LTE_TIM_IT_CLK							RCC_APB1Periph_TIM7
			#define LTE_TIM_IT_CLK_Cmd					RCC_APB1PeriphClockCmd
			#define LTE_TIM_IT_IRQn							TIM7_IRQn
			#define LTE_TIM_IT_IRQHandler				TIM7_IRQHandler
			

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t txBufferLTE[];
uint8_t rxBufferLTE[];

char getBookingID = '1';

initSimModule InitSimModule = {false, false};

SimModuleState simModuleState = Get;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Uart Configuration
  * @param  baudrate
  * @retval None
  */
void UART_LTE_Config (uint32_t baudrate)
{
 /* GPIO Configuration for UART2*/

 GPIO_InitTypeDef 	GPIO_InitStructure;
	
 LTE_TX_CLK_Cmd(LTE_TX_CLK, ENABLE);	
 GPIO_InitStructure.GPIO_Pin = LTE_TX_PIN;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
 GPIO_Init(LTE_TX_PORT, &GPIO_InitStructure);
 GPIO_PinAFConfig(LTE_TX_PORT, LTE_TX_SOURCE, LTE_USART_AF);
 
 LTE_RX_CLK_Cmd(LTE_RX_CLK, ENABLE);	
 GPIO_InitStructure.GPIO_Pin = LTE_RX_PIN;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
 GPIO_Init(LTE_RX_PORT, &GPIO_InitStructure);
 GPIO_PinAFConfig(LTE_RX_PORT, LTE_RX_SOURCE, LTE_USART_AF);
	
 LTE_USART_CLK_Cmd(LTE_USART_CLK, ENABLE);
 USART_InitTypeDef 						USART_InitStructure;
 USART_InitStructure.USART_BaudRate = baudrate;
 USART_InitStructure.USART_WordLength = USART_WordLength_8b;
 USART_InitStructure.USART_StopBits = USART_StopBits_1;
 USART_InitStructure.USART_Parity = USART_Parity_No;
 USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
 USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 USART_Init(LTE_USART, &USART_InitStructure);
 /* Enable USART */
 USART_Cmd(LTE_USART, ENABLE);
 /* Enable UART1 DMA */
 
 LTE_DMA_CLK_Cmd(LTE_DMA_CLK, ENABLE);
 USART_DMACmd(LTE_USART, USART_DMAReq_Tx, ENABLE);
 /* DMA2 Stream3 Channe4 for UART1 Tx configuration */
 DMA_InitTypeDef 		DMA_InitStructure;
 DMA_InitStructure.DMA_Channel = LTE_DMA_TX_Channel;
 DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&LTE_USART->DR;
 DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)txBufferLTE;
 DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
 DMA_InitStructure.DMA_BufferSize = TXBUFFERSIZELTE;										// khong quan trong vi khi LTE thi ta dung NDTR de xac dinh byte LTE roi
 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		// khong tang dia chi ngoai vi
 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 						// tang dia chi bo nho de LTE uart
 DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
 DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
 DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 											
 
 DMA_InitStructure.DMA_Priority = DMA_Priority_High;
 DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
 DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
 DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
 DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
 DMA_Init(LTE_DMA_TX_Stream, &DMA_InitStructure);
// DMA_Cmd(LTE_DMA_TX_Stream, ENABLE);
 
 /* DMA2 Stream2 Channe4 for UART1 Rx configuration */
 USART_DMACmd(LTE_USART,USART_DMAReq_Rx,ENABLE);
 DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&LTE_USART->DR;
 DMA_InitStructure.DMA_Memory0BaseAddr 			= (uint32_t)rxBufferLTE;
 DMA_InitStructure.DMA_DIR 									= DMA_DIR_PeripheralToMemory;
 DMA_InitStructure.DMA_BufferSize 					= RXBUFFERSIZELTE;
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
 DMA_Init(LTE_DMA_Stream, &DMA_InitStructure);
 DMA_Cmd(LTE_DMA_Stream, ENABLE);

 
 //	//Interrupt
	NVIC_InitTypeDef															NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = LTE_DMA_IRQPreemptionPriority;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority	  		= LTE_DMA_IRQSubPriority;
	NVIC_InitStruct.NVIC_IRQChannel 									= LTE_DMA_Stream_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd								= ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority	  		= 0;
	NVIC_InitStruct.NVIC_IRQChannel 									= LTE_DMA_TX_Stream_IRQn;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_DMACmd(LTE_USART, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);	
	DMA_ITConfig(LTE_DMA_Stream, DMA_IT_TC, ENABLE);
//	DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);

}

/**
  * @brief  Timer Configuration
  * @param  baudrate
  * @retval None
  */
void TIM7_Config (uint16_t SampleTime)
{
	LTE_TIM_IT_CLK_Cmd(LTE_TIM_IT_CLK, ENABLE);
	
	TIM_TimeBaseInitTypeDef						TimeBaseInitStruct;
	TimeBaseInitStruct.TIM_ClockDivision 		= TIM_CKD_DIV1;
	TimeBaseInitStruct.TIM_Period 					= SampleTime*2 - 1;  //dem SampleTime*2 thi tran va reset dem lai -> SampleTime(ms) la thoi gian dinh thi
	TimeBaseInitStruct.TIM_Prescaler 				= 41999;				//tan so cua prescaler (84MHz/x) - 1 = 41999 -> 2000Hz -> Update Event = (TIM_Period + 1)*(84MHz(TIM_Prescaler + 1))
	TimeBaseInitStruct.TIM_CounterMode 			= TIM_CounterMode_Up;
	TIM_TimeBaseInit(LTE_TIM_IT, &TimeBaseInitStruct);
	
	NVIC_InitTypeDef TIM7_NVICInitStruct;
	TIM7_NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	TIM7_NVICInitStruct.NVIC_IRQChannelSubPriority	  		= 0;
	TIM7_NVICInitStruct.NVIC_IRQChannel 									= LTE_TIM_IT_IRQn;
	TIM7_NVICInitStruct.NVIC_IRQChannelCmd								= ENABLE;
	NVIC_Init(&TIM7_NVICInitStruct);

}
/**
  * @brief  Timer Configuration
  * @param  baudrate
  * @retval None
  */

void TIM2_InitModuleSim_Config(void)
{
	NVIC_InitTypeDef nvic_struct;

	nvic_struct.NVIC_IRQChannel = TIM2_IRQn;
	nvic_struct.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_struct.NVIC_IRQChannelSubPriority = 1;
	nvic_struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_struct);

	TIM_TimeBaseInitTypeDef tim_struct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	tim_struct.TIM_Period = 250000-1;
	tim_struct.TIM_Prescaler = (uint16_t)(SystemCoreClock/2/1000000)-1; //FTIMER= fSYSTEM/[(PSC+1)(Period+1)]
	tim_struct.TIM_ClockDivision = 0;
	tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &tim_struct);
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	
}

/**
  * @brief  Timer Configuration
  * @param  baudrate
  * @retval None
  */

void accessToRxBuffSimTIMCofig (void)
{
	NVIC_InitTypeDef nvic_struct;

	nvic_struct.NVIC_IRQChannel = TIM3_IRQn;
	nvic_struct.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_struct.NVIC_IRQChannelSubPriority = 1;
	nvic_struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_struct);

	TIM_TimeBaseInitTypeDef tim_struct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	tim_struct.TIM_Period = 99;
	tim_struct.TIM_Prescaler = 4199; //FTIMER= fSYSTEM/[(PSC+1)(Period+1)]
	tim_struct.TIM_ClockDivision = TIM_CKD_DIV1;
	tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &tim_struct);
	
}

/**
  * @brief  Interrupt TIM2 InitModuleSim
  * @param  None
  * @retval None
  */

void TIM2_IRQHandler(void) 
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		if ((InitSimModule.InitPB_DONE == true) && (InitSimModule.InitATE0OK == false))
		{
//			reEnableDMARxLTE(rxBufferLTE);
			Init_SimModule();
			checkInitATE0OK();
		}
		
		if (InitSimModule.InitPB_DONE == false)
		{
			checkPB_DONE();
//			reEnableDMARxLTE(rxBufferLTE);
		}
		
		if (InitSimModule.InitATE0OK == true)
		{
			if(rawLtnInfo.Lon[0] == '1')
			{
				disableTIM2();
			
				if(simModuleState == Get)
				{
					getDataFromServer();
					
				}
				else if(simModuleState == Post)
				{
						postDataToServer();
					
				}
				else;
			}
			
		}
		else
		{
			reEnableDMARxLTE(rxBufferLTE);
		}
//		sendTaskToGUI();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		GPIO_ToggleBits(GPIOD, GPIO_LED_RED);		
	}
}

/**
* @brief  Interrupt TIM3: access to rxBufferLTE 10ms each to check /r/n+HTTPACTION:
  * @param  None
  * @retval None
  */

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		if(simModuleState == Get)
		{
			if((rxBufferLTE[8] == '+') && (rxBufferLTE[13] == 'A') && (rxBufferLTE[14] == 'C') && (rxBufferLTE[23] == '2') && (rxBufferLTE[24] == '0') && (rxBufferLTE[25] == '0'))
			{
				disableTIM37();
				GPIO_ToggleBits(GPIOD, GPIO_LED_GREEN);
				HTTPReadData();

			}
		}
		else if (simModuleState == Post)
		{
			if((rxBufferLTE[14] == '+') && (rxBufferLTE[19] == 'A') && (rxBufferLTE[20] == 'C') && (rxBufferLTE[29] == '2') && (rxBufferLTE[30] == '0') && (rxBufferLTE[31] == '0'))
			{
				disableTIM37();
				GPIO_ToggleBits(GPIOD, GPIO_LED_GREEN);
				reEnableDMARxLTE(rxBufferLTE);
				PostOrGetRepeat();
				
			}
			
		}
		else;
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		GPIO_ToggleBits(GPIOD, GPIO_LED_ORANGE);
	}
	
}


/**
  * @brief  Interrupt TIM7
  * @param  None
  * @retval None
  */

void LTE_TIM_IT_IRQHandler(void)
{
	if (TIM_GetITStatus(LTE_TIM_IT, TIM_IT_Update) == SET)
		{
			disableTIM37();
			
			if(simModuleState == Get)
			{
//				HTTPReadData();
			}
			else if(simModuleState == Post)
			{
				GPIO_ToggleBits(GPIOD, GPIO_LED_BLUE);
				reEnableDMARxLTE(rxBufferLTE);
			}
			else;
			
			InitSimModule.InitATE0OK = false;
			PostOrGetRepeat();
			TIM_ClearITPendingBit(LTE_TIM_IT, TIM_IT_Update);
			
		}
}

/**
  * @brief  Interrupt handler for UART DMA
  * @param  None
  * @retval None
  */

//RX
void LTE_DMA_Stream_IRQHandler (void)
{
	if (DMA_GetITStatus(LTE_DMA_Stream, LTE_DMA_RX_FLAG) == SET)
	{
		DMA_ClearITPendingBit(LTE_DMA_Stream, LTE_DMA_RX_FLAG);
		
		if (simModuleState == Get)
		{
			if ((rxBufferLTE[2] == 'O') && (rxBufferLTE[3] == 'K') && (rxBufferLTE[19] == '1') && (rxBufferLTE[20] == '0') && (rxBufferLTE[21] == '4'))
			{
				sendTaskToGUI();
				
				DMA_Cmd(LTE_DMA_Stream, ENABLE);
				reEnableDMARxLTE(rxBufferLTE);
				simModuleState = Donothing;
//				PostOrGetRepeat();
			}
			else
			{
				DMA_Cmd(LTE_DMA_Stream, ENABLE);
				reEnableDMARxLTE(rxBufferLTE);
				PostOrGetRepeat();
			}
		}
		else
		{
			DMA_Cmd(LTE_DMA_Stream, ENABLE);
			reEnableDMARxLTE(rxBufferLTE);
		}
	}
	
}

/**
  * @brief  Write a string onto uart line
  * @param  None
  * @retval None
  */
void UART_LTE_Transmit (char *str)
{
	uint8_t cnt = 0;
	uint8_t len = strlen(str);
	while (cnt < len)
		{
			while (USART_GetFlagStatus(LTE_USART, USART_FLAG_TXE) == RESET);
			USART_SendData(LTE_USART, *str);
			str++;
			cnt++;
		}
	while (USART_GetFlagStatus(LTE_USART, USART_FLAG_TXE) == RESET);
	USART_SendData(LTE_USART, 0x0D);
	while (USART_GetFlagStatus(LTE_USART, USART_FLAG_TXE) == RESET);
	USART_SendData(LTE_USART, 0x0A);
}

/**
  * @brief  Init Sim module
  * @param  None
  * @retval None
  */
void Init_SimModule (void)
{
	char InitSimbf[] = "ATE0";
	UART_LTE_Transmit(InitSimbf);
	
}

/**
  * @brief  checkInitSimModule.InitATE0OK
  * @param  None
  * @retval None
  */

void checkInitATE0OK (void)
{
	delay_ms(1);
	for (int i = 0; i < RXBUFFERSIZELTE; i++)
	{
		if(rxBufferLTE[i] == 'O')
		{
			if(rxBufferLTE[i + 1] == 'K')
			{
				InitSimModule.InitATE0OK = true;
				return;
			}
			else
			{
				InitSimModule.InitATE0OK = false;
				return;
			}
		}
		else if (rxBufferLTE[i] == 0x00)
		{
			InitSimModule.InitATE0OK = false;
			return;
		}
		else;
	}
	InitSimModule.InitATE0OK = false;
	return;
	
}

void checkPB_DONE (void)
{
	for (int i = 0; i < RXBUFFERSIZELTE; i++)
	{
		if(rxBufferLTE[i] == 'P')
		{
			if((rxBufferLTE[i + 1] == 'B') && (rxBufferLTE[i + 3] == 'D'))
			{
				InitSimModule.InitPB_DONE = true;
				return;
			}
			else
			{
				InitSimModule.InitPB_DONE = false;
				return;
			}
		}
		else if (rxBufferLTE[i] == 0x00)
		{
			InitSimModule.InitPB_DONE = false;
			return;
		}
		else;
	}
	InitSimModule.InitPB_DONE = false;
	return;
	
}

/**
  * @brief  reEnableDMARxLTE
  * @param  None
  * @retval None
  */

void reEnableDMARxLTE (uint8_t *rxBufferLTE)
{
	memset(rxBufferLTE, 0x00, RXBUFFERSIZELTE);
	DMA_Cmd(LTE_DMA_Stream, DISABLE);
	DMA_ClearITPendingBit(LTE_DMA_Stream, LTE_DMA_RX_FLAG);
	DMA_MemoryTargetConfig(LTE_DMA_Stream, (uint32_t)rxBufferLTE, (uint32_t)&LTE_USART->DR);
	DMA_Cmd(LTE_DMA_Stream, ENABLE);
	
}

/**
  * @brief  reEnable TIMER2 for GET or Post
  * @param  None
  * @retval None
  */
void PostOrGetRepeat (void)
{
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	
}


/**
  * @brief  getDataFromServer
  * @param  None
  * @retval None
  */
void getDataFromServer (void)
{
	char StopHTTPbf[] = "AT+HTTPTERM";
	UART_LTE_Transmit(StopHTTPbf);
	
	delay_ms(2);
	
	char InitHTTPbf[] = "AT+HTTPINIT";
	UART_LTE_Transmit(InitHTTPbf);
	
	delay_ms(2);
	
	char UrlHTTPbf[] = "AT+HTTPPARA=\"URL\",\"http://fleetmonitoring.000webhostapp.com/getbookedinfo.php?guest_ID=6\"";
	UrlHTTPbf[87] = getBookingID;
	UART_LTE_Transmit(UrlHTTPbf);
	
	delay_ms(2);

	char HTTPActionBf[] = "AT+HTTPACTION=0";
	UART_LTE_Transmit(HTTPActionBf);
	
	waitForHTTPReadTIM();

}

/**
  * @brief  waitForHTTPReadTIM enable TIM37
  * @param  None
  * @retval None
  */

void waitForHTTPReadTIM (void)
{
	TIM_ClearITPendingBit(LTE_TIM_IT, TIM_IT_Update);
	TIM_ITConfig(LTE_TIM_IT, TIM_IT_Update, ENABLE);
	TIM_Cmd(LTE_TIM_IT, ENABLE);
	
	reEnableDMARxLTE(rxBufferLTE);
	
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	
}

/**
  * @brief  Stop TIM for check +HTTPACTION: 
  * @param  None
  * @retval None
  */

void disableTIM37 (void)
{
	TIM_ITConfig(LTE_TIM_IT, TIM_IT_Update, DISABLE);
	TIM_Cmd(LTE_TIM_IT, DISABLE);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
	TIM_Cmd(TIM3, DISABLE);
	
	TIM_SetCounter(LTE_TIM_IT, 0);
	TIM_SetCounter(TIM3, 0);

}

/**
  * @brief  Stop TIM for check ATE0 OK
  * @param  None
  * @retval None
  */

void disableTIM2 (void)
{
	reEnableDMARxLTE(rxBufferLTE);
	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
	TIM_Cmd(TIM2, DISABLE);
	
	TIM_SetCounter(TIM2, 0);
	
}


/**
  * @brief  HTTPReadData
  * @param  None
  * @retval None
  */

void HTTPReadData (void)
{
	reEnableDMARxLTE(rxBufferLTE);
//	char HTTPReadBf[] = "AT+HTTPREAD=0,112";
	char HTTPReadBf[] = "AT+HTTPREAD=0,104";
	UART_LTE_Transmit(HTTPReadBf);

}


/*************Post Data************/
/**
  * @brief  HTTPReadData
  * @param  None
  * @retval None
  */
void postDataToServer (void)
{
	char StopHTTPbf[] = "AT+HTTPTERM";
	UART_LTE_Transmit(StopHTTPbf);
	
	delay_ms(2);
	
	char InitHTTPbf[] = "AT+HTTPINIT";
	UART_LTE_Transmit(InitHTTPbf);
	
	delay_ms(2);
	
	char ContentHTTPbf[] = "AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"";
	UART_LTE_Transmit(ContentHTTPbf);
	
	delay_ms(2);
	
	char UrlHTTPbf[] = "AT+HTTPPARA=\"URL\",\"https://fleetmonitoring.000webhostapp.com/createGPSdata.php\"";
	UART_LTE_Transmit(UrlHTTPbf);
	
	delay_ms(2);
	
	char HTTPDataBf[] = "AT+HTTPDATA=67,1000";
	UART_LTE_Transmit(HTTPDataBf);
	
	delay_ms(2);
	
	char HTTPMsgBf[] = "car_gps_ID=2&longtitude=01234.56789&latitude=0123.45678&velocity=00";

	if(rawLtnInfo.Lon[0] == '1')
	{
		memcpy(&HTTPMsgBf[24], &rawLtnInfo.Lon[0], sizeof(uint8_t)*11);
		memcpy(&HTTPMsgBf[45], &rawLtnInfo.Lat[0], sizeof(uint8_t)*10);
		
	}
	UART_LTE_Transmit(HTTPMsgBf);
	
	delay_ms(2);

	char HTTPActionBf[] = "AT+HTTPACTION=1";
	UART_LTE_Transmit(HTTPActionBf);
	
	waitForHTTPReadTIM();
	
}
