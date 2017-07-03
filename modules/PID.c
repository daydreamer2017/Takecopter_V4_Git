#include "PID.h"

void pidInit(PidObject*pid, float kp,float ki,float kd, float tauf, float deltat, float lowerLimit,float upperLimit)
{
  pid->kp=kp; //proportional gain Kc
	pid->ki=ki; //integral gain Kc/tauI
	pid->kd=kd;
		pid->tauf=tauf;

	pid->uPast=0; //previous control signal
	pid->udPast=0;
	pid->yPast=0; //previous feedback signal
	pid->rPast=0; //previous reference signal
	pid->deltat=deltat; //sampling interval
	pid->lowerLimit=lowerLimit; //control signal lower limitation
	pid->upperLimit=upperLimit; //control signal upper limitation
};

void pidReset(PidObject*pid)
{

	pid->uPast=0; //previous control signal
	pid->udPast=0;
	pid->yPast=0; //previous feedback signal
	pid->rPast=0; //previous reference signal	
};

float PID(PidObject*pid,float rCurrent,float yCurrent)
{
	float uCurrent; //new control signal
	float udCurrent; //new control signal
    udCurrent=pid->tauf/(pid->tauf+pid->deltat)*pid->udPast+pid->kd/(pid->tauf+pid->deltat)*(yCurrent-pid->yPast);


		//uCurrent=pid->uPast+pid-> kp*(-yCurrent+pid->yPast)+(pid->ki*pid->deltat)*(rCurrent-yCurrent)-udCurrent+pid->udPast; //less overshot velocity form PID
	 uCurrent=pid->uPast+pid-> kp*(rCurrent-yCurrent-pid->rPast+pid->yPast)+(pid->ki*pid->deltat)*(rCurrent-yCurrent)-udCurrent+pid->udPast;; //velocity form PID

	if(uCurrent>pid->upperLimit)
		{
			uCurrent=pid->upperLimit;
		}
		else if (uCurrent<pid->lowerLimit)
		{
			uCurrent=pid->lowerLimit;
		}
	pid->uPast=uCurrent;
	pid->yPast=yCurrent;
  pid->rPast=rCurrent;
	pid->udPast=udCurrent;
  return uCurrent;
};


float PI(PidObject*pid,float rCurrent,float yCurrent)//normal velocity PI
{
	float uCurrent; //new control signal
	uCurrent=pid->uPast+pid-> kp*(rCurrent-yCurrent-pid->rPast+pid->yPast)+(pid->ki*pid->deltat)*(rCurrent-yCurrent); //velocity form PID
//	uCurrent=pid->uPast+pid-> kp*(-yCurrent+pid->yPast)+(pid->ki*pid->deltat)*(rCurrent-yCurrent); //less overshot velocity form PID

	if(uCurrent>pid->upperLimit)
		{
			uCurrent=pid->upperLimit;
		}
		else if (uCurrent<pid->lowerLimit)
		{
			uCurrent=pid->lowerLimit;
		}
	pid->uPast=uCurrent;
	pid->yPast=yCurrent;
  pid->rPast=rCurrent;
	
  return uCurrent;
};

float P(PidObject*pid,float rCurrent,float yCurrent)//normal Proportional control
{
	float uCurrent; //new control signal
	uCurrent=pid->kp*(rCurrent-yCurrent);
  return uCurrent;
};
