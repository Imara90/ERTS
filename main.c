//
//  main.cpp
//  ERTS
//
//  Created by Daniel Lemus Perez on 17/02/14.
//  Copyright (c) 2014 Daniel Lemus Perez. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "Package.h"

#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

#include "read_js.h" //Diogo's Function


int open_port()
{
	int fd; /* File descriptor for the port */
	fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
	{
		printf("open_port: Unable to open /dev/ttyUSB0 - \n");
		return 0;
	}
	printf("Port Successfully opened");
	return fd;
}

struct termios stdin_orig;  // Structure to save parameters

void term_reset() {
        tcsetattr(STDIN_FILENO,TCSANOW,&stdin_orig);
        tcsetattr(STDIN_FILENO,TCSAFLUSH,&stdin_orig);
}

void term_nonblocking() {
        struct termios newt;
        tcgetattr(STDIN_FILENO, &stdin_orig);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK); // non-blocking
        newt = stdin_orig;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        atexit(term_reset);
}

int main()
{
	term_nonblocking();	

	clear_js_buffer();

	int key = getchar();
	int fd1 = open_port();
	
	if (fd1 == 0) {
		printf("Error opening the port \n");
		return 0;
	}
	
	int roll, pitch, yaw, lift;
	int data[PARAM_LENGTH] = {0,0,0,0,0,0,0};
	Package mPkg;
	InitPkg(&mPkg,MODE_MANUAL);
	int i;
	while (key != 97) {

		//read_js(&pitch,&roll,&yaw,&lift);

//		data[0] = roll;
//		data[1] = pitch;
//		data[2] = yaw;
//		data[3] = lift;

		//CREATES THE PACKAGE
		SetPkgData(&mPkg, data);
		//Prints the package
		for (i = 0; i < PKGLEN; i++) {
			printf("[%x]",mPkg.Pkg[i]);
		}
		printf("\n");
		//clear the buffer 
		tcflush(fd1,TCIOFLUSH);
		//writes in the port
		write(fd1,mPkg.Pkg,11*sizeof(BYTE));
		key = getchar();
		usleep(20000);
	}
	close(fd1);
	printf("\n Port is closed \n");
	return 0;
	
}

