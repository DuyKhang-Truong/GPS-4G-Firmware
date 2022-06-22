
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm_base.h"
/* Application includes */
#include "gps_handler.h"
#include "uart_comm.h"
#include "comm_output_handle.h"
#include "comm_input_handle.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/*  These function configure scheduler timer
    Sampling time is set by changing value PERIOD
    in the file "operating_system.c"
*/

//extern uint32_t SysTick_5ms_count;
//extern boolean_t SysTick_5ms_flag;

extern void SysTick_Handler(void);
extern void system_config(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_IT_H */


