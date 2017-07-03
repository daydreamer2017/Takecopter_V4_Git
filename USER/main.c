/**************************************************************************//**
Project name: 	takecopter_v4
Author: 				Xi Chen
Created date: 	26/06/2017
Description:		This project is the full version of takecopter attitude control for takecopter_v4 with brussless DC motors.
								
Updated:				
******************************************************************************/

/* Standard includes. */

/* STM32F10 Library includes. */

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Application includes. */
#include "sys_init.h"
#include "att_controller.h"
#include "USART3_IMU.h"	  
#include "receiver.h"		
 int main(void)
 {
		 sysInit(INNER_LOOP_DELTAT); 
		 xTaskCreate( xTaskIMUDecoder, "IMU decoder task", TASK_STACK_SIZE, NULL, IMU_DECODER_PRIORITY, NULL );
		 xTaskCreate( xTaskReceiver, "RC receiver task", TASK_STACK_SIZE, NULL, RECEIVER_PRIORITY, NULL );
		 xTaskCreate( xTaskController, "Main attitude controller task", TASK_STACK_SIZE, NULL, CONTROLLER_PRIORITY, NULL );
		 vTaskStartScheduler(); 
 }
 

			



