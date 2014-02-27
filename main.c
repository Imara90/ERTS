//
//  main.cpp
//  ERTS
//
//  Created by Daniel Lemus Perez on 17/02/14.
//  Copyright (c) 2014 Daniel Lemus Perez. All rights reserved.
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "Package.h"

#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

#include "read_js.h" //Diogo's Function
#include "read_kb.h" // Diogo's keyboard


int open_port()
{
	struct termios	tty;
	int fd_RS232; /* File descriptor for the port */
	int result;

	fd_RS232 = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd_RS232 == -1)
	{
		printf("open_port: Unable to open /dev/ttyUSB0 - \n");
		return 0;
	}
	printf("Port Successfully opened\n");

	tty.c_iflag = IGNBRK; /* ignore break condition */
	tty.c_oflag = 0;
	tty.c_lflag = 0;
	
	//sets the data type transfer
	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; /* 8 bits-per-character */
	tty.c_cflag |= CLOCAL | CREAD; /* Ignore model status + read input */	

	
	//sets the baud rate
	printf("Setting Baudrate 115200bps \n");
	cfsetospeed(&tty, B115200); 
	cfsetispeed(&tty, B115200);

	tty.c_cc[VMIN]  = 0;
	tty.c_cc[VTIME] = 0;

	tty.c_iflag &= ~(IXON|IXOFF|IXANY);
	
	printf("Setting atributes into the port \n");
	result = tcsetattr (fd_RS232, TCSANOW, &tty); /* non-canonical */
	if (result != 0)
	{
		printf("Unable to set port parameters - \n");
		return 0;
	}
	//assert (result != 0);

	//clearing the buffer 
	tcflush(fd_RS232,TCIOFLUSH);

	return fd_RS232;
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

	int keymap[8] = {MODE_SAFE,0,0,0,0,0,0,0};
	int jmap[4] = {0,0,0,0};
	int key = getchar();
	int fd1 = open_port();
	if (fd1 == 0) {
		printf("Error opening the port \n");
		return 0;
	}
	clear_js_buffer();
	
	
	int mode, p, p1, p2;
	int data[PARAM_LENGTH] = {0,0,0,0};
	Package mPkg;
	InitPkg(&mPkg,MODE_SAFE);
	int i;
	int result;
	while (key != 27) {
		
		read_js(&pitch,&roll,&yaw,&lift);
		c = getchar();
		if (c != -1) read_kb(keymap,c);

		
		data[0] = lift+keymap[1];
		data[1] = roll+keymap[2];
		data[2] = pitch+keymap[3];
		data[3] = yaw+keymap[4];
		

		//CREATES THE PACKAGE
		SetPkgMode(&mPkg, keymap[0]);
		SetPkgData(&mPkg, data);
		//Prints the package
		for (i = 0; i < PKGLEN; i++) {
			printf("[%x]",mPkg.Pkg[i]);
		}
		printf("\n");
		
		//writes in the port
		result = write(fd1,mPkg.Pkg,11*sizeof(BYTE));
//		assert(result != 7);

		// Check input key for termination (Press ESC)
		key = getchar();
		// 20 msec pause
		usleep(20000);
	}
	close(fd1);
	printf("\n Port is closed \n");
	return 0;
	
}

