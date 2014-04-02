/*--------------
Define DC offset of the sensors while QR is not moving

05-03-2014(Created by Diogo Monteiro)

*/

#define CALIBRATION_THRESHOLD 256
//GLOBALS
int OFFSET_x0[6] = {0, 0, 0, 0, 0, 0};
int calibration_counter = 0;
int calibration_done = 0;

void calibration_mode(void) {

	int i;
	if (calibration_counter == 0) { 
		for(i = 0; i < 6; i++) OFFSET_x0[i] = 0;
		telemetry_flag = telemetry_flag & 0xFE;
        calibration_done = 0;
	}
		
	for(i = 0; i < 6; i++) 
	{
		OFFSET_x0[i] += x0[i];
	}
	
    
	if (calibration_counter == CALIBRATION_THRESHOLD-1) {

		for(i = 0; i < 6; i++) {
			   OFFSET_x0[i] >>= 8;
		}
        
		telemetry_flag = telemetry_flag | 0x03;
        calibration_done = 1;
	}   
    calibration_counter++;
    
}
