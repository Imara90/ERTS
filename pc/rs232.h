
//
//  rs232.h
//  
//
//  Created by Daniel Lemus Perez on 04/03/14.
//
//


#ifndef _rs232_h
#define _rs232_h

/*

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // UNIX standard function definitions 
#include <fcntl.h>   // File control definitions /
#include <errno.h>   // Error number definitions //
#include <termios.h> // POSIX terminal control definitions 

int fd_RS232; // File descriptor for the port 
int rs232_open()
{
	struct termios	tty;

	int result;
	
	fd_RS232 = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd_RS232 == -1)
	{
		printf("open_port: Unable to open /dev/ttyUSB0 - \n");
		return 0;
	}
	printf("Port Successfully opened\n");
	
	tty.c_iflag = IGNBRK; // ignore break condition /
	tty.c_oflag = 0;
	tty.c_lflag = 0;
	
	//sets the data type transfer
	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8 bits-per-character /
	tty.c_cflag |= CLOCAL | CREAD; // Ignore model status + read input /
	
	
	//sets the baud rate
	printf("Setting Baudrate 115200bps \n");
	cfsetospeed(&tty, B115200);
	cfsetispeed(&tty, B115200);
	
	tty.c_cc[VMIN]  = 0;
	tty.c_cc[VTIME] = 0;
	
	tty.c_iflag &= ~(IXON|IXOFF|IXANY);
	
	printf("Setting atributes into the port \n");
	result = tcsetattr (fd_RS232, TCSANOW, &tty); // non-canonical 
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

void rs232_close()
{
	close(fd_RS232);
}

*/



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







/*------------------------------------------------------------
 * Simple terminal in C
 * 
 * Arjan J.C. van Gemund (+ few mods by Mark Dufour)
 *------------------------------------------------------------
 */
#define	FALSE		0
#define	TRUE		1

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

/*------------------------------------------------------------
 * console I/O
 *------------------------------------------------------------
 */
struct termios 	savetty;

void	term_initio()
{
	struct termios tty;

	tcgetattr(0, &savetty);
	tcgetattr(0, &tty);

//	tty.c_lflag &= ~(ECHO|ECHONL|ICANON|IEXTEN);
	tty.c_cc[VTIME] = 0;
	tty.c_cc[VMIN] = 0;

	tcsetattr(0, TCSADRAIN, &tty);
}

void	term_exitio()
{
	tcsetattr(0, TCSADRAIN, &savetty);
}

void	term_puts(char *s) 
{ 
	fprintf(stderr,"%s",s); 
}

void	term_putchar(char c) 
{ 
	putc(c,stderr);
}

int	term_getchar_nb() 
{ 
        static unsigned char 	line [2];

        if (read(0,line,1)) // note: destructive read
        		return (int) line[0];
        
        return -1;
}

int	term_getchar() 
{ 
        int    c;

        while ((c = term_getchar_nb()) == -1)
                ;
        return c;
}

/*------------------------------------------------------------
 * Serial I/O 
 * 8 bits, 1 stopbit, no parity, 
 * 115,200 baud wired and 9,600 baud wireless.
 *------------------------------------------------------------
 */
#include <termios.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#define SERIAL_DEVICE	"/dev/ttyS0"
#define USB_DEVICE	"/dev/ttyUSB0"
#define WIFI_DEVICE 	"/dev/ttyUSB0" /* may need to change this */

int serial_device = 1;
int fd_RS232;

int rs232_open(void)
{
  	char 		*name;
  	int 		result;  
  	struct termios	tty;

	if (serial_device == 0) 
	{   
		fd_RS232 = open(SERIAL_DEVICE, O_RDWR | O_NOCTTY);
		fprintf(stderr,"using /dev/ttyS0\n"); 

	} 
	else if ( (serial_device == 1) || (serial_device == 2) ) 
	{
        	fd_RS232 = open(USB_DEVICE, O_RDWR | O_NOCTTY);
		fprintf(stderr,"using /dev/ttyUSB0\n"); 
	} 

	assert(fd_RS232>=0);

  	result = isatty(fd_RS232);
  	assert(result == 1);

  	name = ttyname(fd_RS232);
  	assert(name != 0);

  	result = tcgetattr(fd_RS232, &tty);	
	assert(result == 0);

	tty.c_iflag = IGNBRK; /* ignore break condition */
	tty.c_oflag = 0;
	tty.c_lflag = 0;

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; /* 8 bits-per-character */
	tty.c_cflag |= CLOCAL | CREAD; /* Ignore model status + read input */
	//tty.c_cflag |= CCTS_OFLOW;		

	/* Set output and input baud rates. 
	 */
	if (serial_device == 0 || serial_device == 1) // wired 
	{  
		cfsetospeed(&tty, B115200); 
		cfsetispeed(&tty, B115200); 
	} 
    	else if (serial_device == 2) // wireless 
	{  
		cfsetospeed(&tty, B9600); 
		cfsetispeed(&tty, B9600); 
	}

	tty.c_cc[VMIN]  = 0;
	tty.c_cc[VTIME] = 0;

	tty.c_iflag &= ~(IXON|IXOFF|IXANY);

	result = tcsetattr (fd_RS232, TCSANOW, &tty); /* non-canonical */

	tcflush(fd_RS232, TCIOFLUSH); /* flush I/O buffer */
//	return fd_RS232;
}


int 	rs232_close(void)
{
  	int 	result;

  	result = close(fd_RS232);
  	assert (result==0);
}


int	rs232_getchar_nb()
{
	int 		result;
	unsigned char 	c;

	result = read(fd_RS232, &c, 1);

	if (result == 0) 
		return -1;
	
	else 
	{
		assert(result == 1);   
		return (int) c;
	}
}


int 	rs232_getchar()
{
	int 	c;

	while ((c = rs232_getchar_nb()) == -1) 
		;
	return c;
}


int 	rs232_putchar(char c)
{ 
	int result;

	do {
		result = (int) write(fd_RS232, &c, 1);
	} while (result == 0);   

	assert(result == 1);
	return result;
}



#endif



