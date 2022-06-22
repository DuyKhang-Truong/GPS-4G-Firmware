/******************************************************************************
* Project        : BswProject
* Component      : SysMode   
* FileName       : SysMode_1.c
* Company        : HCM Universyty of Technology. 
* Author         : Huy Le
* Description    : 
* Limitations    : 
******************************************************************************/

#ifndef USART2_PUBLIC_H_
#define USART2_PUBLIC_H_

/*------------------------------------------------------------------------------------------------------------
 Include-Files
 ------------------------------------------------------------------------------------------------------------*/
#include "stm_base.h"
#include "access_GUI.h"

/*------------------------------------------------------------------------------------------------------------
 Configuaration
 ------------------------------------------------------------------------------------------------------------*/

/** **********  DEFINE USART2   **************************************************
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
	#define	USART2_Rxbuff_Size								1024u
	#define	Get_GPS_data_Size									100u
	#define USART2_Rx_DMAx									RCC_AHB1Periph_DMA1
	#define USART2_Rx_DMA_Stream							DMA1_Stream5
	#define USART2_Rx_DMA_IRQn								DMA1_Stream5_IRQn
	#define USART2_Tx_DMAx									RCC_AHB1Periph_DMA1
	#define USART2_Tx_DMA_Stream							DMA1_Stream6
	#define USART2_Tx_DMA_IRQn								DMA1_Stream6_IRQn

/* Timeout counter if transmision/reception not finish on time */
/* uart_comm_Main task = 5ms */
#define USART2_TX_WAIT_CNT      200u /* Timeout(millisecond) = USART2_TX_WAIT_CNT * time base of uart_comm_Main task */
#define USART2_RX_WAIT_CNT      1000u /* Timeout(millisecond) = USART2_TX_WAIT_CNT * time base of uart_comm_Main task */

/**< list with all transmittion states */
typedef enum uart_comm_transmit_enum
{
	uart_comm_transmit_noinit	= 0,	/**< uart_comm noinit */
	uart_comm_transmit_idle, 			/**< uart_comm idle */
	uart_comm_transmit_transmitting,	/**< uart_comm transmitting */
	uart_comm_transmit_finished,		/**< uart_comm transmision finished */
	uart_comm_transmit_error,			/**< uart_comm transmision error */
	uart_comm_transmit_state_end
} uart_comm_transmit_state_en;

/**< list with all receiving states */
typedef enum uart_comm_receive_enum
{
	uart_comm_receive_noinit	= 0,	/**< uart_comm noinit */
	uart_comm_receive_idle,				/**< uart_comm idle */
	uart_comm_receive_receiving,		/**< uart_comm recieving */
	uart_comm_receive_finished,			/**< uart_comm finished */
	uart_comm_receive_error,			/**< uart_comm recieve error */
	uart_comm_receive_false,
	uart_comm_receive_true,
	uart_comm_receive_state_end
} uart_comm_receive_state_en;

/**< list with all uart_comm states */
typedef enum uart_comm_state_enum
{
	uart_comm_started = 0,	/**< uart_comm idle */
	uart_comm_stopped,		/**< uart_comm stopped */
	uart_comm_error,		/**< uart_comm error */
	uart_comm_ok,			/**< uart_comm error */
	uart_comm_end
} uart_comm_state_en;

/**< list with all uart_comm states */
typedef enum uart_comm_type
{
	uart_comm_type_free_run = 0,
	uart_comm_type_master_slave,
	uart_comm_type_end 
} uart_comm_type_en;
/* ********************************************************************************* */
/** 
* @brief   Device name define
*/
typedef enum
{
	Boardcast0 = 0,
	Boardcast1,
	ECU,
	PRU, 
} DeviceID_en;

/** 
* @brief   Function Description
*/
typedef enum
{
	NotUse = 0,
	MasterSend_VehicleControlData,
	MasterRequest_VehicleControlData,
	MasterRequest_VehicleStatusData, 
} FunctionDesc_en;

/** 
* @brief   Sub Function Description Master send Vehicle Control Data
*/
typedef enum
{
	SpeedChangedOnly = 0,
	BrakePercetageChangedOnly,
	MovingDirectionChangedOnly,
	TurnSignalChangedOnly, 
	HornSignalChangedOnly, 
	AllChanged, 
} MasterSendVehicleControlData_en;

/** 
* @brief   Sub Function Description Master request Vehicle control Data
*/
typedef enum
{
	RequestAllVehicleData = 0,
} MasterRequestVehicleControlData_en;

/** 
* @brief   Sub Function Description Master request Vehicle Status Data
*/
typedef enum
{
	RequestLongitude = 0,
	RequestLatitude,
} MasterRequestVehicleStatusData_en;

/* ********************************************************************************* */
/** 
* @brief   Vehicle Control Data define 
*/
typedef struct
{
	u8 MovingDirection : 1;
	u8 TurnSignal : 2;
	u8 HornSignal : 1;
}VehicleControlDataBit_t;

typedef union
{
	u8 ByteVal_u8;
	VehicleControlDataBit_t DataBit;
}VehicleControlDataByte3_un;

typedef struct
{
	s8 Speed;
	u8 BrakePercetage;
	s8 StearingAngle;
	VehicleControlDataByte3_un VehicleControlDataBit_un;
}Comm_VehicleControlData_t;

/* ********************************************************************************* */
/** 
* @brief   Vehicle status data Longitude\Latitude define 
*/
typedef struct
{
	u8 Byte0;
	u8 Byte1;
	u8 Byte2;
	u8 Byte3;
}FloatExtract4Byte_t;

typedef union
{
	float Value;
	FloatExtract4Byte_t DataByte;
}FloatLongLatData_un;

typedef struct
{
	FloatLongLatData_un Value_un;
	u8 Direction;
}Comm_LongLatData_un;

/* ********************************************************************************* */
/** 
* @brief   Message header data
*/
typedef struct
{
	u8 DeviceID;
	u8 Function;
	u8 SubFunction;
	u8 DataLen;
}Comm_MsgHeaderData_t;

/*------------------------------------------------------------------------------------------------------------
 Public Variable Declarations
 ------------------------------------------------------------------------------------------------------------*/

extern Comm_VehicleControlData_t Comm_VehicleControlData;
extern Comm_LongLatData_un Comm_LongitudeData;
extern Comm_LongLatData_un Comm_LatitudeData;

/*------------------------------------------------------------------------------------------------------------
 Public Function Declarations
 ------------------------------------------------------------------------------------------------------------*/
extern u8 uart_comm_RxBuffTemp[USART2_Rxbuff_Size];
extern u8 a_buff[USART2_Rxbuff_Size];



extern void uart_comm_Init(void);
extern void uart_comm_Main(void);
extern void uart_comm_Start(void);
extern void uart_comm_Stop(void);
extern void uart_comm_SetTxStatus(uart_comm_transmit_state_en state);
extern void uart_comm_SetRxStatus(uart_comm_receive_state_en state);

extern uart_comm_state_en uart_comm_SendString(char String[], u8 Length);

extern uart_comm_state_en uart_comm_SendFrame(u8 * Frame, u8 Length); /*  */
extern uart_comm_state_en uart_comm_RequestFrame(u32 Length); /*  */

extern boolean_t uart_comm_IsNewMsg(void);

extern uart_comm_transmit_state_en uart_comm_GetTxStatus(void);
extern uart_comm_receive_state_en uart_comm_GetRxStatus(void);

#endif /* USART2_PUBLIC_H_ */
