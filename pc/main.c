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
#define FALSE 	0
#define TRUE 	1
#include "mode_selection.h"	// Diogos mode selection function

#define START_BYTE 0x80
#define TELLEN	      19
#define TELPKGLEN     TELLEN - 1 //EXPECTED TELEMETRY PACKAGE LENGTH EXCLUDING THE STARTING BYTE
#define TELPKGCHKSUM  TELPKGLEN - 1

#define START_BYTE 0x80
#define DLPKGLEN     12 //EXPECTED DATA LOG PACKAGE LENGTH EXCLUDING THE STARTING BYTE
#define DLPKGCHKSUM  DLPKGLEN - 1



int TeleDecode(int* TelPkg/*, int* Output*/){
	
	int i;
	int sum = 0;
	int ChkSum = TelPkg[TELPKGCHKSUM];
	//CHECKSUM CHECK
	for(i = 0; i < TELPKGCHKSUM ; i++)
	{
		sum += TelPkg[i];
	}
	sum = (BYTE)~sum;
//	printf("[%x][%x]",,ChkSum);
	if (ChkSum == sum)
	{
		//DECODING PART
	}
	else
	{
		return FALSE;
	}	
	return TRUE;
}

int DLDecode(int* DLPkg/*, int* Output*/){
	
	int i;
	int sum = 0;
	int ChkSum = DLPkg[DLPKGCHKSUM];
	//CHECKSUM CHECK
	for(i = 0; i < DLPKGCHKSUM ; i++)
	{
		sum += DLPkg[i];
	}
	sum = (BYTE)~sum;
//	printf("[%x][%x]",,ChkSum);
	if (ChkSum == sum)
	{
		//DECODING PART
	}
	else
	{
		return FALSE;
	}	
	return TRUE;
}


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
	int DLData[TELPKGLEN];
	for(i = 0; i < TELPKGLEN; i++){
		DLData[i] = 0;
	}
	int dltimeout = 0;
	//Used to request data logging
	int DLreq = 0;
	FILE *DLfile = fopen("DATALOGGING.txt", "w");
	if (DLfile == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}

	//TELEMETRY
	int TeleData[TELPKGLEN];
	int datacount  = 0;
	int ChkSumOK = FALSE;
	//Initializes telemetry array
	for(i = 0; i < TELPKGLEN; i++){
		TeleData[i] = 0;
	}
	int DisplayArray[7];// Contains the current mode along with the 6 sensors values
	//Initializes display array
	for(i = 0; i < 7; i++){
		DisplayArray[i] = 0;
	}
	FILE *TeleFile = fopen("Telemetry.txt", "w");
	if (TeleFile == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}
	
	while (key != 43) {// + key

		
		//reads data from the joystick ...comment if joystick is not connected
//		abort = read_js(jmap);
		//Gets the pressed key in the keyboard ... for termination (Press ESC)
		key = getchar();
		//printf("key %i\n",key);
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
		
		//EVALUATES IF ABORTION REQUESTED
		if (abort == 1) keymap[0] = MODE_ABORT;

		//SETS THE PACKAGE WITH THE DESIRED DATA
		SetPkgMode(&mPkg, keymap[0]);
		SetPkgData(&mPkg, data);
		//Prints the package
		/*for (i = 0; i < PKGLEN; i++) {
			printf("[%x]",mPkg.Pkg[i]);
		}
		printf("\n");*/
		
		//CHECKS KEYBOARD INPUT FOR DATALOGGING
		if (key == 126){ //Data Logging requested
			DLreq = 1;
			datacount = 0; // Resets the counter (Reading new data)
		}

		
		if (writeflag == 1){
			//WRITTING
			//Writes the pkg byte by byte. Makes sure that each byte is written
			do{
				nbtx = write(fd_RS232, &(mPkg.Pkg[datai]), sizeof(BYTE));
				if (nbtx == 1) { //a Byte has been written	
					datai++;
				}
				if (datai >= PKGLEN){ //The Pkg has been completely written
					datai = 0;
					Pkg_written = 1;
				}
				else {//The Pkg has NOT been completely written
					Pkg_written = 0;
				}
			}while(Pkg_written == 0);
		}
		//READING 
		//Makes sure everything is read from the RX line
		do{
			nbrx = read(fd_RS232, &readbuff, sizeof(BYTE));
			//printf("\n [%i] %i nbrx = %i",writeflag,buff_count++,nbrx);
			if (nbrx > 0)
			{
				// CHECKING FOR STARTING BYTE
				if (readbuff == START_BYTE)
				{
					datacount = 0; //Reset the data counter. Starts over!!
				}
				// STORING THE DATA. Starts over if a starting BYTE is found!!!
				if (readbuff != START_BYTE)
				{
					TeleData[datacount] = readbuff;
					datacount++;
				}
				// NORMAL OPERATION
				if (DLreq == 0){ 
					// TELEMETRY DECODING. Only If the store data has the expected size
					if (datacount == TELPKGLEN) //Complete Pkg Received
					{
						//Prints the stored package
						for (i = 0; i < TELPKGLEN; i++) {
							printf("[%x]",TeleData[i]);
						}
				
						// DECODING. Checksum proof and stores decoded values in new array DispData
						//ChkSumOK = decode(TeleData,&DispData);
						ChkSumOK = TeleDecode(TeleData);
						printf(" Chksum OK = %i \n",ChkSumOK);
						//Saves data only if the pkg is complete
						if (ChkSumOK){
							//Writes the telemetry in a Txt file
							for (i = 0; i < TELPKGLEN; i++) {
								fprintf(TeleFile, "%x ", TeleData[i]);
							}
							fprintf(TeleFile,"\n");
						}
					}
				}
				else //DATA LOG REQUESTED Shuts writting down and only reads
				{
					// DATA LOGGING DECODING. Only If the stored data has the expected size
					if (datacount == DLPKGLEN) //Complete Pkg Received
					{
						printf("DL ");
						//Prints the stored package
						for (i = 0; i < DLPKGLEN; i++) {
							printf("[%x]",TeleData[i]);
						}
						//DECODING. Checksum proof and stores decoded values in new array DispData
						//ChkSumOK = decode(TeleData,&DispData);
						ChkSumOK = DLDecode(TeleData);
						printf(" Chksum OK = %i \n",ChkSumOK);
						//Saves data only if the pkg is complete
						if (ChkSumOK){
							//Writes the datalog in a Txt file
							for (i = 0; i < DLPKGLEN; i++) {
								fprintf(DLfile, "%x ", TeleData[i]);
							}
							fprintf(DLfile,"\n");
						}
					}
					writeflag == 0;
				}
				//dltimeout = 0;
			}
		} while (nbrx > 0);

		/*if( (dltimeout++ > 2000) && writeflag == 0){
			printf("Data Login Downloaded...");
			break;
		}*/
		
		
	}
	fclose(DLfile);
	fclose(TeleFile);
	rs232_close();
	printf("\n Port is closed \n");
	return 0;
	
}

