/* Includes ------------------------------------------------------------------*/
#include "gps_config.h"
#include <stdbool.h>
//#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
uint8_t txbuff_cofFRQ50[] = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x32, 0x00, 0x01, 0x00, 0x01, 0x00, 0x48, 0xE6};
uint8_t txbuff_cofFRQ100[] = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x64, 0x00, 0x01, 0x00, 0x01, 0x00, 0x7A, 0x12};
uint8_t txbuff_cofFRQ150[] = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x96, 0x00, 0x01, 0x00, 0x01, 0x00, 0xAC, 0x3E};
uint8_t txbuff_cofFRQ200[] = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0xC8, 0x00, 0x01, 0x00, 0x01, 0x00, 0xDE, 0x6A};

uint8_t txbuff_cofFRQ[] = {0xB5, 0x62, 0x06, 0x08, 0x00, 0x00, 0x0E, 0x30};
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void GPRMCpkg (bool state);
void GPVTGpkg (bool state);
void GPGGApkg (bool state);
void GPGSApkg (bool state);
void GPGSVpkg (bool state);
void GPGLLpkg (bool state);
void freqConfig (uint8_t freq);

/**
  * @brief  GPRMCpkg Configuaration
  * @param  None
  * @retval None
  */
void GPRMCpkg (bool state)
{
	if (state)
	{
		uint8_t txbuff_GPRMCpkg[] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x04, 0x01, 0xFF, 0x18};
		UART_Transmit(txbuff_GPRMCpkg, 11);
	}
	else {
		uint8_t txbuff_GPRMCpkg[] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x04, 0x00, 0xFE, 0x17};
		UART_Transmit(txbuff_GPRMCpkg, 11);
	}
	
}

/**
  * @brief  GPVTGpkg Configuaration
  * @param  None
  * @retval None
  */
void GPVTGpkg (bool state)
{
	if (state)
	{
		uint8_t txbuff_GPVTGpkg[] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x05, 0x01, 0x00, 0x1A};
		UART_Transmit(txbuff_GPVTGpkg, 11);
	}
	else {
		uint8_t txbuff_GPVTGpkg[] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x05, 0x00, 0xFF, 0x19};
		UART_Transmit(txbuff_GPVTGpkg, 11);
	}
	
}
/**
  * @brief  GPGGApkg Configuaration
  * @param  None
  * @retval None
  */
void GPGGApkg (bool state)
{
	if (state)
	{
		uint8_t txbuff_GPGGApkg[] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x00, 0x01, 0xFB, 0x10};
		UART_Transmit(txbuff_GPGGApkg, 11);
	}
	else {
		uint8_t txbuff_GPGGApkg[] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x00, 0x00, 0xFA, 0x0F};
		UART_Transmit(txbuff_GPGGApkg, 11);
	}
	
}

/**
  * @brief  GPGSApkg Configuaration
  * @param  None
  * @retval None
  */
void GPGSApkg (bool state)
{
	if (state)
	{
		uint8_t txbuff_GPGSApkg[] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x02, 0x01, 0xFD, 0x14};
		UART_Transmit(txbuff_GPGSApkg, 11);
	}
	else {
		uint8_t txbuff_GPGSApkg[] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x02, 0x00, 0xFC, 0x13};
		UART_Transmit(txbuff_GPGSApkg, 11);
	}
	
}

/**
  * @brief  GPGSVpkg Configuaration
  * @param  None
  * @retval None
  */
void GPGSVpkg (bool state)
{
	if (state)
	{
		uint8_t txbuff_GPGSVpkg[] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x03, 0x01, 0xFE, 0x16};
		UART_Transmit(txbuff_GPGSVpkg, 11);
	}
	else {
		uint8_t txbuff_GPGSVpkg[] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x03, 0x00, 0xFD, 0x15};
		UART_Transmit(txbuff_GPGSVpkg, 11);
	}
	
}

/**
  * @brief  GPGLLpkg Configuaration
  * @param  None
  * @retval None
  */
void GPGLLpkg (bool state)
{
	if (state)
	{
		uint8_t txbuff_GPGLLpkg[] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x01, 0x01, 0xFC, 0x12};
		UART_Transmit(txbuff_GPGLLpkg, 11);
	}
	else {
		uint8_t txbuff_GPGLLpkg[] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x01, 0x00, 0xFB, 0x11};
		UART_Transmit(txbuff_GPGLLpkg, 11);
	}
	
}

/**
  * @brief  FREQ Configuaration
  * @param  None
  * @retval None
  */
void freqConfig (uint8_t freq)
{
	switch (freq)
	{
		case 50:
			UART_Transmit(txbuff_cofFRQ50, 14);
			UART_Transmit(txbuff_cofFRQ, 8);
			break;
		
		case 100:
			UART_Transmit(txbuff_cofFRQ100, 14);
			UART_Transmit(txbuff_cofFRQ, 8);
			break;
		
		case 150:
			UART_Transmit(txbuff_cofFRQ150, 14);
			UART_Transmit(txbuff_cofFRQ, 8);
			break;
		
		case 200:
			UART_Transmit(txbuff_cofFRQ200, 14);
			UART_Transmit(txbuff_cofFRQ, 8);
			break;
		
		default:
			break;
		
	}
	
}

/**
  * @brief  GPS Configuaration
  * @param  None
  * @retval None
  */
void gpsConfig (void)
{
	GPRMCpkg (true);
	GPVTGpkg (false);
	GPGGApkg (false);
	GPGSApkg (false);
	GPGSVpkg (false);
	GPGLLpkg (false);
	freqConfig(200);
}

