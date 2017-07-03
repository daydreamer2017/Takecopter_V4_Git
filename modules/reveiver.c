/* Standard includes. */
#include "math.h"
#include <stdbool.h>
#include "string.h"
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* STM32F10 Library includes. */
#include "misc.h"
#include "stm32f10x_tim.h"

/* Application includes. */
#include "receiver.h"		
#include "timer3_cap.h"		

static TaskHandle_t xTaskToNotify; //task handle
static Axis3f receiverData; //
static float Thrust;       //
static bool armFlag=0;     //

float expRC(float input,float factor)
{
	float output=((1-factor)*powf(input,3))+(factor*input);
	//output = ( (1 - factor) x input3 ) + ( factor x input )
	return output;
}

/*RC receiver PPM decoder tasks*/
void xTaskReceiver()
{			
		s16	receriverData[5];	// receriverData[0] not used
		float ref_temp[5]; //  RC receiver data buffer
		xTaskToNotify = xTaskGetCurrentTaskHandle();
		while(1)
		{
				getReceriverData(receriverData); //get raw data from RC receiver
				ref_temp[4]=(float)(receriverData[4]-PPM_NEUTURAL)/PPM_LENGTH_HF*ROLL_MAX;
				ref_temp[2]=(float)(receriverData[2]-PPM_NEUTURAL)/PPM_LENGTH_HF*PITCH_MAX;
				ref_temp[1]=(float)(receriverData[1]-PPM_NEUTURAL)/PPM_LENGTH_HF*YAW_RATE_MAX;
				ref_temp[3]=(float)(receriverData[3]-PPM_MIN)/PPM_LENGTH*THRUST_MAX;

				if(ref_temp[4]>ROLL_MAX)
					ref_temp[4]=ROLL_MAX; 
				if(ref_temp[4]<ROLL_MIN)
					ref_temp[4]=ROLL_MIN;
				
				if(ref_temp[2]>PITCH_MAX)
					ref_temp[2]=PITCH_MAX;
				if(ref_temp[2]<PITCH_MIN)
					ref_temp[2]=PITCH_MIN;
				
				if(ref_temp[1]>YAW_RATE_MAX)
					ref_temp[1]=YAW_RATE_MAX;
				if(ref_temp[1]<YAW_RATE_MIN)
				ref_temp[1]=YAW_RATE_MIN;
				
				if(ref_temp[3]>THRUST_MAX)
				ref_temp[3]=THRUST_MAX;
				if(ref_temp[3]<THRUST_MIN)
				ref_temp[3]=THRUST_MIN;
				
				#ifdef RC_EXP		
							receiverData.x=expRC(ref_temp[4],EXP_FACTOR_ANGLE);
							receiverData.y=expRC(ref_temp[2],EXP_FACTOR_ANGLE);
							receiverData.z=expRC(ref_temp[1],EXP_FACTOR_ANGLE);
							Thrust=expRC(ref_temp[3],EXP_FACTOR_THRUST);
				#else
							receiverData.x=ref_temp[4];
							receiverData.y=ref_temp[2];
							receiverData.z=ref_temp[1];
							Thrust=ref_temp[3];
				#endif		
				if((Thrust<(0.1*THRUST_MAX))&&(receiverData.x<0.6*ROLL_MIN)&&(receiverData.y<0.6*PITCH_MIN)&&(receiverData.z>0.6*YAW_RATE_MAX)) armFlag=1;
				if((Thrust<(0.1*THRUST_MAX))&&(receiverData.x>0.6*ROLL_MAX)&&(receiverData.y<0.6*PITCH_MIN)&&(receiverData.z<0.6*YAW_RATE_MIN)) armFlag=0;
				ulTaskNotifyTake( pdTRUE, portMAX_DELAY);						
		}	
}
void updateRC(Axis3f *RCReference,float *desiredThrust,bool *armed)
{
		RCReference->x=receiverData.x;
		RCReference->y=receiverData.y;
		RCReference->z=receiverData.z;
		*desiredThrust=Thrust;
		*armed=armFlag;
}

/*Timer 3 initialization*/
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
	TIM_TimeBaseStructure.TIM_Period = arr; //set Auto-Reload Register period value r at the next update event
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//prescaler value used to divide the TIM clock
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //TDTS = Tck_tim 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM Upcounting mode
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
  
	/*initialize timer 3 ch1 capture parameters*/
		TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	TIM Channel 1 is used
  	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//The active edge is the TIM Input Capture rising edge
  	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //TIM Input 1, 2, 3 or 4 is selected to be connected to IC1, IC2, IC3 or IC4, respectively.
  	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //Capture performed each time an edge is detected on the capture input.
  	TIM3_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 no filter
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure); //initialize timer 3 ch1 capture parameters
	
    /*initialize timer 3 ch2 capture parameters*/
    TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2; 
  	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	
  	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
  	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 
  	TIM3_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
		
    /*initialize timer 3 ch3 capture parameters*/
				TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=03
  	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	
  	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
  	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 
  	TIM3_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
		
    /*initialize timer 3 ch4 capture parameters*/
				TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=04
  	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	
  	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
  	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  	TIM3_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
	
  // Initialize NVIC 
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3 interrupt
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //set pre-emption priority 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //, subpriority 0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ enable
	NVIC_Init(&NVIC_InitStructure);  // Initializes the NVIC peripheral according to the parameters specified in the NVIC_InitStruct
	TIM_ITConfig(TIM3,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);//enable TIM3 ch1 ch2 ch3 ch4 Capture/Compare Interrupt
	TIM_Cmd(TIM3,ENABLE );
}

static u8 TIM3CH1_ST=0; //ch1 TIM3CH1_ST=1 a rising edge has been captured
static u8 TIM3CH2_ST=0; //ch2 TIM3CH1_ST=1 a rising edge has been captured
static u8 TIM3CH3_ST=0; //ch3 TIM3CH1_ST=1 a rising edge has been captured
static u8 TIM3CH4_ST=0; //ch4 TIM3CH1_ST=1 a rising edge has been captured
static u8 ppmHeader=0;//PPM signal header

static s16 Temp[5];    // static temporary timer channel capture buffer
static s16 Cap_buf[5]; //static varialbe/capture buffer to store 4 channels' values

/*Timer3 ISR record the PPM channels' width
  Once all PPM channel signals detected, the ISR unblock receiver decoder task
*/
void TIM3_IRQHandler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)// Checks whether the TIM3 cc4 interrupt has occurred
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
		if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)  // Checks whether the TIM3 cc3 interrupt has occurred
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
						    vTaskNotifyGiveFromISR(xTaskToNotify , &xHigherPriorityTaskWoken );	//unblock RC receiver decoder task to process the PPM signals
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
	  portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}
void getReceriverData(s16 receriverData[])
	{
		receriverData[1]=Cap_buf[1];
		receriverData[2]=Cap_buf[2];
		receriverData[3]=Cap_buf[3];
		receriverData[4]=Cap_buf[4];
	}

		
		
		
	
		