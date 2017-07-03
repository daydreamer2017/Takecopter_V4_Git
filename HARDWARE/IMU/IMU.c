#include "IMU.h"	  

static void IMUTask();
static signed char IMUBuff;
static unsigned char Temp[11];
static unsigned char Re_buf[11],counter=0;
static Axis3f rate;
static Axis3f angle;

void IMUTaskInit(void)
{
  xTaskCreate(IMUTask, NULL, 500, NULL, 3, NULL);
}


 void IMUUpdate(Axis3f *updateRate,Axis3f *updateAngle)
	{
		*updateRate=rate;
		*updateAngle=angle;
	}

static void IMUTask()
{
	
			TickType_t xLastExecutionTime;
			const TickType_t xTicksToWait = pdMS_TO_TICKS( 100 );
			xLastExecutionTime = xTaskGetTickCount();
	while(1)
	{
   		 xSerialGetChar( 0, &IMUBuff, 0 );
			 Temp[counter] = IMUBuff;
			 if(counter == 0 && Temp[0] != 0x55)
			 counter=0;
			 else
			 counter++; 
		    if(counter==11) //���յ� 11 ������
				{ 
					 memcpy(Re_buf,Temp,11);
					 counter=0; //���¸�ֵ��׼����һ֡���ݵĽ���
					 if(Re_buf[0]==0x55)       //���֡ͷ
					 {  
									switch(Re_buf[1])
										{
											 case 0x52: //this pakage is angular velocity 
													rate.x = (float)((short)(Re_buf[3]<<8| Re_buf[2]))/32768.0*2000*3.14/180;      //x angular rate
													rate.y = (float)((short)(Re_buf[5]<<8| Re_buf[4]))/32768.0*2000*3.14/180;      //y angular rate
													rate.z = (float)((short)(Re_buf[7]<<8| Re_buf[6]))/32768.0*2000*3.14/180;      //z angular rate
													//flag=1;
													//T    = ((short)(Re_buf[9]<<8| Re_buf[8]))/340.0+36.25;      //�¶�
													break;
											 case 0x53: //this pakage is angule
													angle.x = (float)((short)(Re_buf[3]<<8| Re_buf[2]))/32768.0*3.14;   //x angle roll
													angle.y = (float)((short)(Re_buf[5]<<8| Re_buf[4]))/32768.0*3.14;   //y angle pitch
													angle.z = (float)((short)(Re_buf[7]<<8| Re_buf[6]))/32768.0*3.14;   //z angle yaw
													//T        = ((short)(Re_buf[9]<<8| Re_buf[8]))/340.0+36.25;   //�¶�
													//usartSign=1; 
													break;
											 default:  break;
										}
								//printf("X�Ƕȣ�%.2f  Y�Ƕȣ�%.2f  Z�Ƕȣ�%.2f  X�ٶȣ�%.2f  Y�ٶȣ�%.2f  Z�ٶȣ�%.2f\r\n",angle->x,angle->y,angle->z,w->x,w->y,w->z);
					 }
										
				}  	 
 		//printf("In IMU task\n");	
		//vTaskDelayUntil( &xLastExecutionTime, pdMS_TO_TICKS( 10 ) );
	}
}