#include "rotors.h"
#include "receiver.h"
#include "PID.h"
//#define TURN_PITCH
//#define TURN_ROLL
//#define TURN_YAW
#define TURN_ALL
#undef max
#define max(a,b)((a)>(b)?(a):(b))
#undef min
#define min(a,b)((a)<(b)?(a):(b))


static float constrain(float value, float minVal, float maxVal)
{
	return min(maxVal,max(minVal,value));
}


void rotorPwm(float *pwm1Ratio,float *pwm2Ratio,float *pwm3Ratio,float *pwm4Ratio,float thrust,Axis3f *torque)
{
		float v1;//motor 1 voltage in percentage
		float v2;//motor 2 voltage in percentage
		float v3;//motor 3 voltage in percentage
		float v4;//motor 4 voltage in percentage
// 		float limitedxTorque;
// 		float limitedyTorque;
// 		float limitedzTorque;

	
		
// 		limitedxTorque=constrain(torque->x,-0.4,0.4);
// 		limitedyTorque=constrain(torque->y,-0.4,0.4);
// 		limitedzTorque=constrain(torque->z,-0.4,0.4);
	
	
		#ifdef TURN_PITCH
		v1=1*thrust-torque->y;
		v2=0;
		v3=1*thrust+torque->y;
		v4=0;
		#endif
	
		#ifdef TURN_ROLL
		v1=0;
		v2=1*thrust-torque->x;
		v3=0;
		v4=1*thrust+torque->x;
		#endif
	
		#ifdef TURN_ALL
		v1=1*thrust-torque->y-torque->z+HOVOR_PWM;
		v2=1*thrust-torque->x+torque->z+HOVOR_PWM;
		v3=1*thrust+torque->y-torque->z+HOVOR_PWM;
		v4=1*thrust+torque->x+torque->z+HOVOR_PWM;
								
// 		v1=1*thrust-limitedyTorque-limitedzTorque;
// 		v2=1*thrust-limitedxTorque+limitedzTorque;
// 		v3=1*thrust+limitedyTorque-limitedzTorque;
// 		v4=1*thrust+limitedxTorque+limitedzTorque;
		#endif	
		*pwm1Ratio=constrain(v1,0,1);
		*pwm2Ratio=constrain(v2,0,1);
		*pwm3Ratio=constrain(v3,0,1);
		*pwm4Ratio=constrain(v4,0,1);
}

