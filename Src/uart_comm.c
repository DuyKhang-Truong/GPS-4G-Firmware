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
#include "uart_comm.h"
/*------------------------------------------------------------------------------------------------------------
 Private Definitions
 ------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------
 Private Structs
 ------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------
 Private Variables
 ------------------------------------------------------------------------------------------------------------*/
static uart_comm_transmit_state_en uart_comm_transmission_state = uart_comm_transmit_noinit;
static uart_comm_receive_state_en uart_comm_receive_state = uart_comm_receive_noinit;
static u16 uart_comm_tx_timeout_cnt_u16 = 0u;
static u16 uart_comm_rx_timeout_cnt_u16 = 0u;
static uart_comm_state_en uart_comm_state;
u16 uart_comm_transmision_error_cnt = 0u;
u16 uart_comm_reception_error_cnt = 0u;
u8 uart_comm_CurrentReceived_u16 = 0u;
u8 uart_comm_RxBuffTemp[USART2_Rxbuff_Size];
u8 a_buff[];
boolean_t uart_comm_NewRxMsg_bl = FALSE;

/*------------------------------------------------------------------------------------------------------------
 Private Constants
 ------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------
 Public Variables
 ------------------------------------------------------------------------------------------------------------*/
Comm_VehicleControlData_t Comm_VehicleControlData;
Comm_LongLatData_un Comm_LongitudeData;
Comm_LongLatData_un Comm_LatitudeData;

/*-----------------------------------------------------------------------------------------------------------
 Private Function Declarations
 ------------------------------------------------------------------------------------------------------------*/
static void uart_comm_cfg_Init(u32 Baudrate);
static void uart_comm_RxDMA_Init(void);
static void uart_comm_TxDMA_Init(void);
static void uart_comm_TransmisionErrorCallout(void);
static void uart_comm_ReceptionErrorCallout(void);


/*-----------------------------------------------------------------------------------------------------------
 Private\Public Function Definitions
 ------------------------------------------------------------------------------------------------------------*/

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
void uart_comm_Init(void)
{
	/* Init global and local variable */

	/* Call init */
	uart_comm_cfg_Init(USART2_INIT_BAUDRATE);
	uart_comm_TxDMA_Init();
	uart_comm_RxDMA_Init();

	/*Init variable*/
	uart_comm_Start();
	uart_comm_transmission_state = uart_comm_transmit_idle;
	uart_comm_receive_state = uart_comm_receive_idle;
	uart_comm_tx_timeout_cnt_u16 = USART2_TX_WAIT_CNT;
	uart_comm_rx_timeout_cnt_u16 = USART2_RX_WAIT_CNT;

	Comm_VehicleControlData.BrakePercetage = 100u;
	Comm_LongitudeData.Value_un.Value = 0.0;
	Comm_LatitudeData.Value_un.Value = 0.0;
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
void uart_comm_Main(void)
{
	if (uart_comm_started == uart_comm_state)
	{
		/* Check if transmition time out */
		if(uart_comm_transmit_transmitting ==  uart_comm_transmission_state)
		{
			if (uart_comm_tx_timeout_cnt_u16 > 0u)
			{
				uart_comm_tx_timeout_cnt_u16--;
			}
			else
			{
				uart_comm_tx_timeout_cnt_u16 = USART2_TX_WAIT_CNT;
				uart_comm_TransmisionErrorCallout();
				uart_comm_transmission_state = uart_comm_transmit_idle;
			}
		}
		else
		{
			uart_comm_tx_timeout_cnt_u16 = USART2_TX_WAIT_CNT;
		}
		
		/* Check if reception time out */
		if (uart_comm_receive_receiving == uart_comm_receive_state)
		{
			if (uart_comm_rx_timeout_cnt_u16 > 0u)
			{
				uart_comm_rx_timeout_cnt_u16--;
			}
			else
			{
				uart_comm_rx_timeout_cnt_u16 = USART2_RX_WAIT_CNT;
				uart_comm_ReceptionErrorCallout();
				uart_comm_receive_state = uart_comm_receive_idle;
			}
		}
		else
		{
			uart_comm_rx_timeout_cnt_u16 = USART2_RX_WAIT_CNT;
		}
		
		/* Check if transmition has error */
		if(uart_comm_transmit_error == uart_comm_transmission_state)
		{
			uart_comm_TransmisionErrorCallout();
			uart_comm_transmission_state = uart_comm_transmit_idle;
		}

		/* Check if reception has error */
		if((uart_comm_receive_error == uart_comm_receive_state))
		{
			uart_comm_ReceptionErrorCallout();
			uart_comm_receive_state = uart_comm_receive_idle;
		}
	}
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
void uart_comm_Start(void)
{
	/* Set state to started */
	uart_comm_state = uart_comm_started;
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
void uart_comm_Stop(void)
{
	/* Set state to stopped */
	uart_comm_state = uart_comm_stopped;

	DMA_Cmd(USART2_Tx_DMA_Stream, DISABLE);
    DMA_ITConfig(USART2_Tx_DMA_Stream, DMA_IT_TC, DISABLE);
	DMA_ClearITPendingBit(USART2_Tx_DMA_Stream, DMA_IT_TCIF6);
	DMA_SetCurrDataCounter(USART2_Tx_DMA_Stream, 0xFFFFu);

	DMA_Cmd(USART2_Rx_DMA_Stream, DISABLE);
    DMA_ITConfig(USART2_Rx_DMA_Stream, DMA_IT_TC, DISABLE);
	DMA_ClearITPendingBit(USART2_Rx_DMA_Stream, DMA_IT_TCIF5);
	DMA_SetCurrDataCounter(USART2_Rx_DMA_Stream, 0xFFFFu);
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
static void uart_comm_TransmisionErrorCallout(void)
{
	DMA_Cmd(USART2_Tx_DMA_Stream, DISABLE);
    DMA_ITConfig(USART2_Tx_DMA_Stream, DMA_IT_TC, DISABLE);
	DMA_ClearITPendingBit(USART2_Tx_DMA_Stream, DMA_IT_TCIF6);
	DMA_SetCurrDataCounter(USART2_Tx_DMA_Stream, 0xFFFFu);
	uart_comm_transmission_state = uart_comm_transmit_idle;

    if (uart_comm_transmision_error_cnt < 65535u)
    {
        uart_comm_transmision_error_cnt++;
    }
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
static void uart_comm_ReceptionErrorCallout(void)
{
	DMA_Cmd(USART2_Rx_DMA_Stream, DISABLE);
	DMA_ITConfig(USART2_Rx_DMA_Stream, DMA_IT_TC, DISABLE);
	DMA_ClearITPendingBit(USART2_Rx_DMA_Stream, DMA_IT_TCIF5);
	DMA_SetCurrDataCounter(USART2_Rx_DMA_Stream, 0xFFFFu);
	uart_comm_NewRxMsg_bl = FALSE;
	uart_comm_receive_state = uart_comm_receive_idle;

    if (uart_comm_reception_error_cnt < 65535u)
    {
        uart_comm_reception_error_cnt++;
    }
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
static void uart_comm_cfg_Init(u32 Baudrate)
{	
    USART_InitTypeDef 	USART_InitStructure;
    GPIO_InitTypeDef 	GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(USART2_RCC_APBPeriph_UARTx, ENABLE);

    RCC_AHB1PeriphClockCmd(USART2_RCC_GPIO_PORT, ENABLE);
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin 	= USART2_GPIO_Pin_TX | USART2_GPIO_Pin_RX;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(USART2_GPIOx, &GPIO_InitStructure);
    GPIO_PinAFConfig(USART2_GPIOx, USART2_GPIO_PinSourceTX, USART2_GPIO_AF); 
    GPIO_PinAFConfig(USART2_GPIOx, USART2_GPIO_PinSourceRX, USART2_GPIO_AF); 

    USART_InitStructure.USART_BaudRate 						= Baudrate;
    USART_InitStructure.USART_WordLength 					= USART_WordLength_8b;
    USART_InitStructure.USART_StopBits 						= USART_StopBits_1;
    USART_InitStructure.USART_Parity 						= USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl           = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode 							= USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2_UARTx, &USART_InitStructure);

    USART_Cmd(USART2_UARTx, ENABLE); 
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
static void uart_comm_RxDMA_Init(void)
{
    DMA_InitTypeDef   DMA_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;	

    RCC_AHB1PeriphClockCmd(USART2_Rx_DMAx, ENABLE);

    USART_ITConfig(USART2_UARTx, USART_IT_RXNE, DISABLE);

    USART_DMACmd(USART2_UARTx, USART_DMAReq_Rx, ENABLE);

    /* DMA1 Stream2 Channel4 for USART2 Rx configuration */			
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2_UARTx->DR;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)uart_comm_RxBuffTemp;
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
    DMA_Init(USART2_Rx_DMA_Stream, &DMA_InitStructure);
    DMA_Cmd(USART2_Rx_DMA_Stream, DISABLE);

    /* Enable DMA Interrupt to the highest priority */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_Rx_DMA_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Transfer complete interrupt mask */
    DMA_ITConfig(USART2_Rx_DMA_Stream, DMA_IT_TC, DISABLE);

}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
void DMA1_Stream5_IRQHandler(void)
{
		DMA_Cmd(USART2_Rx_DMA_Stream, DISABLE);
    DMA_ITConfig(USART2_Rx_DMA_Stream, DMA_IT_TC, DISABLE);

    /* Clear the DMA1_Stream5 TCIF5 pending bit */
    DMA_ClearITPendingBit(USART2_Rx_DMA_Stream, DMA_IT_TCIF5);
    uart_comm_SetRxStatus(uart_comm_receive_finished);
		for (int i=0;i<1024;i++)
		{
			a_buff[i] = uart_comm_RxBuffTemp[i];
		}
		
		memcpy(&txBufferGUI[0], &a_buff[24], sizeof(uint8_t)*Get_GPS_data_Size);
		sendTaskToGUI();
		
    uart_comm_NewRxMsg_bl = TRUE;
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
static void uart_comm_TxDMA_Init(void)
{
    DMA_InitTypeDef   DMA_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;	

    RCC_AHB1PeriphClockCmd(USART2_Tx_DMAx, ENABLE);

    USART_ITConfig(USART2_UARTx, USART_IT_TXE, DISABLE);

    USART_DMACmd(USART2_UARTx, USART_DMAReq_Tx, ENABLE);

    /* DMA1 Stream6 Channel4 for USART2 Tx configuration */			
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2_UARTx->DR;
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
    DMA_Init(USART2_Tx_DMA_Stream, &DMA_InitStructure);
    DMA_Cmd(USART2_Tx_DMA_Stream, DISABLE); /* Should enable when start send Tx */

    /* Enable DMA Interrupt to the highest priority */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_Tx_DMA_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Transfer complete interrupt mask */
    DMA_ITConfig(USART2_Tx_DMA_Stream, DMA_IT_TC, DISABLE); /* Should enable when start send Tx via DMA*/
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
void DMA1_Stream6_IRQHandler(void)
{
		DMA_Cmd(USART2_Tx_DMA_Stream, DISABLE);
    DMA_ITConfig(USART2_Tx_DMA_Stream, DMA_IT_TC, DISABLE);

    /* Clear the DMA1_Stream6 TCIF6 pending bit */
    DMA_ClearITPendingBit(USART2_Tx_DMA_Stream, DMA_IT_TCIF6);

    uart_comm_SetTxStatus(uart_comm_transmit_finished);
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
uart_comm_state_en uart_comm_SendString(char String[], u8 Length)
{
	/* local vars */
	uart_comm_state_en return_state = uart_comm_ok;

	if ((uart_comm_state == uart_comm_started) && (uart_comm_transmission_state == uart_comm_transmit_idle))
	{
		uart_comm_transmission_state = uart_comm_transmit_transmitting;
		uart_comm_tx_timeout_cnt_u16 = USART2_TX_WAIT_CNT;

		/* Send data over uart_comm */
		DMA_SetCurrDataCounter(USART2_Tx_DMA_Stream, Length);
		DMA_MemoryTargetConfig(USART2_Tx_DMA_Stream, (u32)&String[0], DMA_Memory_0);

		DMA_Cmd(USART2_Tx_DMA_Stream, ENABLE);
		DMA_ITConfig(USART2_Tx_DMA_Stream, DMA_IT_TC, ENABLE);
	}
	else
	{
		return_state = uart_comm_error;
		if (uart_comm_state != uart_comm_started)
		{
			uart_comm_transmission_state = uart_comm_transmit_error;
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
uart_comm_state_en uart_comm_SendFrame(u8 * Frame, u8 Length)
{
	/* local vars */
	uart_comm_state_en return_state = uart_comm_ok;

	if ((uart_comm_state == uart_comm_started) && (uart_comm_transmission_state == uart_comm_transmit_idle))
	{
		uart_comm_transmission_state = uart_comm_transmit_transmitting;
		uart_comm_tx_timeout_cnt_u16 = USART2_TX_WAIT_CNT;

		/* Send data over uart_comm */
		DMA_SetCurrDataCounter(USART2_Tx_DMA_Stream, Length);
		DMA_MemoryTargetConfig(USART2_Tx_DMA_Stream, (u32)&Frame[0], DMA_Memory_0);

		DMA_Cmd(USART2_Tx_DMA_Stream, ENABLE);
		DMA_ITConfig(USART2_Tx_DMA_Stream, DMA_IT_TC, ENABLE);
	}
	else
	{
		return_state = uart_comm_error;
		if (uart_comm_state != uart_comm_started)
		{
			uart_comm_transmission_state = uart_comm_transmit_error;
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
uart_comm_state_en uart_comm_RequestFrame(u32 Length)
{
	/* local vars */
	uart_comm_state_en return_state = uart_comm_ok;

	if ((uart_comm_state == uart_comm_started) && (uart_comm_receive_state == uart_comm_receive_idle))
	{
		uart_comm_receive_state = uart_comm_receive_receiving;
		uart_comm_rx_timeout_cnt_u16 = USART2_RX_WAIT_CNT;
		uart_comm_NewRxMsg_bl = FALSE;

		/* Send data over uart_comm */
		DMA_SetCurrDataCounter(USART2_Rx_DMA_Stream, Length);
		DMA_MemoryTargetConfig(USART2_Rx_DMA_Stream, (u32)uart_comm_RxBuffTemp, (u32)&USART2->DR);

		DMA_Cmd(USART2_Rx_DMA_Stream, ENABLE);
		DMA_ITConfig(USART2_Rx_DMA_Stream, DMA_IT_TC, ENABLE);
	}
	else
	{
		return_state = uart_comm_error;
		if (uart_comm_state != uart_comm_started)
		{
			uart_comm_receive_state = uart_comm_receive_error;
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
uart_comm_transmit_state_en uart_comm_GetTxStatus(void)
{
	return uart_comm_transmission_state;
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
void uart_comm_SetTxStatus(uart_comm_transmit_state_en state)
{
	uart_comm_transmission_state = state;
	if (uart_comm_transmit_finished == state)
	{
		uart_comm_transmission_state = uart_comm_transmit_idle;
	}
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
uart_comm_receive_state_en uart_comm_GetRxStatus(void)
{
	return uart_comm_receive_state;
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
void uart_comm_SetRxStatus(uart_comm_receive_state_en state)
{
	uart_comm_receive_state = state;
	if (uart_comm_receive_finished == state)
	{
		uart_comm_receive_state = uart_comm_receive_idle;
	}
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
boolean_t uart_comm_IsNewMsg(void)
{
	return uart_comm_NewRxMsg_bl;
}
