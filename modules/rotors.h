#ifndef __ROTORS_H
#define __ROTORS_H

#include "stm32f10x.h"
#include "types.h"
#include "pwm.h"

#define VOLT_MAX 3.9 //v
#define VOLT_MIN 0 //v
void rotorPwm(float *pwm1Ratio,float *pwm2Ratio,float *pwm3Ratio,float *pwm4Ratio,float thrust,Axis3f *torque);
#endif

