#include "timer2_pwm.h"
#include "misc.h"
#include "stm32f10x_tim.h"
#include "string.h"

// //TIM2 PWM部分初始化 
// //PWM输出初始化
// //arr：自动重装值
// //psc：时钟预分频数
void TIM2_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能定时器2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIO PA外设时钟
	
   //设置该引脚为复用输出功能,输出TIM2 CH1的PWM脉冲波形	GPIOA.0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO PA0
	
	  //设置该引脚为复用输出功能,输出TIM2 CH2的PWM脉冲波形	GPIOA.1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO PA1
	
		  //设置该引脚为复用输出功能,输出TIM2 CH3的PWM脉冲波形	GPIOA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //TIM_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO PA2
	
		  //设置该引脚为复用输出功能,输出TIM2 CH4的PWM脉冲波形	GPIOA.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //TIM_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO PA3
	
 
   //初始化TIM2
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM2 Channel 1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC1

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR1上的预装载寄存器
	
	
		//初始化TIM2 Channel 2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC2

	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
	
	
			//初始化TIM2 Channel 3 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC3

	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR3上的预装载寄存器
	
	
			//初始化TIM2 Channel 4 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC4

	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR4上的预装载寄存器
 
	TIM_Cmd(TIM2, ENABLE);  //使能TIM2
	TIM_SetCompare1(TIM2,arr);
	TIM_SetCompare2(TIM2,arr);
	TIM_SetCompare3(TIM2,arr);
	TIM_SetCompare4(TIM2,arr);
}
