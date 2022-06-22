/* Includes ------------------------------------------------------------------*/
#include "access_GUI.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

			#define 	GUI_TX_PIN  				   	GPIO_Pin_6			
			#define 	GUI_TX_PORT 				  	GPIOB
			#define 	GUI_TX_CLK							RCC_AHB1Periph_GPIOB		
			#define 	GUI_TX_SOURCE				 		GPIO_PinSource6
			#define		GUI_TX_CLK_Cmd		  	 	RCC_AHB1PeriphClockCmd			

			#define 	GUI_RX_PIN  				   	GPIO_Pin_7
			#define 	GUI_RX_PORT 				  	GPIOB
			#define 	GUI_RX_CLK							RCC_AHB1Periph_GPIOB
			#define 	GUI_RX_SOURCE				 		GPIO_PinSource7
			#define		GUI_RX_CLK_Cmd		  	 	RCC_AHB1PeriphClockCmd
			
			#define		GUI_USART								USART1
			#define 	GUI_USART_CLK						RCC_APB2Periph_USART1
			#define		GUI_USART_CLK_Cmd		 		RCC_APB2PeriphClockCmd
			#define 	GUI_USART_BAUDRATE			115200 
			#define		GUI_USART_AF						GPIO_AF_USART1
			#define 	GUI_USART_IRQn					USART3_IRQn
			#define 	GUI_USART_IRQPreemptionPriority		0x00
			#define 	GUI_USART_IRQSubPriority					0x00
			
			#define   GUI_DMA_Channel					DMA_Channel_4
			#define   GUI_DMA_Stream					DMA2_Stream2
			#define		GUI_DMA_CLK_Cmd		  		RCC_AHB1PeriphClockCmd
			#define 	GUI_DMA_CLK							RCC_AHB1Periph_DMA2
			#define		GUI_DMA_Stream_IRQn			DMA2_Stream2_IRQn
			#define 	GUI_DMA_IRQPreemptionPriority		0x00
			#define 	GUI_DMA_IRQSubPriority					0x00
			
			#define		GUI_DMA_Stream_IRQHandler			DMA2_Stream2_IRQHandler
			#define		GUI_USART_IRQHandler					USART1_IRQHandler
			#define		GUI_DMA_RX_FLAG								DMA_IT_TCIF2
			
			#define   GUI_DMA_TX_Channel				DMA_Channel_4
			#define   GUI_DMA_TX_Stream					DMA2_Stream7
			#define		GUI_DMA_TX_Stream_IRQn		DMA2_Stream7_IRQn
			#define		GUI_DMA_TX_FLAG						DMA_FLAG_TCIF7
			
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//extern uint8_t txBufferGUI[RXBUFFERSIZEGUI] = {'$',0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x00,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x00,140,0x0D,0x0A};
uint8_t txBufferGUI[TXBUFFERSIZEGUI];
uint8_t rxBufferGUI[RXBUFFERSIZEGUI];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Uart Configuration
  * @param  baudrate
  * @retval None
  */
void UART_GUI_Config (uint32_t baudrate)
{

 /* GPIO Configuration for UART3*/

 GPIO_InitTypeDef 	GPIO_InitStructure;
	
 GUI_TX_CLK_Cmd(GUI_TX_CLK, ENABLE);	
 GPIO_InitStructure.GPIO_Pin = GUI_TX_PIN;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GUI_TX_PORT, &GPIO_InitStructure);
 GPIO_PinAFConfig(GUI_TX_PORT, GUI_TX_SOURCE, GUI_USART_AF);
	
 GUI_RX_CLK_Cmd(GUI_RX_CLK, ENABLE);	
 GPIO_InitStructure.GPIO_Pin = GUI_RX_PIN;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GUI_RX_PORT, &GPIO_InitStructure);
	

 GPIO_PinAFConfig(GUI_RX_PORT, GUI_RX_SOURCE, GUI_USART_AF);
	
 GUI_USART_CLK_Cmd(GUI_USART_CLK, ENABLE);
 USART_InitTypeDef 						USART_InitStructure;
 USART_InitStructure.USART_BaudRate = baudrate;
 USART_InitStructure.USART_WordLength = USART_WordLength_8b;
 USART_InitStructure.USART_StopBits = USART_StopBits_1;
 USART_InitStructure.USART_Parity = USART_Parity_No;
 USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
 USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 USART_Init(GUI_USART, &USART_InitStructure);
 /* Enable USART */
 USART_Cmd(GUI_USART, ENABLE);
 /* Enable UART1 DMA */
 
 GUI_DMA_CLK_Cmd(GUI_DMA_CLK, ENABLE);
 USART_DMACmd(GUI_USART, USART_DMAReq_Tx, ENABLE);
 /* DMA2 Stream3 Channe4 for UART1 Tx configuration */
 DMA_InitTypeDef 		DMA_InitStructure;
 DMA_InitStructure.DMA_Channel = GUI_DMA_TX_Channel;
 DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&GUI_USART->DR;
 DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)txBufferGUI;
 DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
 DMA_InitStructure.DMA_BufferSize = TXBUFFERSIZEGUI;										// khong quan trong vi khi GUI thi ta dung NDTR de xac dinh byte GUI roi
 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		// khong tang dia chi ngoai vi
 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 						// tang dia chi bo nho de GUI uart
 DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
 DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
 DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 											
 
 DMA_InitStructure.DMA_Priority = DMA_Priority_High;
 DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
 DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
 DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
 DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
 DMA_Init(GUI_DMA_TX_Stream, &DMA_InitStructure);
// DMA_Cmd(GUI_DMA_TX_Stream, ENABLE);
 
 /* DMA2 Stream2 Channe4 for UART1 Rx configuration */
 USART_DMACmd(GUI_USART,USART_DMAReq_Rx,ENABLE);
 DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&GUI_USART->DR;
 DMA_InitStructure.DMA_Memory0BaseAddr 			= (uint32_t)rxBufferGUI;
 DMA_InitStructure.DMA_DIR 									= DMA_DIR_PeripheralToMemory;
 DMA_InitStructure.DMA_BufferSize 					= RXBUFFERSIZEGUI;
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
 DMA_Init(GUI_DMA_Stream, &DMA_InitStructure);
 DMA_Cmd(GUI_DMA_Stream, ENABLE);

 
 //	//Interrupt
	NVIC_InitTypeDef															NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = GUI_DMA_IRQPreemptionPriority;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority	  		= GUI_DMA_IRQSubPriority;
	NVIC_InitStruct.NVIC_IRQChannel 									= GUI_DMA_Stream_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd								= ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority	  		= 1;
	NVIC_InitStruct.NVIC_IRQChannel 									= GUI_DMA_TX_Stream_IRQn;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_DMACmd(GUI_USART, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);	
	DMA_ITConfig(GUI_DMA_Stream, DMA_IT_TC, ENABLE);
//	DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);

}



/**
  * @brief  Interrupt handler for UART DMA
  * @param  None
  * @retval None
  */

//RX
void GUI_DMA_Stream_IRQHandler (void)
{
	if (DMA_GetITStatus(GUI_DMA_Stream, GUI_DMA_RX_FLAG) == SET)
	{
		if(rxBufferGUI[0] == 'S')// Stop
		{
			getBookingID++;
			simModuleState = Get;
			disableTIM37();
			disableTIM2();
			PostOrGetRepeat();
		}
		else if(rxBufferGUI[0] == 'R')// Running
		{
			simModuleState = Post;
			disableTIM37();
			disableTIM2();
			PostOrGetRepeat();
		}
		else;
		
//		memset(rxBufferGUI, 0x00, RXBUFFERSIZEGUI);
		
		DMA_Cmd(GUI_DMA_Stream, DISABLE);
		DMA_ClearITPendingBit(GUI_DMA_Stream, GUI_DMA_RX_FLAG);
		DMA_MemoryTargetConfig(GUI_DMA_Stream, (uint32_t)rxBufferGUI, (uint32_t)&GUI_USART->DR);
		DMA_Cmd(GUI_DMA_Stream, ENABLE);
		
	}
	
}


/**
  * @brief  Send Data to GUI
  * @param  None
  * @retval None
  */

void sendGPSDataToGUI (void)
{
	memcpy(&txBufferGUI[0], &rxBufferGPS[0], sizeof(uint8_t)*BUFFERSIZEGPS);
	
	GPIO_ToggleBits(GPIOD, GPIO_LED_ORANGE);
	
	DMA_SetCurrDataCounter(GUI_DMA_TX_Stream, BUFFERSIZEGPS);
	DMA_ClearFlag(GUI_DMA_TX_Stream, GUI_DMA_TX_FLAG);
	DMA_Cmd(GUI_DMA_TX_Stream, ENABLE);
	
}

/**
  * @brief  Send Data to GUI
  * @param  None
  * @retval None
  */

void sendTaskToGUI (void)
{
	memcpy(&txBufferGUI[0], &rxBufferLTE[24], sizeof(uint8_t)*RXBUFFERSIZETASK);
	
//	char test[] = "{\"guest_num\":\"0384370746\",\"lat_a\":\"0000000323\",\"lon_a\":\"00000000685\",\"lat_b\":\"0000000104\",\"lon_b\":\"00000000645\"}";
	
//	memcpy(&txBufferGUI[0], &test[0], sizeof(char)*RXBUFFERSIZETASK);
//	txBufferGUI[112] = 0x0D;
//	txBufferGUI[113] = 0x0A;
	DMA_SetCurrDataCounter(GUI_DMA_TX_Stream, RXBUFFERSIZETASK);
	DMA_ClearFlag(GUI_DMA_TX_Stream, GUI_DMA_TX_FLAG);
	DMA_Cmd(GUI_DMA_TX_Stream, ENABLE);
	
}


