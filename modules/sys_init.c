/* Standard includes. */
#include "string.h"
/* STM32F10 Library includes. */
#include "misc.h"
#include "stm32f10x_tim.h"
/* Application includes. */
#include "att_controller.h"
#include "USART3_IMU.h"	  
#include "receiver.h"		
#include "PID.h"
#include "rotors.h"
#include "timer2_pwm.h"	

void sysInit(u16 DeltaT)
{
	 //DeltaT: inner-loop sampling interval in microsecond
	 //u16 arr;
	 //delay_init();
 	 NVIC_Configuration(); 
	 vUART3Init(115200); //UART3 initialization
	 TIM3_Cap_Init(0XFFFF,72-1);	//以1Mhz的频率计数 
	 TIM2_PWM_Init(PWM_TICKS,PWM_PRESCALER);	 //no frequency division PWM frequency=72000/900=8Khz	 
	 TIM4_Ctrl_Init(100,7199);    //100Hz
		pwm1(0);
		pwm2(0);
		pwm3(0);
		pwm4(0);

}
