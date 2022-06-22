/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LTE_HANDLER_H
#define __LTE_HANDLER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "sign.h"
#include "delay.h"
#include "access_GUI.h"
#include <stdbool.h>
#include <string.h>

/*Defines*/

#define RXBUFFERSIZELTE 136
#define TXBUFFERSIZELTE 100

#define RXBUFFERSIZETASK 106

#define SAMPLETIME 6500

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
typedef enum
{
	Get = 0,
	Post,
	Donothing,
} SimModuleState;

typedef struct
{
	bool InitPB_DONE;
	bool InitATE0OK;
	
}initSimModule;


/* Exported variables*/
extern uint8_t txBufferLTE[TXBUFFERSIZELTE];
extern uint8_t rxBufferLTE[RXBUFFERSIZELTE];

extern initSimModule InitSimModule;
extern SimModuleState simModuleState;

/* Exported functions ------------------------------------------------------- */
void UART_LTE_Config (uint32_t baudrate);
void UART_LTE_Transmit (char *str);

void TIM2_InitModuleSim_Config(void);
void Init_SimModule (void);
void checkInitATE0OK (void);
void checkPB_DONE (void);

void PostOrGetRepeat (void); //enable TIM2 for check ATE0 OK
void waitForHTTPReadTIM (void); //enable TIM37
void disableTIM37 (void);
void disableTIM2 (void);

/**Get Data**/
void TIM7_Config(uint16_t SampleTime);//waitForHTTPReadTIM -- Timeout 5s
void getDataFromServer (void);
void HTTPReadData (void);
void reEnableDMARxLTE (uint8_t *rxBufferLTE);

/**Post Data**/
void postDataToServer (void);

void accessToRxBuffSimTIMCofig (void);//TIM3=5ms waitForHTTPReadTIM



#endif /* __LTE_HANDLER_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
