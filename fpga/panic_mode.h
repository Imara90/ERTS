/*--------------
Moderate RPM's value to lift threshold and after few seconds enter safe mode

05-03-2014(Created by Diogo Monteiro)

*/

#define LIFT_THRESHOLD 500 //TODO Define this values experimentally
#define ENGINE_STEP 30


void panic_mode(void) {
	int i;
	int mean_thrust=0;	
	int reference_thrust = LIFT_THRESHOLD;	
	for(i = 0; i < 4; i++) mean_thrust += ae[i]/4;
	
	if (mean_thrust <= LIFT_THRESHOLD) reference_thrust = mean_thrust;
		
	for(i = 0; i < 4; i++) {
	
		if(ae[i]-reference_thrust <= ENGINE_STEP && ae[i]-reference_thrust >= -ENGINE_STEP) {
			ae[i] = reference_thrust;
		}
		else if (ae[i]-reference_thrust <= -ENGINE_STEP) {
			ae[i]+=	30;
		}
		else {
			ae[i]-=	30;
		}

	}
	
	if(ae[0] == reference_thrust && ae[1] == reference_thrust && ae[2] == reference_thrust && ae[3] == reference_thrust) {
		delay_ms(2000);//wait 2 seconds
		telemetry_flag = telemetry_flag | 0x02;
	}

}
