#include "timer3_cap.h"
#include "misc.h"
#include "stm32f10x_tim.h"
#include "string.h"

void TIM3_Cap_Init(u16 arr,u16 psc)
{	
  TIM_ICInitTypeDef  TIM3_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);		
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);		
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);					
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);			

	/*Initialize timer 3*/
	TIM_TimeBaseStructure.TIM_Period = arr; //set counter auto reload value
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//prescaler
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //timer counter up
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
  
	/*initialize timer 3 ch1 capture parameters*/
		TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
  	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM3_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
    /*initialize timer 3 ch2 capture parameters*/
    TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=02? 	选择输入端 IC2映射到TI上
  	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI2上
  	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM3_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
		
    /*initialize timer 3 ch3 capture parameters*/
				TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=03? 	选择输入端 IC3映射到TI3上
  	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM3_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
		
    /*initialize timer 3 ch4 capture parameters*/
				TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=04? 	选择输入端 IC3映射到TI4上
  	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM3_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM3,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//允许CC1IE,CC2IE,CC3IE,CC4IE捕获中断	

	TIM_Cmd(TIM3,ENABLE );
}

static u8 TIM3CH1_ST=0; //ch1 TIM3CH1_ST=1 a rising edge has been captured
static u8 TIM3CH2_ST=0; //ch2 TIM3CH1_ST=1 a rising edge has been captured
static u8 TIM3CH3_ST=0; //ch3 TIM3CH1_ST=1 a rising edge has been captured
static u8 TIM3CH4_ST=0; //ch4 TIM3CH1_ST=1 a rising edge has been captured
static u8 ppmHeader=0;//PPM signal header

static s16 Temp[5];    // static temporary timer channel capture buffer
static s16 Cap_buf[5]; //static varialbe/capture buffer to store 4 channels' values
 
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)//channel 4 capture interrupt happens
		{	
			if(TIM3CH4_ST)		//if rising edge already captured		
			{	
				TIM3CH4_ST=0;			//reset rising edge capture status flag
				Temp[1]=TIM_GetCapture4(TIM3); //get ch4 positive signal width
				ppmHeader=1; //PPM frame header captured
			  TIM_OC4PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC4P=0 set ch4 as rising edge capture
			}else  								//rising edge not captured yet
			{
				TIM_SetCounter(TIM3,0); // reset timer 3 ch4 counter
				TIM3CH4_ST=1;		//set rising edge capture status flag true
				TIM_OC4PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC4P=2 set ch4 as falling edge capture 
			}		    
		}

	if(ppmHeader) //PPM signal ch1 has been detected
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
						{	
							if(TIM3CH3_ST)			
							{	
								TIM3CH3_ST=0;		
								Temp[2]=TIM_GetCapture3(TIM3);
								//Cap_buf[2]=Temp[2];
								TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Rising);
							}else  							
							{
								TIM_SetCounter(TIM3,0);
								TIM3CH3_ST=1;		
								TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Falling);	
							}		    
						}
				if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
						{	
							if(TIM3CH2_ST)		
							{	
								TIM3CH2_ST=0;		
								Temp[3]=TIM_GetCapture2(TIM3);
							  TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Rising);
							}else  			
							{
								TIM_SetCounter(TIM3,0);
								TIM3CH2_ST=1;	
								TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Falling);	
							}		    
						}
		if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
			{	
				if(TIM3CH1_ST)		
					{	
						TIM3CH1_ST=0;	
						ppmHeader=0;		
						    Temp[4]=TIM_GetCapture1(TIM3);
						    /*save capture PPM width to buffer*/
								Cap_buf[1]=Temp[1];
								Cap_buf[2]=Temp[2];
								Cap_buf[3]=Temp[3];
								Cap_buf[4]=Temp[4];
							  TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Rising);
					}
				else  								
					{
						TIM_SetCounter(TIM3,0);
						TIM3CH1_ST=1;		
						TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling);	
					}		    
			}					
	}		
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4); //clear all channel's interrupt flags
}
void getReceriverData(s16 receriverData[])
	{
		receriverData[1]=Cap_buf[1];
		receriverData[2]=Cap_buf[2];
		receriverData[3]=Cap_buf[3];
		receriverData[4]=Cap_buf[4];
	}
