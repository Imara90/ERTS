/*--------------
YAW_CONTROL_MODE

Generates actuator inputs for the yaw control mode.

13-03-2014(Created by Diogo Monteiro)

*/

#define MAX_Z 3200 //for all engines at 800
#define LOW_Z_CONVERSION 1800/75 //1800 for all engines at 450 (hover)
#define HIGH_Z_CONVERSION (MAX_Z-1800)/(255-75)

#define MAX_LM	50 // one engine at 500 and the other 3 at 450
#define LM_CONVERSION MAX_LM/127

#define MAX_YAWRATE 20 // ?? - define experimentally
#define YAWRATE_SCALING MAX_YAWRATE/127



void yaw_control_mode(void) {
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
		L = package[ROLL]*LM_CONVERSION;
	}
	else {
		L = (package[ROLL]-255)*LM_CONVERSION;
	}
	
	//PITCH MOMENTUM
	if (package[PITCH] <= 127) {
		M = package[PITCH]*LM_CONVERSION;
	}
	else {
		M = (package[PITCH]-255)*LM_CONVERSION;
	}		
		
	//YAW MOMENTUM
	if (package[YAW] <= 127) {
		N = pcontrol*package[YAW]*YAWRATE_SCALING - pcontrol*r;
	}			
	else {
		N = pcontrol*(package[YAW] - 255)*YAWRATE_SCALING - pcontrol*r;
	}

	if(package[LIFT] == 0) 
	{
		N = 0;
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
	for( i = 0; i < 4; i++){ 
		if(ww[i] < 0){
            ww[i] = 0;
	    }
    }
	for( i = 0; i < 4; i++){
        
        ae[i]=ww[i];
    }

	



}
	









