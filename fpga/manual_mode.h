

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
		ae[3]=package[ROLL]*ROLLPITCHYAW_CONVERSION;
	}
	else {
		ae[1]=-(package[ROLL]-255)*NEG_ROLLPITCHYAW_CONVERSION;
	}

//PITCH
if(package[PITCH]<=127) {
		ae[0]=package[PITCH]*ROLLPITCHYAW_CONVERSION;
	}
	else {
		ae[2]= -(package[PITCH]-255)*NEG_ROLLPITCHYAW_CONVERSION;
	}

//YAW
if(package[YAW]<=127) {
	ae[0]+=package[YAW]*ROLLPITCHYAW_CONVERSION;
	ae[2]+= package[YAW]*ROLLPITCHYAW_CONVERSION;
}
else {
	ae[1]+=-(package[YAW]-255)*NEG_ROLLPITCHYAW_CONVERSION;
	ae[3]+=-(package[YAW]-255)*NEG_ROLLPITCHYAW_CONVERSION;
}
//LIFT
for(i = 0; i < 4; i++) {

	if(package[LIFT]<=75) {
			
		ae[i]+=package[LIFT]*LOW_LIFT_CONVERSION;
	}
	else {

		ae[i]+=(package[LIFT]-255)*HIGH_LIFT_CONVERSION+ENGINE_LIMIT;
	}

	
	if(ae[i]>ENGINE_LIMIT) {
			ae[i]=ENGINE_LIMIT;
		}

}
}

