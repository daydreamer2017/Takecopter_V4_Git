  #ifndef __PID_H
#define __PID_H	
		#define ANGLE_ERROR_MAX 6
		#define ROLL_KP 4
		#define ROLL_KI 4.5
    #define ROLL_KD 0.034
		#define ROLL_TAUF 0.1*ROLL_KD/ROLL_KP
		
		#define PITCH_KP 4
		#define PITCH_KI 4.5
		#define PITCH_KD 0.034
		#define PITCH_TAUF 0.1*PITCH_KD/PITCH_KP

// 		#define YAW_KP 2
// 		#define YAW_KI 1
// 		#define YAW_KD 0
		
		//#define ROLL_KP 2.4745
	  //#define ROLL_KI 3.0625

		//#define PITCH_KP 2.4745
		//#define PITCH_KI 3.0625
			

		
		#define ROLL_RATE_KP 0.04
		#define ROLL_RATE_KI 0.0
		#define ROLL_RATE_KD 0.0002
		#define ROLL_RATE_TAUF 0.1*ROLL_RATE_KD/ROLL_RATE_KP


		#define PITCH_RATE_KP 0.04
		#define PITCH_RATE_KI 0.0
		#define PITCH_RATE_KD 0.0002
		#define PITCH_RATE_TAUF 0.1*PITCH_RATE_KD/PITCH_RATE_KP


		#define YAW_RATE_KP 0.2
		#define YAW_RATE_KI 0.2
		#define YAW_RATE_KD 0.005
		#define YAW_RATE_TAUF 0.1*YAW_RATE_KD/YAW_RATE_KP

		
//			#define INTEGRAL_RATIO 0.6
// 			#define TORQUE_MAX 0.0065	
// 			#define TORQUE_MIN -0.0065		
// 			
// 			#define TORQUE_I_MAX 0.2	//integral upper limitation
// 			#define TORQUE_I_MIN -0.2	//integral lower limitation
			
 			#define ANGLAR_RATE_MAX 2
 			#define ANGLAR_RATE_MIN -2
			
// 			#define ANGLAR_RATE_I_MAX INTEGRAL_RATIO*ANGLAR_RATE_MAX
// 			#define ANGLAR_RATE_I_MIN INTEGRAL_RATIO*ANGLAR_RATE_MIN

		#define TORQUE_MAX 0.3	
		#define TORQUE_MIN -0.3	
// 		
// 		#define TORQUE_Y_MAX 1	
// 		#define TORQUE_Y_MIN -1	
// 		
// 		#define TORQUE_Z_MAX 1	
// 		#define TORQUE_Z_MIN -1	

typedef struct
{
	float kp;
	float ki;
	float kd;
	float tauf;
	float uPast;
	float udPast;
	float yPast;
	float rPast;
	float deltat;
	float lowerLimit;
	float upperLimit;
}PidObject;

// float PI_M(PidObject*pid,float rCurrent,float yCurrent);//modified velocity form PI
float PI(PidObject*pid,float rCurrent,float yCurrent);//normal velocity PI
float PID(PidObject*pid,float rCurrent,float yCurrent);
float P(PidObject*pid,float rCurrent,float yCurrent);//normal Proportional control
void pidInit(PidObject*pid, float kp,float ki,float kd, float tauf, float deltat, float lowerLimit,float upperLimit);
void pidReset(PidObject*pid);
#endif
