/*--------------
YAW_CONTROL_MODE

Generates actuator inputs for the yaw control mode.

13-03-2014(Created by Diogo Monteiro)

*/

#define MAX_Z 2560000 //for all engines at 800
#define LOW_Z_CONVERSION 81000/75 //81000 for all engines at 450 (hover)
#define HIGH_Z_CONVERSION (MAX_Z-450)/(255-75)

#define MAX_LM	50000 // one engine at 600 and the other 3 at 450
#define LM_CONVERSION MAX_LM/127

#define MAX_YAWRATE 20 // ?? - define experimentally
#define YAWRATE_SCALING MAX_YAWRATE/127


/*--------------
SQRT

Computes the square root of a number, based on bitwise operations

13-03-2014(Created by Diogo Monteiro)

*/
long int sqrt(long int number) {
    long int result = 0;
    long int error = 1 << 30; 
 		
    while (error > number)
        error >>= 2;
 
    while (error != 0) {
        if (number >= result + error) {
            number -= result + error;
            result = (result >> 1) + error;
        }
        else
            result >>= 1;
        error >>= 2;
    }
    return result;
}


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
	









