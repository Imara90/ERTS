//
//  rs232.h
//  
//
//  Created by Daniel Lemus Perez on 04/03/14.
//
//

#ifndef _rs232_h
#define _rs232_h

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */


int open_rs232_port()
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

void close_rs232_port(int fd_RS232)
{
	close(fd_RS232);
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

void keyboard_nonblocking() {
        struct termios newt;
        tcgetattr(STDIN_FILENO, &stdin_orig);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK); // non-blocking
        newt = stdin_orig;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        atexit(term_reset);
}

#endif
