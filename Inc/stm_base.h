/******************************************************************************
* FileName       : 
* Company        : HCM Universyty of Technology. 
* Author         : Huy Le
* Description    : 
* Limitations    : 
******************************************************************************/

#ifndef _BASETYPE_H
#define _BASETYPE_H

/* STM32F4 base includes */
#include <stdio.h>
#include "string.h"
#include "stm32f4xx.h" 
#include "stm32f4xx_adc.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_crc.h"
#include "stm32f4xx_cryp.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_dbgmcu.h"
#include "stm32f4xx_dcmi.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_flash.h"
#include "stm32f4xx_fsmc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_hash.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_iwdg.h"
#include "stm32f4xx_pwr.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_rng.h"
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_sdio.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_wwdg.h"
#include "system_stm32f4xx.h"

/* brief abstract Endless Loops */
#define ENDLESS 1

typedef enum {
FALSE = 0, /**<    FALSE */
TRUE       /**<    TRUE */
} boolean_t;

typedef char                c8;

typedef unsigned char       u8;

typedef unsigned short      u16;

typedef unsigned int        u32;

typedef signed char         s8;

typedef short               s16;

typedef int                 s32;

typedef char u1;

#define CONSTPARAM
#define PRIVATE
#define INTERRUPT
#define VOLATILE

#define OFF     (0)
#define ON      (1)

#ifndef PUBLIC
#define PUBLIC
#endif
#ifndef ISRFUNC
#define ISRFUNC        __interrupt
#endif
#ifndef PRIVATE
#define PRIVATE        static
#endif
#ifndef CONST
#define CONST       const
#endif
#ifndef EXTERN
#define EXTERN      extern
#endif
#ifndef VOLATILE
#define VOLATILE    volatile
#endif
#ifndef INLINE
#define INLINE      (#pragma inline)

#define true ((u1)1u)
#define false ((u1)0u)

typedef const unsigned char cu8;

typedef const unsigned short cu16;

typedef const unsigned long cu32;

typedef unsigned char* ptu8;

typedef CONST u8 * cptu8;

typedef unsigned short* ptu16;

typedef signed short* pts16;

typedef CONST u16* cptu16;

typedef unsigned long* ptu32;

typedef signed long* pts32;

typedef const signed long cs32;

typedef signed char* pts8;

typedef CONST signed long* cpts32;

#ifndef NULL
  #define NULL ( void * ) 0
#endif
	
/**
 * @brief definitions to access a specific bit in a byte
 */
typedef enum BITMASK_T {
    BIT0 = 0x01, /**<    Bit 0 */
    BIT1 = 0x02, /**<    Bit 1 */
    BIT2 = 0x04, /**<    Bit 2 */
    BIT3 = 0x08, /**<    Bit 3 */
    BIT4 = 0x10, /**<    Bit 4 */
    BIT5 = 0x20, /**<    Bit 5 */
    BIT6 = 0x40, /**<    Bit 6 */
    BIT7 = 0x80  /**<    Bit 7 */
} bitmask_t;

#define BIT_SET (u8)1
#define BIT_CLR (u8)0

/**\brief  min value for s8 */
#define base_d_min_val_s8    ((s8)-128)
/**\brief  max. value for s8 */
#define base_d_max_val_s8    ((s8)127)
/**\brief  min value for u8 */
#define base_d_min_val_u8    ((u8)0)
/**\brief  max. value for u8 */
#define base_d_max_val_u8    ((u8)255)
/**\brief  min value for s16 */
#define base_d_min_val_s16   ((s16)-32768)
/**\brief  max value for s16 */
#define base_d_max_val_s16   ((s16)32767)
/**\brief  min value for u16 */
#define base_d_min_val_u16  ((u16)0)
/**\brief  max value for u16 */
#define base_d_max_val_u16  ((u16)65535)
/**\brief  min value for s32 */
#define base_d_min_val_s32    ((s32)-2147483648)
/**\brief  max value for s32 */
#define base_d_max_val_s32    ((s32)2147483647)
/**\brief  min value for u32 */
#define base_d_min_val_u32   ((u32)0)
/**\brief  max value for u32 */
#define base_d_max_val_u32   ((u32)4294967295u)
/**\brief  max id for a u16 mask */
#define base_d_max_u16_mask_id_u8   ((u8)16)
/**
 * @brief generic bitfield type for 8 bit
 */
typedef struct
{
u8 bit0_u1 : 1;  /**< Bit No.0  */
u8 bit1_u1 : 1;  /**< Bit No.1  */
u8 bit2_u1 : 1;  /**< Bit No.2  */
u8 bit3_u1 : 1;  /**< Bit No.3  */
u8 bit4_u1 : 1;  /**< Bit No.4  */
u8 bit5_u1 : 1;  /**< Bit No.5  */
u8 bit6_u1 : 1;  /**< Bit No.6  */
u8 bit7_u1 : 1;  /**< Bit No.7  */
}bitfield_u8_t;


/**
 * @brief generic bitfield type for 16 bit
 */
typedef struct
{
u16 bit0_u1 : 1;	 /**< Bit No.0  */
u16 bit1_u1 : 1;	 /**< Bit No.1  */
u16 bit2_u1 : 1;	 /**< Bit No.2  */
u16 bit3_u1 : 1;	 /**< Bit No.3  */
u16 bit4_u1 : 1;	 /**< Bit No.4  */
u16 bit5_u1 : 1;	 /**< Bit No.5  */
u16 bit6_u1 : 1;	 /**< Bit No.6  */
u16 bit7_u1 : 1;	 /**< Bit No.7  */
u16 bit8_u1 : 1;	 /**< Bit No.8  */
u16 bit9_u1 : 1;	 /**< Bit No.9  */
u16 bit10_u1 : 1;	 /**< Bit No.10 */
u16 bit11_u1 : 1;	 /**< Bit No.11 */
u16 bit12_u1 : 1;	 /**< Bit No.12 */
u16 bit13_u1 : 1;	 /**< Bit No.13 */
u16 bit14_u1 : 1;	 /**< Bit No.14 */
u16 bit15_u1 : 1;	 /**< Bit No.15 */
}bitfield_u16_t;


/**
 * @brief Help structure for alternate halfword access
 */
typedef struct
{
u8 low_byte_u8;	 /**< for low byte access   */
u8 high_byte_u8; /**< for high byte access  */
}bytes_u16_t;

/**
 * @brief variant-type for alternate halfword access ( bits/bytes/halfword )
 */
typedef union
{
u16            halfword_u16; /**< for halfword access */
bytes_u16_t    bytes;        /**< byte access         */
bitfield_u16_t bits;         /**< bit access          */
}variant_u16_t;


/* Fault_t: To mark an object as a defect.	 */
typedef enum { 
	NO_FAULT = 0, 	
	FAULT = 1  
}Fault_t;

#endif


#endif /* _BASETYPE_H */
