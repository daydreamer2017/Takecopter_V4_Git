// #include "usart.h"
// #include "string.h"
// #include "delay.h"
// #include "types.h"
// #include "sensor.h"

// //extern unsigned char Re_buf[11];

// void sensorUpdate(Axis3f *a,Axis3f *w,Axis3f *angle)
// {
// 			
// 				unsigned char sensorData[11]; 
// //         memcpy(sensorData,Re_buf,11);
//          //usartDone=0;
// 					getSensorData(sensorData);
//          if(sensorData[0]==0x55)       //���֡ͷ
//          {  
// 	
// // 					printf("%d \n",sensorData[1]);
//             switch(sensorData[1])
//             {
//                case 0x51: //��ʶ������Ǽ��ٶȰ�
//                   a->x = ((short)(sensorData[3]<<8 | sensorData[2]))/32768.0*16;      //X����ٶ�
//                   a->y = ((short)(sensorData[5]<<8 | sensorData[4]))/32768.0*16;      //Y����ٶ�
//                   a->z = ((short)(sensorData[7]<<8 | sensorData[6]))/32768.0*16;      //Z����ٶ�
//                   //T    = ((short)(sensorData[9]<<8 | sensorData[8]))/340.0+36.25;      //�¶�
//                   break;
//                case 0x52: //��ʶ������ǽ��ٶȰ�
//                   w->x = ((short)(sensorData[3]<<8| sensorData[2]))/32768.0*2000*3.14/180;      //X����ٶ�
//                   w->y = ((short)(sensorData[5]<<8| sensorData[4]))/32768.0*2000*3.14/180;      //Y����ٶ�
//                   w->z = ((short)(sensorData[7]<<8| sensorData[6]))/32768.0*2000*3.14/180;      //Z����ٶ�
//                   //T    = ((short)(sensorData[9]<<8| sensorData[8]))/340.0+36.25;      //�¶�
//                   break;
//                case 0x53: //��ʶ������ǽǶȰ�
//                   angle->x = ((short)(sensorData[3]<<8| sensorData[2]))/32768.0*3.14;   //X���ת�ǣ�x �ᣩ
//                   angle->y = ((short)(sensorData[5]<<8| sensorData[4]))/32768.0*3.14;   //Y�ḩ���ǣ�y �ᣩ
//                   angle->z = ((short)(sensorData[7]<<8| sensorData[6]))/32768.0*3.14;   //Z��ƫ���ǣ�z �ᣩ
//                   //T        = ((short)(sensorData[9]<<8| sensorData[8]))/340.0+36.25;   //�¶�

//                   //printf("X��Ƕȣ�%.2f   Y��Ƕȣ�%.2f   Z��Ƕȣ�%.2f\r\n",angle[0],angle[1],angle[2]);
//                   break;
//                default:  break;
//             }
// 							//printf("X�Ƕȣ�%.2f  Y�Ƕȣ�%.2f  Z�Ƕȣ�%.2f  X�ٶȣ�%.2f  Y�ٶȣ�%.2f  Z�ٶȣ�%.2f\r\n",angle->x,angle->y,angle->z,w->x,w->y,w->z);
//          }			 

// }
