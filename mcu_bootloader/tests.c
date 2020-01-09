#include "include.h"

#ifndef NDEBUG

enum
{
	TEST_ALL_SERVO 		 = 0, 
	TEST_ONE_SERVO		 = 1, 
	TEST_DROP_MODULE	 = 2, 
	/*TEST_CELL_MEASURMENT_CASE			 = 3,
	TEST_ICM_CASE						 = 7,
	TEST_MAIN_TASK_CASE					 = 8,
	TEST_CAPACITY_BTWN_TWO_POINT_CASE	 = 9,
	TEST_BATTERY_CORE_TASK				 = 10,*/

};

static const uint32_t TEST_STATE 		= TEST_DROP_MODULE;

//=============================================================================
void TEST_init(void)
{

	switch(TEST_STATE)
	{		
		case(TEST_ALL_SERVO):	//Result: on the board tow LEDs must toggle
		{		

			break;
		}
		/*
		case():
		{
			
			break;
		}
		
		case():
		{			
			
			break;
		}
*/
		default:
			break;
	}	
}

//=============================================================================
void TEST_task(void)
{
	switch(TEST_STATE)
	{
		case(TEST_ALL_SERVO):
		{
			for(uint32_t i = 0; i < 3; i++)
			{
				servoCmd (i, LEFT);
				delay_ms(500);
				servoCmd (i, CENTER);
				delay_ms(500);
				servoCmd (i, RIGHT);
				delay_ms(500);					
			}

			break;
		}
	
		case(TEST_ONE_SERVO):
		{
			static uint16_t k = 0;
			servoCmd (k, LEFT);
			delay_ms(500);
			servoCmd (k, CENTER);
			delay_ms(500);
			servoCmd (k, RIGHT);
			delay_ms(500);
			break;
		}
		
		case(TEST_DROP_MODULE):
		{
			static uint16_t z = 0;
			static uint16_t n = 0;
			if(z)
			{
				modules_dropModule(n);
			}
			break;
		}
		
		default:
			break;
	}
}

#endif
