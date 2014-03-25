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
	rs232_open();
	if (fd_RS232 == 0) {
		printf("Error opening the port \n");
		return 0;
	}
	//Joystick buffer clearence and calibration of yaw axis
//	clear_js_buffer();
//	js_calibration();

	/*Initializes the Package Data (Lift,Roll,Pitch,Yaw for Control Modes)
	 *(P,P1,P2,0 for Control Gains Mode)*/
	int data[PARAM_LENGTH] = {0,0,0,0};
	Package mPkg;
	InitPkg(&mPkg,MODE_SAFE); //Intializes Package
	
	//General Counter
	int i; 
	//AbortFlag
	int abort = 0;
	//Stores the pressed key char
	int key = 0;
	//Stores the read byte
	BYTE readbuff;
	//Counts how many bytes have been read during the entire loop
 	int buff_count = 0;
	//Count variable which loops over the sending package
	int datai = 0;
	//Used to check whether the complete Pkg has been written
	int Pkg_written = 0;
	//Used to disable writting to the port
	int writeflag = 1;

	// Variables supervision for tx and rx number of sent/received bytes
	int 	nbrx, nbtx;
	nbtx=nbrx=0;

	//DATA LOGGING FILE
	FILE *DLf = fopen("DATALOGGING.txt", "w");
	if (DLf == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}

	while (key != 43) {

		
		//reads data from the joystick ...comment if joystick is not connected
//		abort = read_js(jmap);
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

		//SETS THE PACKAGE WITH THE DESIRED DATA
		SetPkgMode(&mPkg, keymap[0]);
		SetPkgData(&mPkg, data);
		//Prints the package
		/*for (i = 0; i < PKGLEN; i++) {
			printf("[%x]",mPkg.Pkg[i]);
		}
		printf("\n");*/
		
		//CHECKS KEYBOARD INPUT FOR WRITTING
		if (key == 126){ //stops writting
			printf("\nLogging to filee..");
			writeflag = 0;
		}

		//WRITTING
		if (writeflag == 1){
			//printf("\nWriteloop");
			// Writes the pkg byte by byte. Makes sure that each byte is written
			do{
				
				nbtx = write(fd_RS232, &(mPkg.Pkg[datai]), sizeof(BYTE));
				if (nbtx == 1) { //a Byte has been written	
					datai++;
				}
				if (datai >= PKGLEN){ //The Pkg has not been completely written
					datai = 0;
					Pkg_written = 1;
				}
				else {
					Pkg_written = 0;
				}
			}while(Pkg_written == 0);
		}


		//READING
		do{
			//printf("\nReadloop");
			nbrx = read(fd_RS232, &readbuff, sizeof(BYTE));
			//printf("\n [%i] %i nbrx = %i",writeflag,buff_count++,nbrx);
			if (nbrx > 0)
			{
				//if (readbuff != 0x80 && readbuff != 0x11){
				printf("\n\nRead %i: [%x] Wrote[%x] ",buff_count++, readbuff,mPkg.Pkg[PKGLEN-1]);
				//}
		
				//Writes the datalog in a Txt file
				if (!writeflag){
					fprintf(DLf, "%x\n", (BYTE)readbuff);
					//printf("\n printing to file...%x ", (BYTE)readbuff);
				}
				//printf("\nReadBuffer hex: %x char: %c ", readbuff, readbuff);
			}
		} while (nbrx < 0);
	}
	fclose(DLf);
	rs232_close();
	printf("\n Port is closed \n");
	return 0;
	
}

