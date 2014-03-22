/*--------------
FULL_CONTROL_MODE

Generates actuator inputs for the full control mode.

19-03-2014(Created by Diogo Monteiro)

*/

#define MAX_ROLLPITCH 10000 //??? -  TODO define experimentally
#define ROLLPITCH_SCALING	MAX_ROLLPITCH/127 

void full_control_mode(void) {
	int i;
	
	//LIFT FORCE
	if (package[LIFT]<=75) {
		Z = package[LIFT]*LOW_Z_CONVERSION;
	}	
	else {
		Z = (package[LIFT]-255)*HIGH_Z_CONVERSION+MAX_Z; 
	}	
	//ROLL MOMENTUM
	if (package[ROLL] <= 127) {
		L = p1control*(package[ROLL]*ROLLPITCH_SCALING - phi);
	}
	else {
		L = p1control*((package[ROLL]-255)*ROLLPITCH_SCALING - phi);
	}
	L -= p2control*p; //Derivative Controller
	
	//PITCH MOMENTUM
	if (package[PITCH] <= 127) {
		M = p1control*(package[ROLL]*ROLLPITCH_SCALING - theta);
	}
	else {
		M = p1control*((package[PITCH] - 255)*ROLLPITCH_SCALING - theta);
	}		
	M -= p2control*q; //Derivative Controller

	//YAW MOMENTUM
	if (package[YAW] <= 127) {
		N = pcontrol*package[YAW]*YAWRATE_SCALING - pcontrol*r;
	}			
	else {
		N = pcontrol*(package[YAW] - 255)*YAWRATE_SCALING - pcontrol*r;
	}

	//INVERT DYNAMICS MATRIX
	ww[0] = Z + 2 * M - N; 
	ww[1] = Z - 2 * L + N;
	ww[2] = Z - 2 * M - N;
	ww[3] = Z + 2 * L + N;

	for (i = 0; i < 4;i++){
		ww[i] >>= 2;
	}
	

	//clip actuator values to minimun 0
	for( i = 0; i < 2; i++) 
		if(ww[i] < 0) 
			ww[i] = 0;

	//ASSIGN ENGINE VALUES	
	ae[0] = sqrt(ww[0]); 
	ae[1] = sqrt(ww[1]); 
	ae[2] = sqrt(ww[2]);	
	ae[3] = sqrt(ww[3]);


}
	









