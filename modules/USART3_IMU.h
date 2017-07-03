#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include "FreeRTOS.h"
//#include "IMU.h"	
#include "types.h"

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

void vUART3Init(u32 bound);
void IMUUpdate(Axis3f *rateP,Axis3f *angleP);
void xTaskIMUDecoder();
#endif


