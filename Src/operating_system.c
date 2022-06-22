/* Includes ------------------------------------------------------------------*/
#include "operating_system.h"

uint32_t	SysTick_5ms_count;
boolean_t	SysTick_5ms_flag;
uint8_t cout=0;

static void Cyclic1xBaseMain(void);
static void Cyclic2xBaseMain(void);
static void Cyclic4xBaseMain(void);
static void Cyclic8xBaseMain(void);

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
void system_config(void)
{
//	comm_input_handle_init();
	comm_output_handle_init();
	uart_comm_Init();
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
void SysTick_Handler(void)
{
	static uint8_t tSysTimeSliceCnt_u8 = 0u;
	SysTick_5ms_flag = TRUE;
	SysTick_5ms_count++;
	tSysTimeSliceCnt_u8++;

	/* call 5ms  function */
	Cyclic1xBaseMain();

	if (tSysTimeSliceCnt_u8 == 1u)
	{
		/* call 40ms slice function */
		Cyclic8xBaseMain();
	}

	if ((tSysTimeSliceCnt_u8 % 2u) == 0u)
	{
		/* call 10ms slice function */
		Cyclic2xBaseMain();
	}

	if ((tSysTimeSliceCnt_u8 == 3u) || (tSysTimeSliceCnt_u8 == 7u))
	{
		/* call 20ms slice function */
		Cyclic4xBaseMain();
	}

	if (tSysTimeSliceCnt_u8 == 8u )
	{
		tSysTimeSliceCnt_u8 = 0u;
	}
}

/******************************************************************************
* Description: 
* Parameter: 
* Return Value: None
* Author: Huy Le
******************************************************************************/
static void Cyclic1xBaseMain(void)
{
	/* 5ms */
	uart_comm_Main();
//	uart_gps_Main();
}
//static u8 command_=97;
static u8 command_=99;
static u32 frame_n = 0;
u8 frame_id = 1;

static void Cyclic2xBaseMain(void)
{
	/* 10ms */
	// gps_handler();
	cout++;
	if (cout == 20) //1000ms
	{
			switch (command_){
				case 97:
				{
					comm_output_handler("AT+CRESET\r\n");
					command_ = 98;
					break;
				}
				case 98:
				{
					if (frame_n < 75)
					{
						frame_n++;
						command_ = 98;
					}
					else
					{
						command_ = 99;//
						frame_n = 0;;
					}
					break;
				}
				case 99:
				{
					comm_output_handler("ATE0\r\n");
					comm_input_handler(1024);
					command_ = 0;
					break;
				}
				case 0:
				{
					comm_output_handler("AT+HTTPINIT\r\n");
					comm_input_handler(1024);
					command_ = 1;
					break;
				}
				case 1:
				{
					comm_output_handler("AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"\r\n");
					comm_input_handler(1024);
					//					command_ = 2;//POST
					command_ = 9;//GET
					
					break;
				}
				case 2:
				{
					comm_output_handler("AT+HTTPPARA=\"URL\",\"https://fleetmonitoring.000webhostapp.com/createGPSdata.php\"\r\n");

					command_ = 3;

					
					break;
				}
				case 3:
				{
					comm_output_handler("AT+HTTPDATA=67,3000\r\n");
					command_ = 4;
					break;
				}
				case 4:
				{
					char gps_data[] = "car_gps_ID=2&longtitude=10639.67639&latitude=1045.59172&velocity=16\r\n";
					
					for (int i = 0; i < 11; i++)
					{
						gps_data[i + 24] = rawLtnInfo.Lon[i];
						
					}
					
					for (int i = 0; i < 10; i++)
					{
						gps_data[i + 45] = rawLtnInfo.Lat[i];
						
					}	
					
//					comm_output_handler("car_gps_ID=8&longtitude=18.07&latitude=87.01&velocity=18\r\n");
					comm_output_handler(gps_data);
					command_ = 5;
					break;
				}		
				case 5:
				{
					comm_output_handler("AT+HTTPACTION=1\r\n");
					command_ = 6;
					break;
				}
				case 6:
				{
					if (frame_n < 20)
					{
//						comm_output_handler("AT+HTTPREAD=0,20\r\n");
						frame_n++;
						command_ = 6;
					}
					else
					{
						command_ = 2;//POST NEW FRAME
						frame_n = 0;;
					}
					
					break;
				}
				case 9:
				{
					switch (frame_id){
						case 1:
						{
							comm_output_handler("AT+HTTPPARA=\"URL\",\"https://fleetmonitoring.000webhostapp.com/searchGPSdata.php?frame_ID=1\"\r\n");
							frame_id = 2;
							break;
						}
						case 2:
						{
							comm_output_handler("AT+HTTPPARA=\"URL\",\"https://fleetmonitoring.000webhostapp.com/searchGPSdata.php?frame_ID=2\"\r\n");
							frame_id = 3;
							break;
						}
						case 3:
						{
							comm_output_handler("AT+HTTPPARA=\"URL\",\"https://fleetmonitoring.000webhostapp.com/searchGPSdata.php?frame_ID=3\"\r\n");
							frame_id = 1;
							break;
						}
					}
						
//					comm_output_handler("AT+HTTPPARA=\"URL\",\"https://fleetmonitoring.000webhostapp.com/searchGPSdata.php?frame_ID=1\"\r\n");
					comm_input_handler(1024);
					command_ = 10;
					break;
				}
				case 10:
				{
					comm_output_handler("AT+HTTPACTION=0\r\n");
					comm_input_handler(1024);
					command_ = 11;
					break;
				}
				case 11:
				{
					if (frame_n < 25)
					{
						frame_n++;
						command_ = 11;
					}
					else
					{	
						comm_output_handler("AT+HTTPREAD=0,100\r\n");
						comm_input_handler(140);//126 doi voi frame cu						
						command_ = 9;//GET NEW FRAME
						frame_n = 0;;
					}
					break;
				}
			}
		cout = 0;
	}
	
}

static void Cyclic4xBaseMain(void)
{
	
	/* 20ms */
//	VehicleControl_handler();
//	comm_input_handler();
	
}

static void Cyclic8xBaseMain(void)
{
	/* 40ms */
//	encoder_handler();
	
}
