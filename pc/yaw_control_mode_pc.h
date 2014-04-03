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



void yaw_control_mode(int LIFT, int ROLL, int PITCH, int YAW,int *ae) {
	int i;
	
	//LIFT FORCE
	if (LIFT<=75) {
		Z = LIFT*LOW_Z_CONVERSION;
	}	
	else {
		Z = (LIFT-255)*HIGH_Z_CONVERSION+MAX_Z; 
	}	
	//ROLL MOMENTUM
	if (ROLL <= 127) {
		L = ROLL*LM_CONVERSION;
	}
	else {
		L = (ROLL-255)*LM_CONVERSION;
	}
	
	//PITCH MOMENTUM
	if (PITCH <= 127) {
		M = PITCH*LM_CONVERSION;
	}
	else {
		M = (PITCH-255)*LM_CONVERSION;
	}		
		
	//YAW MOMENTUM
	if (YAW <= 127) {
		N = pcontrol*YAW*YAWRATE_SCALING - pcontrol*r;
	}			
	else {
		N = pcontrol*(YAW - 255)*YAWRATE_SCALING - pcontrol*r;
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
	









