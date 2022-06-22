/******************************************************************************
* Project        : BswProject
* Component      : SysMode   
* FileName       : SysMode_1.c
* Company        : HCM Universyty of Technology. 
* Author         : Huy Le
* Description    : 
* Limitations    : 
******************************************************************************/

#ifndef USART1_GPS_PUBLIC_H_
#define USART1_GPS_PUBLIC_H_

/*------------------------------------------------------------------------------------------------------------
 Include-Files
 ------------------------------------------------------------------------------------------------------------*/
#include "stm_base.h"

/*------------------------------------------------------------------------------------------------------------
 Configuaration
 ------------------------------------------------------------------------------------------------------------*/

/** ******************************   DEFINE USART1   ***************************************************
* @brief   GPIO define 
*/
	#define USART1_INIT_BAUDRATE							9600 /* 115200 */
	#define USART1_RCC_GPIO_PORT							RCC_AHB1Periph_GPIOA
	#define USART1_RCC_APBPeriph_UARTx						RCC_APB2Periph_USART1
	#define USART1_UARTx									USART1
	#define USART1_GPIOx									GPIOA
	#define USART1_GPIO_AF									GPIO_AF_USART1
	#define USART1_GPIO_Pin_RX								GPIO_Pin_10
	#define USART1_GPIO_Pin_TX								GPIO_Pin_9
	#define USART1_GPIO_PinSourceRX							GPIO_PinSource10
	#define USART1_GPIO_PinSourceTX							GPIO_PinSource9			
/** 
* @brief   DMA define 
*/
	#define USART1_Rx_DMAx									RCC_AHB1Periph_DMA2
	#define USART1_Rx_DMA_Channel							DMA_Channel_4
	#define USART1_Rx_DMA_Stream							DMA2_Stream5
	#define USART1_Rx_DMA_IRQn								DMA2_Stream5_IRQn
	#define USART1_Tx_DMAx									RCC_AHB1Periph_DMA2
	#define USART1_Tx_DMA_Channel							DMA_Channel_4
	#define USART1_Tx_DMA_Stream							DMA2_Stream7
	#define USART1_Tx_DMA_IRQn								DMA2_Stream7_IRQn

/* Timeout counter if transmision/reception not finish on time */
/* uart_gps_Main task = 5ms */
#define USART1_TX_WAIT_CNT      200u	/* Timeout(millisecond) = USART1_TX_WAIT_CNT * time base of uart_gps_Main task */
#define USART1_RX_WAIT_CNT      200u	/* Timeout(millisecond) = USART1_TX_WAIT_CNT * time base of uart_gps_Main task */

#define USART1_GPS_BUFF_SIZE    60u
#define USART1_GPS_CFG_TABLE_SIZE    6u

/**< list with all transmittion states */
typedef enum uart_gps_transmit_enum
{
	uart_gps_transmit_noinit	= 0,/**< uart_gps noinit */
	uart_gps_transmit_idle,			/**< uart_gps idle */
	uart_gps_transmit_transmitting,	/**< uart_gps transmitting */
	uart_gps_transmit_finished,		/**< uart_gps transmision finished */
	uart_gps_transmit_error,		/**< uart_gps transmision error */
	uart_gps_transmit_state_end
} uart_gps_transmit_state_en;

/**< list with all receiving states */
typedef enum uart_gps_receive_enum
{
	uart_gps_receive_noinit	= 0,	/**< uart_gps noinit */
	uart_gps_receive_idle,			/**< uart_gps idle */
	uart_gps_receive_receiving,		/**< uart_gps recieving */
	uart_gps_receive_finished,		/**< uart_gps finished */
	uart_gps_receive_error,			/**< uart_gps recieve error */
	uart_gps_receive_state_end
} uart_gps_receive_state_en;

/**< list with all uart_gps states */
typedef enum uart_gps_state_enum
{
	uart_gps_started = 0,	/**< uart_gps idle */
	uart_gps_stopped,		/**< uart_gps stopped */
	uart_gps_error,			/**< uart_gps error */
	uart_gps_ok,			/**< uart_gps error */
	uart_gps_end
} uart_gps_state_en;

/*------------------------------------------------------------------------------------------------------------
 Public Variable Declarations
 ------------------------------------------------------------------------------------------------------------*/
extern u8 GPS_RxBuff[USART1_GPS_BUFF_SIZE];

// const u8 GPS_Disable_GPGSV[11] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x03, 0x00, 0xFD, 0x15};
// const u8 GPS_Disable_GPRMC[11] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x04, 0x00, 0xFE, 0x17};
// const u8 GPS_Disable_GPVTG[11] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x05, 0x00, 0xFF, 0x19};
// const u8 GPS_Disable_GPGGA[11] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x00, 0x00, 0xFA, 0x0F};
// const u8 GPS_Disable_GPGSA[11] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x02, 0x00, 0xFC, 0x13};
// const u8 GPS_Enable_GPGLL[11] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x01, 0x01, 0xFC, 0x12};

// u8 *GPS_Cgf_table[USART1_GPS_CFG_TABLE_SIZE] =
// {
// 	GPS_Disable_GPGSV,
// 	GPS_Disable_GPRMC,
// 	GPS_Disable_GPVTG,
// 	GPS_Disable_GPGGA,
// 	GPS_Disable_GPGSA,
// 	GPS_Enable_GPGLL
// };
/*------------------------------------------------------------------------------------------------------------
 Public Function Declarations
 ------------------------------------------------------------------------------------------------------------*/

extern void uart_gps_Init(void);
extern void uart_gps_Main(void);
extern void uart_gps_Start(void);
extern void uart_gps_Stop(void);
extern boolean_t uart_gps_IsNewMsg(void);
extern uart_gps_state_en uart_gps_SendString(char String[], u8 Length);
extern uart_gps_state_en uart_gps_SendFrame(u8 * Frame, u8 Length);
extern uart_gps_state_en uart_gps_RequestFrame(u8 * Rx_Buff_pu8, u8 Length);
extern uart_gps_transmit_state_en uart_gps_GetTxStatus(void);
extern uart_gps_receive_state_en uart_gps_GetRxStatus(void);

#endif /* USART1_GPS_PUBLIC_H_ */
