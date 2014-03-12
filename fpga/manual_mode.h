#define ENGINE_LIMIT 800
#define LIFT_CONVERSION ENGINE_LIMIT/254
#define NEG_LIFT_CONVERSION ENGINE_LIMIT/252
#define LIFT_OFFSET ENGINE_LIMIT/2
#define ROLLPITCHYAW_CONVERSION ENGINE_LIMIT/127
#define NEG_ROLLPITCHYAW_CONVERSION ENGINE_LIMIT/126

void manual_mode() 
{
	int i;
	//INITIALIZATION
	for (i = 0; i < 4; i++)
		{
			ae[i] = 0;
		}

	//ROLL
	if(roll<=127) 
	{
		ae[3]=roll*ROLLPITCHYAW_CONVERSION;
	}
	else 
	{
		ae[1]=-(roll-255)*NEG_ROLLPITCHYAW_CONVERSION;
	}

	//PITCH
	if(pitch<=127) {
		ae[0]=pitch*ROLLPITCHYAW_CONVERSION;
	}
	else {
		ae[2]= -(pitch-255)*NEG_ROLLPITCHYAW_CONVERSION;
	}

	//YAW
	if(yaw<=127) {
		ae[0]+=yaw*ROLLPITCHYAW_CONVERSION;
		ae[2]+= yaw*ROLLPITCHYAW_CONVERSION;
	}
	else {
		ae[1]+=-(yaw-255)*NEG_ROLLPITCHYAW_CONVERSION;
		ae[3]+=-(yaw-255)*NEG_ROLLPITCHYAW_CONVERSION;
	}
	//LIFT
	for(i = 0; i < 4; i++) {

		if(lift<=127) 
		{
			ae[i]+=lift*LIFT_CONVERSION+LIFT_OFFSET;
		}
		else 
		{
			ae[i]+=(lift-129)*NEG_LIFT_CONVERSION;
		}
		if(ae[i]>ENGINE_LIMIT) 
		{
			ae[i]=ENGINE_LIMIT;
		}
	}
}

