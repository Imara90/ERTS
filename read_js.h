/*Reads the input of the joystick
* 20-02-2014(Created by Diogo Monteiro)

Input: mi - values readed and converted (15bits to 8bits)

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



struct js_event js; 
int fd;
//unsigned char axes = 2;
//unsigned char buttons = 2;
int version = 0x000800;
int axis[4];
int j_abort=0;


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
	*js_map=-axis[3]*CONVERSION_FACTOR;

	/*Roll*/
	*(js_map+1)=axis[0]*CONVERSION_FACTOR;

	/*Pitch*/
	*(js_map+2)=axis[1]*CONVERSION_FACTOR;

		/*Yaw*/
	*(js_map+3)=-axis[2]*CONVERSION_FACTOR;
	
	
	
	return j_abort;
}

void clear_js_buffer(void) {
/*Open Joystick driver*/	
	if ((fd = open(JS_DEV, O_RDONLY)) < 0) {
		printf("\n Error: Check joystick connection!\n\n");
		exit(1);
	}

/*Set Joystick Driver Information*/	
	//ioctl(fd, JSIOCGVERSION, &version);
	//ioctl(fd, JSIOCGAXES, &axes);
	//ioctl(fd, JSIOCGBUTTONS, &buttons);	
	
/* set non-blocking mode*/
	fcntl(fd, F_SETFL, O_NONBLOCK);
	
/*Clean Buffer*/
if (read(fd, &js, sizeof(struct js_event)) != sizeof(struct js_event)) {
		perror("\nJOYSTICk 3D: error reading");
		exit (1);
	}
	js.value=0;

}

