#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include "FreeRTOS.h"
//#include "IMU.h"	
#include "types.h"

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收

void vUART3Init(u32 bound);
void IMUUpdate(Axis3f *rateP,Axis3f *angleP);
void xTaskIMUDecoder();
#endif


