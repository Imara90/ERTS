/*All functions related to Joystick reading
* 20-02-2014(Created by Diogo Monteiro)

Output: strike button - 0 for off and 1 for on


*/
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "joystick.h"

#define NAME_LENGTH 128
#define JS_DEV	"/dev/input/js0"
#define CONVERSION_FACTOR 127/32767
#define LIFT_CONVERSION_FACTOR 255/65534



struct js_event js; 
int fd;
//unsigned char axes = 2;
//unsigned char buttons = 2;
int version = 0x000800;
int axis[4];
int j_abort=0;
int pos_calibration_yaw=0;
int neg_calibration_yaw=0;


/*------------------------------------------------------------------
 * Sets joystick non-blocking mode and clears buffer
 * Created by Diogo Monteiro
 *------------------------------------------------------------------
 */
int read_js(int *js_map) {


/*Read Joystick*/
	while (read(fd, &js, sizeof(struct js_event)) == 
		       			sizeof(struct js_event))  {
		switch(js.type & ~JS_EVENT_INIT) {
		case JS_EVENT_BUTTON:
			if(js.number==0) {
			
				j_abort = js.value;
		
			}
			break;
		case JS_EVENT_AXIS:
			if(js.number == 0 || js.number == 1 || js.number == 2 || js.number == 3) {
				axis[js.number] = js.value;
			}
			break;
		}
	}
	
	/*Lift*/
	*js_map=-(axis[3]-32767)*LIFT_CONVERSION_FACTOR;
	//AVOID HEADER	
	if(*js_map==128) {
	
		*js_map=129;
	}	
	
	/*Roll*/
	*(js_map+1)=axis[0]*CONVERSION_FACTOR;

	/*Pitch*/
	*(js_map+2)=axis[1]*CONVERSION_FACTOR;

	/*Yaw*/
	if ( axis[2]>pos_calibration_yaw || axis[2]<neg_calibration_yaw){
		
		if( axis[2]<0) {
	
			*(js_map+3)=((-axis[2]-32767)*127/(32767+neg_calibration_yaw)+127);
		}
		else {
		
			*(js_map+3)=-((axis[2]-32767)*127/(32767-pos_calibration_yaw)+127);		
		}
	
	}
	else *(js_map+3)=0;
	printf("\n\n YAW:%d axis[2]:%d neg: %d pos: %d \n",*(js_map+3),axis[2],neg_calibration_yaw,pos_calibration_yaw);
		
	return j_abort;
}
/*------------------------------------------------------------------
 * Sets joystick non-blocking mode and clears buffer
 * Created by Diogo Monteiro
 *------------------------------------------------------------------
 */
void clear_js_buffer(void) {
/*Open Joystick driver*/	
	if ((fd = open(JS_DEV, O_RDONLY)) < 0) {
		printf("\n Error: Check joystick connection!\n\n");
		exit(1);
	}
	
/* set non-blocking mode*/
	fcntl(fd, F_SETFL, O_NONBLOCK);
	
/*Clear Buffer*/
if (read(fd, &js, sizeof(struct js_event)) != sizeof(struct js_event)) {
		perror("\nJOYSTICk 3D: error reading");
		exit (1);
	}
	js.value=0;

}
/*------------------------------------------------------------------
 * Joystick (YAW AXIS) calibration
 * Created by Diogo Monteiro
 *------------------------------------------------------------------
 */
void js_calibration(void) {
	char c = 0;
	printf("Calibration on process.......press 't' to terminate!\n\n");

	
	while(c!='t'){
	
		/*Read Joystick*/
		while (read(fd, &js, sizeof(struct js_event)) == 
				     			sizeof(struct js_event))  {
			switch(js.type & ~JS_EVENT_INIT) {
			case JS_EVENT_BUTTON:
				if(js.number==0) {
			
					j_abort = js.value;
		
				}
				break;
			case JS_EVENT_AXIS:
				if(js.number == 0 || js.number == 1 || js.number == 2 || js.number == 3) {
					axis[js.number] = js.value;
				}
				break;
			}
		}

		if(axis[2]>pos_calibration_yaw){
			pos_calibration_yaw=axis[2];
		}

		if(axis[2]<neg_calibration_yaw){
			neg_calibration_yaw=axis[2];
		}
		
		c=getchar();
		if(c == 't' && (pos_calibration_yaw>6000 || neg_calibration_yaw<-6000)){
		
			printf("\nCalibration might be incorrect! Do you want to repeat[y/n]?");
			c=getchar();			
			while (c == -1) {c=getchar();usleep(20000);}
			if (c == 'y') {
				c=0;printf("\n\nCalibration on process.......press 't' to terminate!\n\n");
			}
			else c ='t';
		}
	}
}
