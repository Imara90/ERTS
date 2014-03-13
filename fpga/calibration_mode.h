/*--------------
Define DC offset of the sensors while QR is not moving

05-03-2014(Created by Diogo Monteiro)

*/
#define STANDBY_MODE 0x08

//GLOBALS
int OFFSET_y0[6] = {0, 0, 0, 0, 0, 0};
int calibration_counter = 0;


void calibration_mode(void) {

	int i;
	if (calibration_counter == 0) { 
		printf("\nSensor calibration on process....\n");
		for(i = 0; i < 6; i++) OFFSET_y0[i] = 0;
	}
		
	for(i = 0; i < 6; i++) OFFSET_y0[i] += y0[i];
	calibration_counter++;

	if (calibration_counter == 128) {

		for(i = 0; i < 6; i++) OFFSET_y0[i] >>= 7;
		calibration_counter = 0;
		mode = STANDBY_MODE;
		printf("\n... sensor calibration complete. Press '0' to enter SAFE MODE!\n");
	}

}
