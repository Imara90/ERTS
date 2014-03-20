/*--------------
Manual Mode

Last update: 17-03-2014(Created by Diogo Monteiro)

*/

#define LIFT_ENGINE_LIMIT 800
#define LOW_LIFT_CONVERSION 450/75
#define HIGH_LIFT_CONVERSION (LIFT_ENGINE_LIMIT-450)/(255-75)


#define ROLLPITCHYAW_ENGINE_LIMIT 50
#define ROLLPITCHYAW_CONVERSION ROLLPITCHYAW_ENGINE_LIMIT/127
#define NEG_ROLLPITCHYAW_CONVERSION ROLLPITCHYAW_ENGINE_LIMIT/126
void manual_mode() {
int i;
	//LIFT
	for(i = 0; i < 4; i++) {

		if(package[LIFT]<=75) {
			
			ae[i] = package[LIFT]*LOW_LIFT_CONVERSION;
		}
		else {

			ae[i] = (package[LIFT]-255)*HIGH_LIFT_CONVERSION+LIFT_ENGINE_LIMIT;
		}
	}

	//ROLL
		if(package[ROLL]<=127) {
			ae[3] += package[ROLL]*ROLLPITCHYAW_CONVERSION;
		}
		else {
			ae[1] += -(package[ROLL]-255)*NEG_ROLLPITCHYAW_CONVERSION;
		}

	//PITCH
	if(package[PITCH]<=127) {
			ae[0] += package[PITCH]*ROLLPITCHYAW_CONVERSION;
		}
		else {
			ae[2] += -(package[PITCH]-255)*NEG_ROLLPITCHYAW_CONVERSION;
		}

	//YAW
	if(package[YAW]<=127) {
		ae[0] += package[YAW]*ROLLPITCHYAW_CONVERSION;
		ae[2] += package[YAW]*ROLLPITCHYAW_CONVERSION;
		
		ae[1] -= package[YAW]*ROLLPITCHYAW_CONVERSION;
		ae[3]	-= package[YAW]*ROLLPITCHYAW_CONVERSION;	
	}
	else {
		ae[1] += -(package[YAW]-255)*NEG_ROLLPITCHYAW_CONVERSION;
		ae[3] += -(package[YAW]-255)*NEG_ROLLPITCHYAW_CONVERSION;
		
		ae[0] -= -(package[YAW]-255)*NEG_ROLLPITCHYAW_CONVERSION;
		ae[2] -= -(package[YAW]-255)*NEG_ROLLPITCHYAW_CONVERSION;	
	}

}

