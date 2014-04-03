/*--------------
FULL_CONTROL_MODE

Generates actuator inputs for the full control mode.

19-03-2014(Created by Diogo Monteiro)

*/

#define MAX_ROLLPITCH 100 //??? -  TODO define experimentally
#define ROLLPITCH_SCALING	MAX_ROLLPITCH/127 

void full_control_mode(int LIFT, int ROLL, int PITCH, int YAW,int *ae) {
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
		L = p1control*(ROLL*ROLLPITCH_SCALING - phi);
	}
	else {
		L = p1control*((ROLL-255)*ROLLPITCH_SCALING - phi);
	}
    L -= p2control*p; //Derivative Controller
    
   
        
	//PITCH MOMENTUM
	if (PITCH <= 127) {
		M = p1control*(PITCH*ROLLPITCH_SCALING - theta);
	}
	else {
		M = p1control*((PITCH - 255)*ROLLPITCH_SCALING - theta);
	}		
	M -= p2control*q; //Derivative Controller
    
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
	









