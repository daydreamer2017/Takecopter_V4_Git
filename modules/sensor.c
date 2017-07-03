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
//          if(sensorData[0]==0x55)       //检查帧头
//          {  
// 	
// // 					printf("%d \n",sensorData[1]);
//             switch(sensorData[1])
//             {
//                case 0x51: //标识这个包是加速度包
//                   a->x = ((short)(sensorData[3]<<8 | sensorData[2]))/32768.0*16;      //X轴加速度
//                   a->y = ((short)(sensorData[5]<<8 | sensorData[4]))/32768.0*16;      //Y轴加速度
//                   a->z = ((short)(sensorData[7]<<8 | sensorData[6]))/32768.0*16;      //Z轴加速度
//                   //T    = ((short)(sensorData[9]<<8 | sensorData[8]))/340.0+36.25;      //温度
//                   break;
//                case 0x52: //标识这个包是角速度包
//                   w->x = ((short)(sensorData[3]<<8| sensorData[2]))/32768.0*2000*3.14/180;      //X轴角速度
//                   w->y = ((short)(sensorData[5]<<8| sensorData[4]))/32768.0*2000*3.14/180;      //Y轴角速度
//                   w->z = ((short)(sensorData[7]<<8| sensorData[6]))/32768.0*2000*3.14/180;      //Z轴角速度
//                   //T    = ((short)(sensorData[9]<<8| sensorData[8]))/340.0+36.25;      //温度
//                   break;
//                case 0x53: //标识这个包是角度包
//                   angle->x = ((short)(sensorData[3]<<8| sensorData[2]))/32768.0*3.14;   //X轴滚转角（x 轴）
//                   angle->y = ((short)(sensorData[5]<<8| sensorData[4]))/32768.0*3.14;   //Y轴俯仰角（y 轴）
//                   angle->z = ((short)(sensorData[7]<<8| sensorData[6]))/32768.0*3.14;   //Z轴偏航角（z 轴）
//                   //T        = ((short)(sensorData[9]<<8| sensorData[8]))/340.0+36.25;   //温度

//                   //printf("X轴角度：%.2f   Y轴角度：%.2f   Z轴角度：%.2f\r\n",angle[0],angle[1],angle[2]);
//                   break;
//                default:  break;
//             }
// 							//printf("X角度：%.2f  Y角度：%.2f  Z角度：%.2f  X速度：%.2f  Y速度：%.2f  Z速度：%.2f\r\n",angle->x,angle->y,angle->z,w->x,w->y,w->z);
//          }			 

// }
