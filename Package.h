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
#include "Definitions.h"


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

