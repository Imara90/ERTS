

#define ENGINE_LIMIT 800
#define LOW_LIFT_CONVERSION 450/75
#define HIGH_LIFT_CONVERSION (ENGINE_LIMIT-450)/(255-75)

#define ROLLPITCHYAW_CONVERSION ENGINE_LIMIT/127
#define NEG_ROLLPITCHYAW_CONVERSION ENGINE_LIMIT/126
void manual_mode() {
int i;
//INITIALIZATION
for (i = 0; i < 4; i++)
{
	ae[i] = 0;
}

//ROLL
	if(roll<=127) {
		ae[3]=roll*ROLLPITCHYAW_CONVERSION;
	}
	else {
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

	if(lift<=75) {
			
		ae[i]+=lift*LOW_LIFT_CONVERSION;
	}
	else {

		ae[i]+=(lift-255)*HIGH_LIFT_CONVERSION+ENGINE_LIMIT;
	}

	
	if(ae[i]>ENGINE_LIMIT) {
			ae[i]=ENGINE_LIMIT;
		}

}
}

