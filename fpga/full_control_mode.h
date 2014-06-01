/*--------------
FULL_CONTROL_MODE

Generates actuator inputs for the full control mode.

19-03-2014(Created by Diogo Monteiro)

*/

#define MAX_ROLLPITCH 100 
#define ROLLPITCH_SCALING	MAX_ROLLPITCH/127 

void full_control_mode(void) {
	int i;
	if (package[LIFT]<=75) {
		Z = package[LIFT]*LOW_Z_CONVERSION;
	}	
	else {
		Z = (package[LIFT]-255)*HIGH_Z_CONVERSION+MAX_Z; 
	}	
	if (package[ROLL] <= 127) {
		L = p1control*(package[ROLL]*ROLLPITCH_SCALING - phi);
	}
	else {
		L = p1control*((package[ROLL]-255)*ROLLPITCH_SCALING - phi);
	}
	L -= p2control*p; 
   	if (package[PITCH] <= 127) {
		M = p1control*(package[PITCH]*ROLLPITCH_SCALING - theta);
	}
	else {
		M = p1control*((package[PITCH] - 255)*ROLLPITCH_SCALING - theta);
	}		
	M -= p2control*q; 
   	if (package[YAW] <= 127) {
		N = pcontrol*package[YAW]*YAWRATE_SCALING - pcontrol*r;
	}			
	else {
		N = pcontrol*(package[YAW] - 255)*YAWRATE_SCALING - pcontrol*r;
	}
	ww[0] = Z + 2 * M - N; 
	ww[1] = Z - 2 * L + N;
	ww[2] = Z - 2 * M - N;
	ww[3] = Z + 2 * L + N;
	for (i = 0; i < 4;i++)
	{
		ww[i] >>= 2;
	}
	for( i = 0; i < 4; i++)
	{ 
	    if(ww[i] < 0)
	    {
	      ww[i] = 0;
	    }
	}
	for( i = 0; i < 4; i++)
	{
	  ae[i]=ww[i];
	}
}
	









