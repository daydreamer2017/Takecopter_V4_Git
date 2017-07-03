/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
/* Application includes. */
#include "sys.h"
#include "USART3_IMU.h"	  

 /* Redirect printf output to USART3*/
#pragma import(__use_no_semihosting)             
               
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
_sys_exit(int x) 
{ 
	x = x; 
} 
//redirect fputc to uart 
int fputc(int ch, FILE *f)
{  
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); 
  USART_SendData(USART3,(uint8_t)ch);   
	return ch;
}

static TaskHandle_t xTaskToNotify;

static signed char IMUBuff;
static unsigned char Temp[11];
static unsigned char Re_buf[11],counter=0;
static Axis3f rate;
static Axis3f angle;

/*UART3 initialization*/
void vUART3Init(u32 BaudRate)  //specify Baud rate
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
 	USART_DeInit(USART3);  //¸´Î»´®¿Ú1
 //USART3_TX   PB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
 
	//USART3_RX	  PB.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  

	USART_InitStructure.USART_BaudRate = BaudRate;//
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART3, &USART_InitStructure); 

  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	
   
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART3, ENABLE);                    
}


void IMUUpdate(Axis3f *angleP,Axis3f *rateP)
{
*rateP=rate;
*angleP=angle;
}


/*UART3 RX interrupt collect IMU data and then defer IMU decoding to IMU decoder task*/
void USART3_IRQHandler(void)                	//UART3 ISR
	{
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		if( USART_GetITStatus( USART3, USART_IT_RXNE ) == SET )  //if UART RX interrupt happens
		{
			Temp[counter] = USART_ReceiveData( USART3 ); //read data;
			if(counter == 0 && Temp[0] != 0x55)  //if the first byte is not frame header
			counter=0;                           //drop this byte and reset the counter
			else
			counter++; 													//prepare receiving next byte
			if(counter==11)											//11 bytes/one data frame received
			vTaskNotifyGiveFromISR(xTaskToNotify , &xHigherPriorityTaskWoken );	//unblock decoder task to process the data package
		}		
		portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	} 


/* IMU decoder task to convert raw IMU data angle and speed */
void xTaskIMUDecoder()
{
	      xTaskToNotify = xTaskGetCurrentTaskHandle(); //calling task's handle
    		while(1)
				{
					if(counter==11) // recheck 11 bytes received
						{ 
							 memcpy(Re_buf,Temp,11); //copy received data to RX buffer
							 counter=0; //reset counter to prepare receiving next frame
							 if(Re_buf[0]==0x55)  //recheck the fist byte is the frame header      
							 {  
											switch(Re_buf[1]) //the second byte shows the frame type
												{
													 case 0x52: //this pakage is angular velocity 
															rate.x = (float)((short)(Re_buf[3]<<8| Re_buf[2]))/32768.0*2000*3.14/180;      //x angular rate
															rate.y = (float)((short)(Re_buf[5]<<8| Re_buf[4]))/32768.0*2000*3.14/180;      //y angular rate
															rate.z = (float)((short)(Re_buf[7]<<8| Re_buf[6]))/32768.0*2000*3.14/180;      //z angular rate															
															break;
													 case 0x53: //this pakage is angule
															angle.x = (float)((short)(Re_buf[3]<<8| Re_buf[2]))/32768.0*3.14;   //x angle roll
															angle.y = (float)((short)(Re_buf[5]<<8| Re_buf[4]))/32768.0*3.14;   //y angle pitch
															angle.z = (float)((short)(Re_buf[7]<<8| Re_buf[6]))/32768.0*3.14;   //z angle yaw
															break;
													 default:  break;
												}
							 }					
						}
						ulTaskNotifyTake( pdTRUE, portMAX_DELAY);						
				}
}
