#ifndef __RECEIVER_H
#define __RECEIVER_H
#include "pwm.h"
#include "types.h"
#include "stm32f10x_tim.h"
#include "types.h"
#include "sys_init.h"
#include <stdbool.h>

    #define RC_EXP  //exponential RC control
		#define EXP_FACTOR_THRUST 0.4
		#define EXP_FACTOR_ANGLE 0.7

		#define PPM_PERIOD 21200 //ppm signal maximum length us
		#define PPM_PERIOD_MILLISECONDS PPM_PERIOD*MILLI_SECONDS_CONVERSION
		#define PPM_MAX 1840 //ppm signal maximum length us
		#define PPM_MIN 1120 //ppm signal minimum length	us
		
// 		#define PPM_MAX_THRUST 4265 //ppm signal maximum length us for ch3 thrust
// 		#define PPM_MIN_THRUST 3465 //ppm signal minimum length	us for ch3 thrust
		
		#define PPM_NEUTURAL 1520//ppm sinal neutural point	us
		#define PPM_LENGTH 800//ppm sinal total length	us
		#define PPM_LENGTH_HF 400//ppm sinal half length	us
		
		#define SYS_CLC 72000//khz
		#define PWM_PRESCALER 99 //PWM PRESCALER 100
		#define PWM_PRESCALED_FREQUENCY (SYS_CLC/(PWM_PRESCALER+1)) // PWM prescaled timer FREQUENCY in khz
		
		#define PWM_TICKS (PPM_PERIOD_MILLISECONDS*PWM_PRESCALED_FREQUENCY) //number of ticks in one pwm period
		
		//#define PWM_FREQUENCY 8 //khz
		//#define PWM_LEN 899 //number of ticks in one pwm period	
		
		#define ROLL_MAX 0.523 //maximum desired roll angle rad
		#define ROLL_MIN -0.523 //minimum desired roll angle rad
		#define PITCH_MAX 0.523 //maximum desired pitch angle rad
		#define PITCH_MIN -0.523 //minimum desired pitch angle rad
			
		#define YAW_RATE_MAX 1.5 //maximum desired yaw anglar rate rad/s
		#define YAW_RATE_MIN -1.5 //minimum desired yaw anglar rate rad/s
		#define THRUST_MAX 1 //1 is 100% and 0 is 0%
		#define THRUST_MIN 0 //
		//#define THRUST_PERCENTAGE 0.7//
		//#define PWM_PERIOD 800

void xTaskReceiver();
float expRC(float input,float factor);
void updateRC(Axis3f *RCReference,float *desiredThrust,bool *armed);
void TIM3_Cap_Init(u16 arr,u16 psc);
void getReceriverData(s16 receriverData[]);
#endif


