/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ACCESS_GUI_H
#define __ACCESS_GUI_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "gps_handler.h"
#include "LTE_handler.h"
#include "sign.h"
#include <string.h>
#include <stdbool.h>


/*Defines*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables*/
#define RXBUFFERSIZEGUI 1
#define TXBUFFERSIZEGUI 106

#define BUFFERSIZEGPS		52



union DoubleToByte{
 double myDouble;
 uint8_t myByte[8];
} ;

//extern uint8_t txBufferGUI[RXBUFFERSIZEGUI] = {'$',0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x00,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x00,140,0x0D,0x0A};
extern uint8_t txBufferGUI[TXBUFFERSIZEGUI];
extern uint8_t rxBufferGUI[RXBUFFERSIZEGUI];

extern char getBookingID;

/* Exported functions ------------------------------------------------------- */
void UART_GUI_Config (uint32_t baudrate);

void sendGPSDataToGUI (void);
void sendTaskToGUI (void);


#endif /* __ACCESS_GUI_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
