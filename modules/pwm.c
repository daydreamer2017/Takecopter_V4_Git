#include "pwm.h"
#include "stm32f10x.h"
#include "timer2_pwm.h"
#include "stm32f10x_tim.h"
void pwm1(float pwm1Ratio)
{
		u16 temp;
// 		temp=(u16)(PWM_TICKS*(0.05566*(1+pwm1Ratio)));
			temp=(u16)(PWM_TICKS*(0.049*(1+pwm1Ratio)));

		TIM_SetCompare1(TIM2,temp);	
		//printf("temp%d",temp);
};

void pwm2(float pwm2Ratio)
{
		u16 temp;
			temp=(u16)(PWM_TICKS*(0.049*(1+pwm2Ratio)));
		TIM_SetCompare2(TIM2,temp);	
};

void pwm3(float pwm3Ratio)
{
		u16 temp;
					temp=(u16)(PWM_TICKS*(0.049*(1+pwm3Ratio)));
		TIM_SetCompare3(TIM2,temp);	
};

void pwm4(float pwm4Ratio)
{
		u16 temp;
		temp=(u16)(PWM_TICKS*(0.049*(1+pwm4Ratio)));
		TIM_SetCompare4(TIM2,temp);	
};
