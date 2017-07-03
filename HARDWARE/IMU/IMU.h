#include "usart.h"	  
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "queue.h"

 typedef struct {
         float x;
         float y;
         float z;
 } Axis3f;
 
 void IMUUpdate(Axis3f *updateRate,Axis3f *updateAngle);
 void IMUTaskInit(void);
