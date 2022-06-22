/******************************************************************************
* Project        : BswProject
* Component      : SysMode   
* FileName       : SysMode_1.c
* Company        : HCM Universyty of Technology. 
* Author         : Huy Le
* Description    : 
* Limitations    : 
******************************************************************************/

/*------------------------------------------------------------------------------------------------------------
 Include-Files
 ------------------------------------------------------------------------------------------------------------*/
#include "uart_gps.h"
/*------------------------------------------------------------------------------------------------------------
 Private Definitions
 ------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------
 Private Structs
 ------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------
 Private Variables
 ------------------------------------------------------------------------------------------------------------*/
static uart_gps_transmit_state_en uart_gps_transmission_state = uart_gps_transmit_noinit;
static uart_gps_receive_state_en uart_gps_receive_state = uart_gps_receive_noinit;
static u16 uart_gps_tx_timeout_cnt_u16 = 0u;
static u16 uart_gps_rx_timeout_cnt_u16 = 0u;
static uart_gps_state_en uart_gps_state;
static u16 uart_gps_transmision_error_cnt = 0u;
static u16 uart_gps_reception_error_cnt = 0u;
static boolean_t uart_gps_NewRxMsg_bl = FALSE;

/*------------------------------------------------------------------------------------------------------------
 Private Constants
 ------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------
 Public Variables
 ------------------------------------------------------------------------------------------------------------*/
u8 GPS_RxBuff[USART1_GPS_BUFF_SIZE] = {0xFFu};

/*-----------------------------------------------------------------------------------------------------------
 Private Function Declarations
 ------------------------------------------------------------------------------------------------------------*/
static void uart_gps_Cfg_Init(u32 Baudrate);
static void uart_gps_Cfg_RxDMA_Init(void);
static void uart_gps_Cfg_TxDMA_Init(void);
static void uart_gps_Cfg_RxDMA_SetExpectedReceived(u32 RxBuff_Addr_u32, u32 Length);
static void uart_gps_Cfg_TransmisionErrorCallout(void);
static void uart_gps_Cfg_ReceptionErrorCallout(void);
static void uart_gps_SetTxStatus(uart_gps_transmit_state_en state);
static void uart_gps_SetRxStatus(uart_gps_receive_state_en state);

/*-----------------------------------------------------------------------------------------------------------
 Private\Public Function Definitions
 ------------------------------------------------------------------------------------------------------------*/

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
void uart_gps_Init(void)
{
	/* Init global and local variable */

	/* Call init */
	uart_gps_Cfg_Init(USART1_INIT_BAUDRATE);
	
	uart_gps_Cfg_TxDMA_Init();
	uart_gps_Cfg_RxDMA_Init();

	/*Init variable*/
	uart_gps_Start();
	uart_gps_transmission_state = uart_gps_transmit_idle;
	uart_gps_receive_state = uart_gps_receive_idle;
	uart_gps_tx_timeout_cnt_u16 = USART1_TX_WAIT_CNT;
	uart_gps_rx_timeout_cnt_u16 = USART1_RX_WAIT_CNT;
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
void uart_gps_Main(void)
{
	if (uart_gps_started == uart_gps_state)
	{
		/* Check if transmition time out */
		if(uart_gps_transmit_transmitting ==  uart_gps_transmission_state)
		{
			if (uart_gps_tx_timeout_cnt_u16 > 0u)
			{
				uart_gps_tx_timeout_cnt_u16--;
			}
			else
			{
				uart_gps_tx_timeout_cnt_u16 = USART1_TX_WAIT_CNT;
				uart_gps_Cfg_TransmisionErrorCallout();
				uart_gps_transmission_state = uart_gps_transmit_idle;
			}
		}
		else
		{
			uart_gps_tx_timeout_cnt_u16 = USART1_TX_WAIT_CNT;
		}
		
		/* Check if reception time out */
		if (uart_gps_receive_receiving == uart_gps_receive_state)
		{
			if (uart_gps_rx_timeout_cnt_u16 > 0u)
			{
				uart_gps_rx_timeout_cnt_u16--;
			}
			else
			{
				uart_gps_rx_timeout_cnt_u16 = USART1_RX_WAIT_CNT;
				uart_gps_Cfg_ReceptionErrorCallout();
				uart_gps_receive_state = uart_gps_receive_idle;
			}
		}
		else
		{
			uart_gps_rx_timeout_cnt_u16 = USART1_RX_WAIT_CNT;
		}
		
		/* Check if transmition has error */
		if(uart_gps_transmit_error == uart_gps_transmission_state)
		{
			uart_gps_Cfg_TransmisionErrorCallout();
			uart_gps_transmission_state = uart_gps_transmit_idle;
		}

		/* Check if reception has error */
		if((uart_gps_receive_error == uart_gps_receive_state))
		{
			uart_gps_Cfg_ReceptionErrorCallout();
			uart_gps_receive_state = uart_gps_receive_idle;
		}
	}
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
void uart_gps_Start(void)
{
	/* Set state to started */
	uart_gps_state = uart_gps_started;
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
void uart_gps_Stop(void)
{
	/* Set state to stopped */
	uart_gps_state = uart_gps_stopped;

	DMA_Cmd(USART1_Tx_DMA_Stream, DISABLE);
    DMA_ITConfig(USART1_Tx_DMA_Stream, DMA_IT_TC, DISABLE);
	DMA_ClearITPendingBit(USART1_Tx_DMA_Stream, DMA_IT_TCIF7);
	DMA_SetCurrDataCounter(USART1_Tx_DMA_Stream, 0xFFFFu);

	DMA_Cmd(USART1_Rx_DMA_Stream, DISABLE);
    DMA_ITConfig(USART1_Rx_DMA_Stream, DMA_IT_TC, DISABLE);
	DMA_ClearITPendingBit(USART1_Rx_DMA_Stream, DMA_IT_TCIF5);
	DMA_SetCurrDataCounter(USART1_Rx_DMA_Stream, 0xFFFFu);
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
static void uart_gps_Cfg_TransmisionErrorCallout(void)
{
	DMA_Cmd(USART1_Tx_DMA_Stream, DISABLE);
    DMA_ITConfig(USART1_Tx_DMA_Stream, DMA_IT_TC, DISABLE);
	DMA_ClearITPendingBit(USART1_Tx_DMA_Stream, DMA_IT_TCIF7);
	DMA_SetCurrDataCounter(USART1_Tx_DMA_Stream, 0xFFFFu);
	uart_gps_transmission_state = uart_gps_transmit_idle;

    if (uart_gps_transmision_error_cnt < 65535u)
    {
        uart_gps_transmision_error_cnt++;
    }
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
static void uart_gps_Cfg_ReceptionErrorCallout(void)
{
	DMA_Cmd(USART1_Rx_DMA_Stream, DISABLE);
    DMA_ITConfig(USART1_Rx_DMA_Stream, DMA_IT_TC, DISABLE);
	DMA_ClearITPendingBit(USART1_Rx_DMA_Stream, DMA_IT_TCIF5);
	DMA_SetCurrDataCounter(USART1_Rx_DMA_Stream, 0xFFFFu);
	uart_gps_NewRxMsg_bl = FALSE;
	uart_gps_receive_state = uart_gps_receive_idle;

    if (uart_gps_reception_error_cnt < 65535u)
    {
        uart_gps_reception_error_cnt++;
    }
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
static void uart_gps_Cfg_Init(u32 Baudrate)
{	
    USART_InitTypeDef 	USART_InitStructure;
    GPIO_InitTypeDef 	GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(USART1_RCC_APBPeriph_UARTx, ENABLE);

    RCC_AHB1PeriphClockCmd(USART1_RCC_GPIO_PORT, ENABLE);
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin 	= USART1_GPIO_Pin_TX | USART1_GPIO_Pin_RX;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(USART1_GPIOx, &GPIO_InitStructure);
    GPIO_PinAFConfig(USART1_GPIOx, USART1_GPIO_PinSourceTX, USART1_GPIO_AF); 
    GPIO_PinAFConfig(USART1_GPIOx, USART1_GPIO_PinSourceRX, USART1_GPIO_AF); 

    USART_InitStructure.USART_BaudRate 						= Baudrate;
    USART_InitStructure.USART_WordLength 					= USART_WordLength_8b;
    USART_InitStructure.USART_StopBits 						= USART_StopBits_1;
    USART_InitStructure.USART_Parity 						= USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl           = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode 							= USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1_UARTx, &USART_InitStructure);

    USART_Cmd(USART1_UARTx, ENABLE); 
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
static void uart_gps_Cfg_RxDMA_Init(void)
{
    DMA_InitTypeDef   DMA_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;	

    RCC_AHB1PeriphClockCmd(USART1_Rx_DMAx, ENABLE);
    USART_ITConfig(USART1_UARTx, USART_IT_RXNE, DISABLE);
    USART_DMACmd(USART1_UARTx, USART_DMAReq_Rx, ENABLE);

    /* DMA1 Stream2 Channel4 for USART1 Rx configuration */			
    DMA_InitStructure.DMA_Channel = USART1_Rx_DMA_Channel;  
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1_UARTx->DR;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(GPS_RxBuff);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = 0u;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(USART1_Rx_DMA_Stream, &DMA_InitStructure);
    DMA_Cmd(USART1_Rx_DMA_Stream, DISABLE);

    /* Enable DMA Interrupt to the highest priority */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_Rx_DMA_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Transfer complete interrupt mask */
    DMA_ITConfig(USART1_Rx_DMA_Stream, DMA_IT_TC, DISABLE);

}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
void DMA2_Stream5_IRQHandler(void)
{
	DMA_Cmd(USART1_Rx_DMA_Stream, DISABLE);
	DMA_ITConfig(USART1_Rx_DMA_Stream, DMA_IT_TC, DISABLE);

	/* Clear the DMA1_Stream5 TCIF5 pending bit */
	DMA_ClearITPendingBit(USART1_Rx_DMA_Stream, DMA_IT_TCIF5);
	uart_gps_SetRxStatus(uart_gps_receive_finished);
	uart_gps_NewRxMsg_bl = TRUE;
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
static void uart_gps_Cfg_RxDMA_SetExpectedReceived(u32 RxBuff_Addr_u32, u32 Length)
{
	uart_gps_NewRxMsg_bl = FALSE;

    DMA_SetCurrDataCounter(USART1_Rx_DMA_Stream, Length);
    DMA_MemoryTargetConfig(USART1_Rx_DMA_Stream, RxBuff_Addr_u32, DMA_Memory_0);

    DMA_Cmd(USART1_Rx_DMA_Stream, ENABLE);
    DMA_ITConfig(USART1_Rx_DMA_Stream, DMA_IT_TC, ENABLE);
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
static void uart_gps_Cfg_TxDMA_Init(void)
{
    DMA_InitTypeDef   DMA_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;	

    RCC_AHB1PeriphClockCmd(USART1_Tx_DMAx, ENABLE);

    USART_ITConfig(USART1_UARTx, USART_IT_TXE, DISABLE);

    USART_DMACmd(USART1_UARTx, USART_DMAReq_Tx, ENABLE);

    /* DMA1 Stream6 Channel4 for USART1 Tx configuration */			
    DMA_InitStructure.DMA_Channel = USART1_Tx_DMA_Channel;  
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1_UARTx->DR;
    DMA_InitStructure.DMA_Memory0BaseAddr = NULL;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = 0u;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(USART1_Tx_DMA_Stream, &DMA_InitStructure);
    DMA_Cmd(USART1_Tx_DMA_Stream, DISABLE); /* Should enable when start send Tx */

    /* Enable DMA Interrupt to the highest priority */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_Tx_DMA_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Transfer complete interrupt mask */
    DMA_ITConfig(USART1_Tx_DMA_Stream, DMA_IT_TC, DISABLE); /* Should enable when start send Tx via DMA*/
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
void DMA2_Stream7_IRQHandler(void)
{
	DMA_Cmd(USART1_Tx_DMA_Stream, DISABLE);
    DMA_ITConfig(USART1_Tx_DMA_Stream, DMA_IT_TC, DISABLE);

    /* Clear the DMA1_Stream6 TCIF6 pending bit */
    DMA_ClearITPendingBit(USART1_Tx_DMA_Stream, DMA_IT_TCIF7);

    uart_gps_SetTxStatus(uart_gps_transmit_finished);
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
uart_gps_state_en uart_gps_SendString(char String[], u8 Length)
{
	/* local vars */
	uart_gps_state_en return_state = uart_gps_ok;

	if ((uart_gps_state == uart_gps_started) && (uart_gps_transmission_state == uart_gps_transmit_idle))
	{
		uart_gps_transmission_state = uart_gps_transmit_transmitting;
		uart_gps_tx_timeout_cnt_u16 = USART1_TX_WAIT_CNT;

		/* Send data over uart_gps */
		DMA_SetCurrDataCounter(USART1_Tx_DMA_Stream, Length);
		DMA_MemoryTargetConfig(USART1_Tx_DMA_Stream, (u32)&String[0], DMA_Memory_0);

		DMA_Cmd(USART1_Tx_DMA_Stream, ENABLE);
		DMA_ITConfig(USART1_Tx_DMA_Stream, DMA_IT_TC, ENABLE);
	}
	else
	{
		return_state = uart_gps_error;
		if (uart_gps_state != uart_gps_started)
		{
			uart_gps_transmission_state = uart_gps_transmit_error;
		}
		else
		{
			/* Do nothing */
		}
	}

	return return_state;
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
uart_gps_state_en uart_gps_SendFrame(u8 * Frame, u8 Length)
{
	/* local vars */
	uart_gps_state_en return_state = uart_gps_ok;

	if ((uart_gps_state == uart_gps_started) && (uart_gps_transmission_state == uart_gps_transmit_idle))
	{
		uart_gps_transmission_state = uart_gps_transmit_transmitting;
		uart_gps_tx_timeout_cnt_u16 = USART1_TX_WAIT_CNT;

		/* Send data over uart_gps */
		DMA_SetCurrDataCounter(USART1_Tx_DMA_Stream, Length);
		DMA_MemoryTargetConfig(USART1_Tx_DMA_Stream, (u32)&Frame[0], DMA_Memory_0);

		DMA_Cmd(USART1_Tx_DMA_Stream, ENABLE);
		DMA_ITConfig(USART1_Tx_DMA_Stream, DMA_IT_TC, ENABLE);
	}
	else
	{
		return_state = uart_gps_error;
		if (uart_gps_state != uart_gps_started)
		{
			uart_gps_transmission_state = uart_gps_transmit_error;
		}
		else
		{
			/* Do nothing */
		}
	}

	return return_state;
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
uart_gps_state_en uart_gps_RequestFrame(u8 * Rx_Buff_pu8, u8 Length)
{
	/* local vars */
	uart_gps_state_en return_state = uart_gps_ok;

	if ((uart_gps_state == uart_gps_started) && (uart_gps_receive_state == uart_gps_receive_idle))
	{
		uart_gps_receive_state = uart_gps_receive_receiving;
		uart_gps_rx_timeout_cnt_u16 = USART1_RX_WAIT_CNT;
		/* Send data over uart_gps */
		uart_gps_Cfg_RxDMA_SetExpectedReceived((u32)Rx_Buff_pu8, Length);
	}
	else
	{
		return_state = uart_gps_error;
		if (uart_gps_state != uart_gps_started)
		{
			uart_gps_receive_state = uart_gps_receive_error;
		}
		else
		{
			/* Do nothing */
		}
	}
	return return_state;
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
uart_gps_transmit_state_en uart_gps_GetTxStatus(void)
{
	return uart_gps_transmission_state;
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
void uart_gps_SetTxStatus(uart_gps_transmit_state_en state)
{
	uart_gps_transmission_state = state;
	if (uart_gps_transmit_finished == state)
	{
		uart_gps_transmission_state = uart_gps_transmit_idle;
	}
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
uart_gps_receive_state_en uart_gps_GetRxStatus(void)
{
	return uart_gps_receive_state;
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
void uart_gps_SetRxStatus(uart_gps_receive_state_en state)
{
	uart_gps_receive_state = state;
	if (uart_gps_receive_finished == state)
	{
		uart_gps_receive_state = uart_gps_receive_idle;
	}
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
boolean_t uart_gps_IsNewMsg(void)
{
	return uart_gps_NewRxMsg_bl;
}
