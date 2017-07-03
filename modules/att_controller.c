/* Standard includes. */
#include "string.h"

/* STM32F10 Library includes. */
#include "misc.h"
#include "stm32f10x_tim.h"
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
/* Application includes. */
#include "att_controller.h"
#include "USART3_IMU.h"	  
#include "receiver.h"		
#include "PID.h"
#include "rotors.h"

   static TaskHandle_t xTaskToNotify;

	 static float desiredThrust;//thrust reference
	 static float desiredThrustCom;//thrust reference with Euler ange feedforward
	 //u16	buff[5];
	 static float pwm1Ratio=0;
	 static float pwm2Ratio=0;
	 static float pwm3Ratio=0;
	 static float pwm4Ratio=0;
	 static Axis3f receiverData={0,0,0};
 	 static Axis3f rateActual={0,0,0};
 	 static Axis3f angleActual={0,0,0};
	 static Axis3f angleDesired={0,0,0};
	 static Axis3f rateDesired={0,0,0};
	 static Axis3f torque={0,0,0};
	 static bool armFlag;
	 static PidObject pidRoll;
	 static PidObject pidPitch;
	 static PidObject pidRollRate;
	 static PidObject pidPitchRate;
	 static PidObject pidYaw;
	 static PidObject pidYawRate;
	 
/*	Timer4 clock is 72MHz, which is twice APB1 frequency,APB1 frequency is 36M  
		arr is 	timer4 auto reload value
		psc is  timer4 clock prescaler 
		Timer4 overflow period (Tout) in us is 
		Tout= ((arr+1)) *(psc+1))/Tclk   
		Tclk is timer4 input frequency in MHz
*/

void TIM4_Ctrl_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //Enable timer4 clock as 2xAPB1
	
	/*Timer4 initialization*/
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //timer4 update interrupt enable

	//Set interrupt priority 
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  

	TIM_Cmd(TIM4, ENABLE);  //Enable timer 4				 
}

/*This ISR is to unblock the controller task*/
void TIM4_IRQHandler(void)
	{
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //if timer4 update interrupt happens 
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update); //clear timer4 update interrupt flag
			vTaskNotifyGiveFromISR(xTaskToNotify , &xHigherPriorityTaskWoken );	 //task notification to unblock the controller task
		}
		portEND_SWITCHING_ISR( xHigherPriorityTaskWoken ); //context switch to the task with the highest priority
	}
	
void xTaskController()
{ 
	 TickType_t xLastWakeTime;
		 //sensorUpdate(&rateActual,&angleActual);
	 pidInit(&pidRoll,ROLL_KP,ROLL_KI,ROLL_KD, ROLL_TAUF, INNER_LOOP_DELTAT_SECONDS*OUTER_LOOP_FREQUENCY_DIVIDER,ANGLAR_RATE_MIN,ANGLAR_RATE_MAX);
	 pidInit(&pidPitch,PITCH_KP,PITCH_KI,PITCH_KD,PITCH_TAUF, INNER_LOOP_DELTAT_SECONDS*OUTER_LOOP_FREQUENCY_DIVIDER,ANGLAR_RATE_MIN,ANGLAR_RATE_MAX);
   //pidInit(&pidYaw,YAW_KP,YAW_KI,INNER_LOOP_DELTAT_SECONDS*OUTER_LOOP_FREQUENCY_DIVIDER,ANGLAR_RATE_MIN,ANGLAR_RATE_MAX);
		
	 pidInit(&pidRollRate,ROLL_RATE_KP,ROLL_RATE_KI,ROLL_RATE_KD,ROLL_RATE_TAUF, INNER_LOOP_DELTAT_SECONDS,TORQUE_MIN,TORQUE_MAX);
	 pidInit(&pidPitchRate,PITCH_RATE_KP,PITCH_RATE_KI,PITCH_RATE_KD,PITCH_RATE_TAUF, INNER_LOOP_DELTAT_SECONDS,TORQUE_MIN,TORQUE_MAX);
	 pidInit(&pidYawRate,YAW_RATE_KP,YAW_RATE_KI,YAW_RATE_KD,PITCH_RATE_TAUF,INNER_LOOP_DELTAT_SECONDS,TORQUE_MIN,TORQUE_MAX);
	
	xTaskToNotify = xTaskGetCurrentTaskHandle();
	while(1)
	{	
		IMUUpdate(&rateActual,&angleActual);
		updateRC(&angleDesired,&desiredThrust,&armFlag);
		desiredThrustCom=desiredThrust/(cos(angleActual.x)*cos(angleActual.y));						
		rateDesired.x=PID(&pidRoll,angleDesired.x,angleActual.x);
		rateDesired.y=PID(&pidPitch,angleDesired.y,angleActual.y); 		
						
		torque.x=PID(&pidRollRate,rateDesired.x,rateActual.x);		
		torque.y=PID(&pidPitchRate,rateDesired.y,rateActual.y);		
		torque.z=PID(&pidYawRate,rateDesired.z,rateActual.z);		
													
		 rotorPwm(&pwm1Ratio,&pwm2Ratio,&pwm3Ratio,&pwm4Ratio,desiredThrustCom,&torque);
										
		 #ifdef ROTOR_OFF	
			pwm1(0);
			pwm2(0);
			pwm3(0);
			pwm4(0);				
		 #else
			pwm1(pwm1Ratio);
			pwm3(pwm3Ratio);
			pwm2(pwm2Ratio);
			pwm4(pwm4Ratio);						
		 #endif		
    ulTaskNotifyTake( pdTRUE, portMAX_DELAY);				 
	}
}
	