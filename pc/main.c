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

#include "read_js.h" 	// Diogo's Function
#include "read_kb.h" 	// Diogo's keyboard
#include "rs232.h" 	// Provides functions to open and close rs232 port

#define BYTE unsigned char

int main()
{
	term_nonblocking();
	keyboard_nonblocking();

	//Initializes the keymap from the keyboard
	int keymap[8] = {MODE_SAFE,0,0,0,0,0,0,0};
	//Initializes the keymap from the keyboard
	int jmap[4] = {0,0,0,0};
	//Opens the port
//	int fd_RS232 = open_rs232_port();
	rs232_open();
	if (fd_RS232 == 0) {
		printf("Error opening the port \n");
		return 0;
	}
	//Joystick buffer clearence and calibration of yaw axis
	//clear_js_buffer();
//	js_calibration();

	/*Initializes the Package Data (Lift,Roll,Pitch,Yaw for Control Modes)
	 *(P,P1,P2,0 for Control Gains Mode)*/
	int data[PARAM_LENGTH] = {0,0,0,0};
	Package mPkg;
	InitPkg(&mPkg,MODE_SAFE); //Intializes Package
	int i;
	int result;
	int abort = 0;
	int key = 0;
	BYTE ReadBuffer[1];
	BYTE readbuff;
 	int buff_count = 0;
	int datai = 0;
	int writting = 1; //ready to write
	int writeflag = 1;
/*****************************************************************/
	// To clean the rx buffer 
	int 	nbrx, nbtx, ptx, prx, nv;
	BYTE 	bytetowrite;

	nbtx=nbrx=0; ptx=prx=0;
	int rwcounter = 0;
/*****************************************************************/
	//DATA LOGGING 
	FILE *DLf = fopen("DATALOGGING.txt", "w");
	if (DLf == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}

	while (key != 'x') {

		
		//reads data from the joystick ...comment if joystick is not connected
		//abort = read_js(jmap);
		//printf("jmap[%x][%x][%x][%x]\n",jmap[0],jmap[1],jmap[2],jmap[3]);
		//Gets the pressed key in the keyboard ... for termination (Press ESC)
		key = getchar();
		if (key != -1) abort = read_kb(keymap,(char*)&key);
		
		switch (keymap[0]) {
			case MODE_P: //CONTROL GAINS, Starting from the second place in data array (First place is reserved for lift value)
				data[0] = 0;				
				data[1] = keymap[5];
				data[2] = keymap[6];
				data[3] = keymap[7];
				break;
				
			default: //CONTROL MODES
				data[0] = jmap[0]+keymap[1];
				data[1] = jmap[1]+keymap[2];
				data[2] = jmap[2]+keymap[3];
				data[3] = jmap[3]+keymap[4];
				break;
		}
		
		//EVALUATES IF ABORTION REQUESTEQ
		if (abort == 1) keymap[0] = MODE_ABORT;

		//CREATES THE PACKAGE
		SetPkgMode(&mPkg, keymap[0]);
		SetPkgData(&mPkg, data);
		//Prints the package
		/*for (i = 0; i < PKGLEN; i++) {
			printf("[%x]",mPkg.Pkg[i]);
		}
		printf("\n"); */
		
		key = getchar();
		if (key == 126){ //stops writting
			writeflag = 0;
		}

		// Checks for the Clear To Send
		int s, ctsflag;
    		ioctl(fd_RS232, TIOCMGET, &s);
		ctsflag = s & TIOCM_CTS;
		//WRITTING
    		if (ctsflag =! 0 && writeflag == 1){
			// Writes the pkg byte by byte. Makes sure that each byte is written
			writting = 1;
			while(writting == 1){
				nbtx = write(fd_RS232, &(mPkg.Pkg[datai]), sizeof(BYTE));
				if (nbtx == 1) {		
					datai++;
				}
				if (datai >= PKGLEN){
					datai = 0;
					writting = 0;
				}
			}
		}

		//READING	
		while (nbrx == 0){
			nbrx = read(fd_RS232, &readbuff, sizeof(BYTE));
		}
		if (nbrx > 0)
		{	
			if (readbuff != 0x80 && readbuff != 0x11){
			printf("\n\nRead %i: [%x] ",buff_count++, readbuff);
			}
			nbrx = 0;
			//Writes the datalog in a Txt file
			if (!writeflag){
				fprintf(DLf, "%x\n", readbuff);
			}
			//printf("\nReadBuffer hex: %x char: %c ", readbuff, readbuff); 
		}
		//}
		rwcounter ++;
//		usleep(20000);
	}
	fclose(DLf);
	rs232_close();
	printf("\n Port is closed \n");
	return 0;
	
}

