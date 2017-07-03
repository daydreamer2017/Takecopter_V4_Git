#ifndef __SYS_INIT_H
#define __SYS_INIT_H
#include "sys.h"

/* Task priorities. */
#define CONTROLLER_PRIORITY				( tskIDLE_PRIORITY + 3 )
#define IMU_DECODER_PRIORITY				( tskIDLE_PRIORITY + 2 )
#define RECEIVER_PRIORITY				( tskIDLE_PRIORITY + 2 )

#define TASK_STACK_SIZE			(configMINIMAL_STACK_SIZE+50)

//the timers' default timer interval is in micro seconds
#define MICRO_SECONDS_CONVERSION 0.000001
#define MILLI_SECONDS_CONVERSION 0.001

#define OUTER_LOOP_FREQUENCY_DIVIDER 1 // outer-loop is 10 times slower than inner-loop
//#define DEBUG_FREQUENCY_DIVIDER 2     // serial port debug data is displayed every 10 samples
#define INNER_LOOP_DELTAT 10000 //10000us or 10ms/100Hz the inner-loop interval is determined by the sensor's updating rate
#define OUTER_LOOP_DELTAT INNER_LOOP_DELTAT*OUTER_LOOP_FREQUENCY_DIVIDER //10000us or 10ms/100Hz the outer-loop is running at the same frequency as the inner-loop
#define DATA_LOGGING_DELTAT_SECONDS INNER_LOOP_DELTAT*DEBUG_FREQUENCY_DIVIDER*MICRO_SECONDS_CONVERSION //data logging interval 0.5s
#define OUTER_LOOP_DELTAT_SECONDS OUTER_LOOP_DELTAT *MICRO_SECONDS_CONVERSION
#define INNER_LOOP_DELTAT_SECONDS INNER_LOOP_DELTAT *MICRO_SECONDS_CONVERSION
#define DEFAULT_PWM 0.15 //motor PWM when arming
#define HOVOR_PWM 0.25   //thrust feedforward when hovering




// #define RATE_P_ANGLE_P
// #define RATE_P_ANGLE_PI
// #define RATE_P_ANGLE_PD
// #define RATE_P_ANGLE_PID

// #define RATE_PD_ANGLE_P
// #define RATE_PD_ANGLE_PI
// #define RATE_PD_ANGLE_PD
#define RATE_PD_ANGLE_PID

// #define RATE_PID_ANGLE_P
// #define RATE_PID_ANGLE_PI
// #define RATE_PID_ANGLE_PD
// #define RATE_PID_ANGLE_PID
void sysInit(u16 DeltaT);
#endif  