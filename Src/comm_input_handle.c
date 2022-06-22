/******************************************************************************
* Company        : HCM Universyty of Technology. 
* Author         : Huy Le
* Description    : 
* Limitations    : 
******************************************************************************/

/******************************************************************************
* Include Files
******************************************************************************/
#include "comm_input_handle.h"
/******************************************************************************
* Local Variables
******************************************************************************/

/******************************************************************************
* Global Variables
******************************************************************************/
//Frame Rx
u8 Comm_RxBuff[BUFF_SIZE_RX];

/******************************************************************************
* Global Function Declarations
******************************************************************************/

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
void comm_input_handle_init(void)
{
	Comm_RxBuff[0]=0;//1 la kiem soat truong hop cac bit bang 0
	Comm_RxBuff[1]=0;
	Comm_RxBuff[2]=45;
	Comm_RxBuff[3]=0;
	Comm_RxBuff[4]=0;
	Comm_RxBuff[5]=0;
	Comm_RxBuff[6]=0;
	Comm_RxBuff[7]=0;
	Comm_RxBuff[8]='\r';
	Comm_RxBuff[9]='\n';
}

u8 brakeRx=0,modeRx=0,turnRx=0,hornRx=0,fr_lightRx=0, speedRx =0;
s8 angleRx=0;
u8 Process_RequestFrame(u8 * Frame,u8 Length)  //done
{
	if ((Frame[0]=='O')&(Frame[1]=='K')&(Frame[2]==0x0D)&(Frame[3]==0x0A))
	{
//		uart_comm_SetRxStatus(uart_comm_receive_true);
		GPIO_ToggleBits(GPIOD,GPIO_Pin_15);
		return 1;
		
	}
	else
	{
//		uart_comm_SetRxStatus(uart_comm_receive_false);
		return 0;
	}
	
}

u8 Rx_Frame_getSpeed(void)
{
	return speedRx;
}
u8 Rx_Frame_getBrake(void)
{
	return brakeRx;
}
s8 Rx_Frame_getAngle(void)
{
	return angleRx;
}
u8 Rx_Frame_getMode(void)
{
	return modeRx;
}
u8 Rx_Frame_getTurn(void)
{
	return turnRx;
}
u8 Rx_Frame_getFr_light(void)
{
	return fr_lightRx;

}
u8 Rx_Frame_getHorn(void)
{	
	return hornRx;
}

/***Differnt from source
Aim:
Edit by: Thanh kroos
************************
***/
static u8 Rx_Len = 10u;
void comm_input_handler(u32 expect_res_size)
{
	if (TRUE == uart_comm_IsNewMsg())
	{
		Process_RequestFrame(uart_comm_RxBuffTemp, expect_res_size);
	}
	uart_comm_SetRxStatus(uart_comm_receive_idle);
	if (uart_comm_GetRxStatus() == uart_comm_receive_idle)
	{
		uart_comm_RequestFrame(expect_res_size); // Comm_RxBuff dua vao khong anh huong
	}
}
