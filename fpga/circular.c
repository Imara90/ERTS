#include <stdio.h>
#include <string.h>
#include <x32.h>
#include "assert.h"

//For the buffer
#include <stdlib.h>

/*********************************************************************/

/* define some peripheral short hands
 */
#define X32_instruction_counter           peripherals[0x03]

#define X32_timer_per           peripherals[PERIPHERAL_TIMER1_PERIOD]
#define X32_leds		peripherals[PERIPHERAL_LEDS]
#define X32_ms_clock		peripherals[PERIPHERAL_MS_CLOCK]
#define X32_us_clock		peripherals[PERIPHERAL_US_CLOCK]
#define X32_QR_a0 		peripherals[PERIPHERAL_XUFO_A0]
#define X32_QR_a1 		peripherals[PERIPHERAL_XUFO_A1]
#define X32_QR_a2 		peripherals[PERIPHERAL_XUFO_A2]
#define X32_QR_a3 		peripherals[PERIPHERAL_XUFO_A3]
#define X32_QR_s0 		peripherals[PERIPHERAL_XUFO_S0]
#define X32_QR_s1 		peripherals[PERIPHERAL_XUFO_S1]
#define X32_QR_s2 		peripherals[PERIPHERAL_XUFO_S2]
#define X32_QR_s3 		peripherals[PERIPHERAL_XUFO_S3]
#define X32_QR_s4 		peripherals[PERIPHERAL_XUFO_S4]
#define X32_QR_s5 		peripherals[PERIPHERAL_XUFO_S5]
#define X32_QR_timestamp 	peripherals[PERIPHERAL_XUFO_TIMESTAMP]

#define X32_rs232_data		peripherals[PERIPHERAL_PRIMARY_DATA]
#define X32_rs232_stat		peripherals[PERIPHERAL_PRIMARY_STATUS]
#define X32_rs232_char		(X32_rs232_stat & 0x02)
#define X32_rs232_txready	(X32_rs232_stat & 0x01)

#define X32_wireless_data	peripherals[PERIPHERAL_WIRELESS_DATA]
#define X32_wireless_stat	peripherals[PERIPHERAL_WIRELESS_STATUS]
#define X32_wireless_char	(X32_wireless_stat & 0x02)

#define X32_button		peripherals[PERIPHERAL_BUTTONS]
#define X32_switches		peripherals[PERIPHERAL_SWITCHES]
#define PERIPHERAL_DISPLAY	0x05
#define X32_display		peripherals[PERIPHERAL_DISPLAY]

/*********************************************************************/

// BYTE and WORD sizes predefined
#define BYTE unsigned char
#define WORD unsigned short

/*********************************************************************/
// actual size is minus one element because of the one slot open protocol 
 
// Circular buffer object 
typedef struct {
	int		size; 		/* maximum number of elements */
	int         	start;  	/* index of oldest element              */
	int	       	end;    	/* index at which to write new element  */
	BYTE	 	*elems;		/* vector of elements                   */
	// including extra element for one slot open protocol
} CBuffer;

CBuffer testcb;

/*********************************************************************/

/*****************************************************************************/
/******************************MACROS*****************************************/

/*------------------------------------------------------------------
 * Write an elemtype to buffer
 * Write an element, overwriting oldest element if buffer is full. App can
 * choose to avoid the overwrite by checking cbIsFull(). 
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */ 
#define cbWrite(CB, VAL){ 				\
    CB.elems[CB.end] = VAL; 				\
    CB.end = (CB.end + 1) % CB.size;			\
    if (CB.end == CB.start)				\
	{        					\
		CB.start = (CB.start + 1) % CB.size; 	\
	}						\
}

#define cbWriteSum(CB, VAL, sum){ 				\
    CB.elems[CB.end] = VAL; 				\
    CB.end = (CB.end + 1) % CB.size;			\
    if (CB.end == CB.start)				\
	{        					\
		CB.start = (CB.start + 1) % CB.size; 	\
	}						\
    *sum ^= VAL;					\
}

/*------------------------------------------------------------------
 * get char from buffer 
 * Read oldest element. App must ensure !cbIsEmpty() first. 
 * By Imara Speek 1506374
 *------------------------------------------------------------------  
 */
#define cbGet(CB, c) {						\
	*c = CB.elems[CB.start];				\
	CB.start = (CB.start + 1) % CB.size;			\
}


#define DLOGSIZE	50000 
BYTE   	dl[DLOGSIZE];
BYTE 	c;
BYTE	sum;
long	starttime = 0;
long 	funtiontime = 0;


void testcbInit(CBuffer *cb, int size, BYTE* array) {
	cb->size  = size + 1;
	cb->start = 0;
	cb->end   = 0;
	cb->elems = array;
}

int main()
{

	int i;

	testcbInit(&testcb, (DLOGSIZE - 1), dl);
	
	//printf("buh\n");

	sum = 0;

	starttime = X32_us_clock;
	for (i = 0; i < 10; i++)
	{
		cbWrite(testcb, i);
	}
	printf("time to print no sum: %d \n", X32_us_clock - starttime);
	
	starttime = X32_us_clock;
	for (i = 0; i < 10; i++)
	{
		cbWriteSum(testcb, i, &sum);
	}
	printf("time to print with sum: %d \n", X32_us_clock - starttime);
	

	while(testcb.end != testcb.start)
	{
		cbGet(testcb, &c);
		printf("[%d]", c);
	}
	printf("\n");	
	return 0;
}



















