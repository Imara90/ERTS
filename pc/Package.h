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


int TrimToMaxLift(int value)
{
    if (value>255)
	{
        //printf("\nClipping %i to %i",value,255);
        value = 255;
	}
	else if (value<0)
	{
        //printf("\nClipping %i to %i",value,0);
        value = 0;
	}
    return value;
}
int TrimToMaxByte(int value)
{
    if (value>INT8_MAX)
	{
        //printf("\nClipping %i to %i",value,INT8_MAX);
        value = INT8_MAX;
	}
	else if (value<INT8_MIN)
	{
        //printf("\nClipping %i to %i",value,INT8_MIN);
        value = INT8_MIN;
	}
    return value;
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
    if (mMode > 9) {
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
	
	mPkg->ChkSum = ~sum;
//	printf("%x \n",mPkg->ChkSum);
	if (mPkg->ChkSum == 0x80){
		mPkg->ChkSum = 0;
	}
	*(mPkg->Pkg + CHKSUM_POS) = mPkg->ChkSum;
	return -1;
}

int SetPkgData(Package* mPkg, int* Data)
{
	int i;
    	*Data = TrimToMaxLift(*Data); //Clipping for the lift
    	mPkg->Pkg[DATA_POS] = (BYTE)*Data;
	Data++;
	for (i = 1; i < PARAM_LENGTH; i++) {
        *Data = TrimToMaxByte(*Data);
        mPkg->Pkg[DATA_POS + i] = (BYTE)*Data;
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

