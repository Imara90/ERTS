//
//  Package.h
//  CommProt
//
//  Created by Daniel Lemus Perez on 22/02/14.
//  Copyright (c) 2014 Daniel Lemus Perez. All rights reserved.
//

#ifndef CommProt_Package_h
#define CommProt_Package_h

#include <stdio.h>
#include <stdlib.h>

#define BYTE unsigned char
#define WORD unsigned short

#define INT8_MAX					127
#ifndef INT8_MIN
	#define INT8_MIN					-127
#endif

#define INT16_MAX					32767
#define INT16_MIN					-32768


//MODES LIST
#define MODE_SAFE				0x00
#define MODE_PANIC				0x01
#define MODE_MANUAL				0x02
#define MODE_CALIBRATION		0x03
#define MODE_YAW_CONTROL		0x04
#define MODE_FULL_CONTROL		0x05
#define MODE_P					0x06
#define MODE_P1P2					0x07



//PACKAGE DEF
#define HEADER_LENGTH			0x01
#define MODE_LENGTH				0x01
#define PARAM_LENGTH			0x04
#define CHKSUM_LENGTH			0X01
#define PKGLEN					HEADER_LENGTH + MODE_LENGTH + PARAM_LENGTH + CHKSUM_LENGTH
#define HEADER_POS				0x00
#define MODE_POS				HEADER_LENGTH
#define DATA_POS				HEADER_LENGTH+MODE_LENGTH
#define CHKSUM_POS				HEADER_LENGTH+MODE_LENGTH+PARAM_LENGTH


WORD TrimToMaxWord(int value)
{
	if (value>INT16_MAX)
	{
		printf("Clipping %i to %u",value,INT16_MAX);
		value = INT16_MAX;
	}
	else if (value<INT16_MIN)
	{
		printf("Clipping %i to %u",value,INT16_MAX);
		value = INT16_MIN;
	}
	return value;
}
BYTE TrimToMaxByte(int value)
{
	if (value>INT8_MAX)
	{
		printf("Clipping %i to %u",value,INT8_MAX);
		value = INT8_MAX;
	}
	else if (value<INT8_MIN)
	{
		printf("Clipping %i to %u",value,INT8_MAX);
		value = INT8_MIN;
	}
	return (BYTE)value;
}

typedef struct
{
	BYTE Pkg[PKGLEN];
	BYTE Mode;
	BYTE ChkSum;
	BYTE Len;
}Package;

int SetPkgMode(Package* mPkg, BYTE mMode)
{
	//Check the mode
	if ((mMode < 0) || (mMode > 9)) {
		mPkg->Pkg[MODE_POS] = MODE_SAFE;
		return 1;
	}
	mPkg->Pkg[MODE_POS] = mMode;
	return -1;
}

void InitPkg(Package* mPkg,BYTE mMode)
{
	int i;
	mPkg->Len = PKGLEN;
	mPkg->Pkg[HEADER_POS] = 0x80; //STARTING BYTE
	//mPkg->Pkg[1] = 0x00; //STARTING BYTE
	for (i = MODE_POS; i< PKGLEN; i++) {
		mPkg->Pkg[i] = 0x00;
	}
	SetPkgMode(mPkg, mMode);
	mPkg->Mode = mPkg->Pkg[MODE_POS];
}


int SetChksum(Package* mPkg)
{
	int i;
	int sum = 0;
	for (i = MODE_POS; i < PKGLEN - CHKSUM_LENGTH; i++) {
		sum += mPkg->Pkg[i];
	}
//	printf("%i \n",sum);
	mPkg->ChkSum = ~sum;
	*(mPkg->Pkg + CHKSUM_POS) = mPkg->ChkSum;
	return -1;
}

int SetPkgData(Package* mPkg, int* Data)
{
	int i;
	for (i = 0; i < PARAM_LENGTH; i++) {
		mPkg->Pkg[DATA_POS + i] = TrimToMaxByte((int)*Data);
		Data++;
	}
	SetChksum(mPkg);
	return -1;
}

void PrintData(Package mPkg)
{
	int i;	
	for (i = 0; i < PKGLEN; i++) {
		printf("[%x]",mPkg.Pkg[i]);
	}
	printf("\n");
}


#endif

