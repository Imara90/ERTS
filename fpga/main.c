/*------------------------------------------------------------------
 *  based on the qtest - fixing all the todos
 *
 *  reads ae[0-3] from stdin
 *  (q,w,e,r increment ae[0-3], a,s,d,f decrement)
 *
 *  prints ae[0-3],sax,say,saz,sp,sq,sr,delta_t on stdout
 *  where delta_t is the qr-isr exec time
 *
 *  Imara Speek
 *  Embedded Real Time Systems
 *
 *  Version Feb 26, 2014

TODO determine priorities
TODO control values have to be send
TODO ask for log that is saved during running
TODO also want telemetry set and concurring protocol

 *------------------------------------------------------------------
 */

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

/*********************************************************************/

// BYTE and WORD sizes predefined
#define BYTE unsigned char
#define WORD unsigned short

// mode, lift ,roll, pitch, yaw, checksum 
#define nParams		0x06

// MODES LIST
#define SAFE_MODE			0x00
#define PANIC_MODE			0x01
#define MANUAL_MODE			0x02
#define CALIBRATION_MODE		0x03
#define YAW_CONTROL_MODE		0x04
#define FULL_CONTROL_MODE		0x05
#define P_CONTROL_MODE			0x06
#define ABORT_MODE					0x07
#define STARTING_BYTE			0x80

// Parameter list numbering
#define MODE		0x00
#define LIFT		0x01
#define ROLL		0x02
#define PITCH		0x03
#define YAW		0x04

#define PCONTROL	0x02
#define P1CONTROL	0x03
#define P2CONTROL	0x04

#define CHECKSUM	0x05

//RAMP-UP CHECK PARAMETERS
#define SAFE_INCREMENT 50

//BUTTERWORTH LOW PASS FILTER CONSTANTS
//for 25Hz cut-off frequency and 1266.5 Hz sampling freq.
#define A0		969
#define A1		969
#define B0		16384
#define B1		14444
//for 10Hz cut-off frequency and 1266.5 Hz sampling freq.
/*#define A0		401
#define A1		401
#define B0		16384
#define B1		15580*/


////filter temporary variables
int   y0[6] = {0,0,0,0,0,0};
int   y1[6] = {0,0,0,0,0,0};
int   y2[6] = {0,0,0,0,0,0};
int   x0[6] = {0,0,0,0,0,0};
int   x1[6] = {0,0,0,0,0,0};
int   x2[6] = {0,0,0,0,0,0};


//DEFINE SIZE OF DATA LOGGING VARIABLES
#define DLOGSIZE	50000 
//data logging variables
int   dl[DLOGSIZE];
int   dl_count = 0;
//#define LogParams	

//initialize previous state (To prevent ramp-up)
int   prev_ae[4] = {0, 0, 0, 0};

/*********************************************************************/

// For defining the circular buffer
// Opaque buffer element type.  This would be defined by the application
typedef struct { BYTE value; } ElemType;

// fixed size for the buffer, no dyanmic allocation is needed
// actual size is minus one element because of the one slot open protocol 
#define CB_SIZE (62 + 1)
 
// Circular buffer object 
typedef struct {
	int         	start;  	/* index of oldest element              */
	int	       	end;    	/* index at which to write new element  */
	ElemType 	elems[CB_SIZE]; /* vector of elements                   */
	// including extra element for one slot open protocol
} CircularBuffer;

CircularBuffer txcb, rxcb;

/*********************************************************************/

/*********************************************************************/
// TODO have to modify this in onder to have good code

// fixed size for the buffer, no dyanmic allocation is needed
// actual size is minus one element because of the one slot open protocol 
#define CBDATA_SIZE (50000 + 1)
 
// Circular buffer object 
typedef struct {
	int         	start;  	/* index of oldest element              */
	int	       	end;    	/* index at which to write new element  */
	ElemType 	elems[CBDATA_SIZE]; /* vector of elements                   */
	// including extra element for one slot open protocol
} CircularDataBuffer;

CircularDataBuffer dscb;

/*********************************************************************/

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

// Globals
char	c;
int	program_done;
int	ae[4];
int	s0, s1, s2, s3, s4, s5, timestamp;
int	isr_qr_counter;
int	isr_qr_time;
int	button;
int	inst;

void	toggle_led(int);
void	delay_ms(int);
void	delay_us(int);

// Own functions
void	decode();
void 	print_comm();
void 	check_start();
int 	check_sum();

// Flags and communication variables
int 	startflag = 0;
int	commflag = 1;
int 	commthres = 1000;

// telemetry
int	polltell = 1;
int 	pollthres = 15;
long 	polltime = 0;


BYTE 	package[nParams];
BYTE 	sel_mode, roll, pitch, yaw, lift, pcontrol, p1control, p2control, checksum;
BYTE 	prev_mode = 0;//VARIABLE TO SAVE PREVIOUS MODE
BYTE	mode = 0;//ACTUAL OPERATING MODE
BYTE	last_control_mode = 0;//VARIABLE TO SAVE LAST_CONTROL_MODE( 4 || 5)

// Own written functions
#include "safe_mode.h"
#include "manual_mode.h"
#include "panic_mode.h"
#include "calibration_mode.h"

// Time variables for integration
int t0 = 0;
int t1 = 0;
int dt = 0;
int integral[3] = {0,0,0};

/*------------------------------------------------------------------
 * Fixed Point Multiplication
 * Multiplies the values and then shift them right by 14 bits
 * By Daniel Lemus
 *------------------------------------------------------------------
 */
int mult(int a, int b)
{
	unsigned int result;
	result = a * b;
	result = (result >> 14);
//	printf("\nresult(%i * %i) = %i,",a,b,result );
 	return result;
}

/*------------------------------------------------------------------
 * 1st Order Butterworth filter
 * By Daniel Lemus
 *------------------------------------------------------------------
 */
void Butt2Filter(void)
{
	int i;
	for (i=0; i<6; i++) {
		y0[i] = (mult(A0,x0[i]) + mult(A1,x1[i]) + mult(B1,y1[i]));
 		x1[i] = x0[i];
		y1[i] = y0[i];
	}
}

/*------------------------------------------------------------------
 * Ramp-Up prevention function
 * Compares current - previous commanded speed and clip the current
 * value if necessary (To avoid sudden changes -> motor ramp-up)
 * By Daniel Lemus
 *------------------------------------------------------------------
 */
void CheckMotorRamp(void)
{
	int delta,i;
	for (i = 0; i < 4; i++) {
		delta = ae[i]-prev_ae[i];
		if (abs(delta) > SAFE_INCREMENT) {
			if (delta < 0) // Negative Increment
			{
				ae[i] = prev_ae[i] - SAFE_INCREMENT;
			}
			else //POSITIVE INCREMENT
			{
				ae[i] = prev_ae[i] + SAFE_INCREMENT;
			}
		}
		prev_ae[i] = ae[i];
	}
}

/*------------------------------------------------------------------
 * Circular buffer initialization 
 * Point start and end to the adress of the allocated vector of elements
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */
void cbInit(CircularBuffer *cb) {
    cb->start = 0;
    cb->end   = 0;
}
void dscbInit(CircularDataBuffer *cb) {
    cb->start = 0;
    cb->end   = 0;
}

void testcbInit(CBuffer *cb, int size) {
	cb->size  = size + 1;
	cb->start = 0;
	cb->end   = 0;
}

/*------------------------------------------------------------------
 * Check if circular buffer is full - not used
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */ 
int cbIsFull(CircularBuffer *cb) {
    return (cb->end + 1) % CB_SIZE == cb->start;
}


/*------------------------------------------------------------------
 * Check if circular buffer is empty - not used
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */ 
int cbIsEmpty(CircularBuffer *cb) {
    return cb->end == cb->start;
}
int testcbIsEmpty(CBuffer *cb) {
    return cb->end == cb->start;
}

/*------------------------------------------------------------------
 * Clean the buffer 
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */ 
void cbClean(CircularBuffer *cb) {
	memset(cb->elems, 0, sizeof(ElemType) * CB_SIZE); 	
}
void dscbClean(CircularDataBuffer *cb) {
	memset(cb->elems, 0, sizeof(ElemType) * CBDATA_SIZE); 	
}

void testcbClean(CBuffer *cb) {
	memset(cb->elems, 0, sizeof(ElemType) * cb->size); 	
}


/*------------------------------------------------------------------
 * Write an elemtype to buffer
 * Write an element, overwriting oldest element if buffer is full. App can
 * choose to avoid the overwrite by checking cbIsFull(). 
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */ 
void cbWrite(CircularBuffer *cb, ElemType *elem) {
    cb->elems[cb->end] = *elem;
    cb->end = (cb->end + 1) % CB_SIZE;
    if (cb->end == cb->start)
	{        
		cb->start = (cb->start + 1) % CB_SIZE; /* full, overwrite */
	}
// TODO determine if we want it to overwrite
}
void dscbWrite(CircularDataBuffer *cb, ElemType *elem) {
    cb->elems[cb->end] = *elem;
    cb->end = (cb->end + 1) % CBDATA_SIZE;
    if (cb->end == cb->start)
	{        
		cb->start = (cb->start + 1) % CBDATA_SIZE; /* full, overwrite */
	}
// TODO determine if we want it to overwrite
}

/*------------------------------------------------------------------
 * Read from buffer and store in elem
 * Read oldest element. App must ensure !cbIsEmpty() first. 
 * By Imara Speek 1506374 - not used
 *------------------------------------------------------------------  
 */
void cbRead(CircularBuffer *cb, ElemType *elem) {
    *elem = cb->elems[cb->start];
    cb->start = (cb->start + 1) % CB_SIZE;
}

/*------------------------------------------------------------------
 * get char from buffer 
 * Read oldest element. App must ensure !cbIsEmpty() first. 
 * By Imara Speek 1506374
 *------------------------------------------------------------------  
 */
BYTE cbGet(CircularBuffer *cb) {
	BYTE c;	

	c = cb->elems[cb->start].value;
	// Whenever the starting byte is read, the package is decoded
	// To make sure the same package isn't read twice, we overwrite
	// the starting byte. The package will still decode because of c
	// and it will not be recognized again. 
	if (c == STARTING_BYTE)
	{
		cb->elems[cb->start].value = 0;	
	}
	cb->start = (cb->start + 1) % CB_SIZE;

	return c;
}

BYTE dscbGet(CircularDataBuffer *cb) {
	BYTE c;	

	c = cb->elems[cb->start].value;
	// Whenever the starting byte is read, the package is decoded
	// To make sure the same package isn't read twice, we overwrite
	// the starting byte. The package will still decode because of c
	// and it will not be recognized again. 
	cb->start = (cb->start + 1) % CBDATA_SIZE;

	return c;
}

/*------------------------------------------------------------------
 * isr_qr_timer -- QR timer interrupt handler - not used
 * By Imara Speek - 1506374
 *------------------------------------------------------------------
 */
void isr_qr_timer(void)
{

}

/*------------------------------------------------------------------
 * isr_qr_link -- QR link rx interrupt handler
 *------------------------------------------------------------------
 */
void isr_button(void)
{
	button = 1;
}

/*------------------------------------------------------------------
 * isr_qr_link -- QR link rx interrupt handler
 *------------------------------------------------------------------
 */
void isr_qr_link(void)
{
	int	ae_index;
	int     i, max[6],min[6];
	
	// record time
	isr_qr_time = X32_us_clock;
        inst = X32_instruction_counter;

	// get sensor and timestamp values
	x0[0] = X32_QR_s0; x0[1] = X32_QR_s1; x0[2] = X32_QR_s2; 
	x0[3] = X32_QR_s3; x0[4] = X32_QR_s4; x0[5] = X32_QR_s5;
	timestamp = X32_QR_timestamp;
	
	Butt2Filter();
	
	//Gets the maximum value
	/*for (i=0;i<6;i++)
	{
		if (x0[i] > max[i])
		{
			max[i] = x0[i];
		}
		if (x0[i] < min[i])
		{
			min[i] = x0[i];
		}
	}*/
		
	/*printf("Unfiltered ... s0 = %i s1 = %i s2 = %i s3 = %i s4 = %i s5 = %i \n",s0,s1,s2,s3,s4,s5);
	printf("filtered ..... s0 = %i s1 = %i s2 = %i s3 = %i s4 = %i s5 = %i \n",y0[1],y0[2],y0[3],y0[4],y0[5],y0[6]);
   	printf("size = %i \n",sizeof(s0));*/
	//printf("max = [%i][%i][%i][%i][%i][%i] \n",max[0],max[1],max[2],max[3],max[4],max[5]);


	// monitor presence of interrupts 
	isr_qr_counter++;
	if (isr_qr_counter % 500 == 0) 
	{
		toggle_led(2);
	}	

	// Clip engine values to be positive and 10 bits.
	for (ae_index = 0; ae_index < 4; ae_index++) 
	{
		if (ae[ae_index] < 0)
		{ 
			ae[ae_index] = 0;
		}
		ae[ae_index] &= 0x3ff;
	}

	// Send actuator values
	// (Need to supply a continous stream, otherwise
	// QR will go to safe mode, so just send every ms)
	X32_QR_a0 = ae[0];
	X32_QR_a1 = ae[1];
	X32_QR_a2 = ae[2];
	X32_QR_a3 = ae[3];

	// record isr execution time (ignore overflow)
        inst = X32_instruction_counter - inst;
	isr_qr_time = X32_us_clock - isr_qr_time;
}

/*------------------------------------------------------------------
 * isr_rs232_rx -- rs232 rx interrupt handler 
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */
void isr_rs232_rx(void)
{
	// Reset the communication flag
	commflag = 0;

	// signal interrupt
	toggle_led(3);

	// may have received > 1 char before IRQ is serviced so loop
	while (X32_rs232_char) 
	{
		rxcb.elems[rxcb.end].value = (BYTE)X32_rs232_data;
		rxcb.end = (rxcb.end + 1) % CB_SIZE;
		if (rxcb.end == rxcb.start)
		{
			rxcb.start = (rxcb.start + 1) % CB_SIZE; /* full, overwrite */
		}	
	}
}

/*------------------------------------------------------------------
 * isr_rs232_tx -- rs232 tx interrupt handler 
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */
void isr_rs232_tx(void)
{
	// signal interrupt
	toggle_led(4);

}

/*------------------------------------------------------------------
 * delay_ms -- busy-wait for ms milliseconds
 *------------------------------------------------------------------
 */
void delay_ms(int ms) 
{
	int time = X32_ms_clock;
	while(X32_ms_clock - time < ms)
		;
}

/*------------------------------------------------------------------
 * delay_us -- busy-wait for us microseconds
 *------------------------------------------------------------------
 */
void delay_us(int us) 
{
	int time = X32_us_clock;
	while(X32_us_clock - time < us)
		;
}

/*------------------------------------------------------------------
 * toggle_led -- toggle led # i
 *------------------------------------------------------------------
 */
void toggle_led(int i) 
{
	X32_leds = (X32_leds ^ (1 << i));
}


/*------------------------------------------------------------------
 * Decoding function with a higher execution level
 * Function called in the timer ISR 
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */
void decode(void)
{	
	/* Get the next character in the buffer after the starting byte
	 * Whilst disabling all the interrupts CRITICAL SECTION
	 */
	int i;

	DISABLE_INTERRUPT(INTERRUPT_GLOBAL); 

	// Safe the current mode to determine mode changes
	prev_mode = package[MODE];

	// Take the value from the buffer and reset the elem
	// buffer value to make sure it isn't read multiple times
	sel_mode = cbGet(&rxcb);
	for (i = 1; i < nParams; i++)
	{
		package[i] = cbGet(&rxcb);
	}

	if( sel_mode == SAFE_MODE || sel_mode == ABORT_MODE || (prev_mode == SAFE_MODE && (ae[0] == 0 && ae[1] == 0 && ae[2] == 0 && ae[3] == 0 && package[LIFT] == 0)) && sel_mode != PANIC_MODE && ((sel_mode != FULL_CONTROL_MODE && sel_mode != YAW_CONTROL_MODE) || calibration_done == 1) && (sel_mode != CALIBRATION_MODE || calibration_counter == 0) || (sel_mode == PANIC_MODE && prev_mode != SAFE_MODE) || (sel_mode == P_CONTROL_MODE && (prev_mode == YAW_CONTROL_MODE || prev_mode == FULL_CONTROL_MODE)) || (prev_mode == P_CONTROL_MODE && sel_mode == last_control_mode) ) 
	{
							package[MODE] = sel_mode;
	}
	else package[MODE] = prev_mode;
	
	
	ENABLE_INTERRUPT(INTERRUPT_GLOBAL); 
}


/*------------------------------------------------------------------
 * Check the checksum and return error message is package is corrupted
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */
int check_sum(void)
{
	BYTE sum;
	int i;

	sum = sel_mode;

	// independent of the package structure
	for (i = 1; i < (nParams - 1); i++)
	{
		sum += package[i];
	}	
	sum = ~sum;
	
	
	if (package[CHECKSUM] != sum) {
		return 0;
	}
	else
		return 1;
}

#define storenosensor 12
#define storeall 24

/*------------------------------------------------------------------
 * Data Logging Storage
 * Store each parameter individually in arrays
 * by Imara Speek 1506374
 *------------------------------------------------------------------
 */
void store_data(void)
{
	BYTE sum;
	int i, j;
	BYTE storing[storenosensor];
	sum = 0;
	
	// determine the checksum for the send package
	sum = X32_ms_clock + package[MODE] + package[LIFT] + package[ROLL] + 				package[PITCH] + package[YAW] + ae[0] + ae[1] + ae[2] + ae[3]; /* + 
				 x0[0] + x0[1] + x0[2] + x0[3] + x0[4] + x0[5]; */
	sum = ~sum;

	// TODO find a way to save P values if the mode has changed
	
	j = 0;
	storing[j++] = 0x80;
	// the ms clock is actually 4 bytes, so takes least significant 2 bytes and log
	storing[j++] = X32_ms_clock >> 8;
	storing[j++] = X32_ms_clock;
	storing[j++] = package[MODE];
	storing[j++] = package[LIFT];
	storing[j++] = package[ROLL];
	storing[j++] = package[YAW];
	storing[j++] = package[PITCH];
	storing[j++] = ae[0];
	storing[j++] = ae[1];
	storing[j++] = ae[2];
	storing[j++] = ae[3];
/*
	storing[j++] = x0[0] >> 8;
	storing[j++] = x0[0];
	storing[j++] = x0[1] >> 8;
	storing[j++] = x0[1];
	storing[j++] = x0[2] >> 8;
	storing[j++] = x0[2];
	storing[j++] = x0[3] >> 8;
	storing[j++] = x0[3];
	storing[j++] = x0[4] >> 8;
	storing[j++] = x0[4];
	storing[j++] = x0[5] >> 8;
	storing[j++] = x0[5];
	storing[j++] = y0[0];
	storing[j++] = y0[1];
	storing[j++] = y0[2];
	storing[j++] = y0[3];
	storing[j++] = y0[4];
	storing[j++] = y0[5];
*/
	storing[j++] = sum;

	for (i = 0; i < 12; i++)
	{
		// make sure only starting byte can be 0x80
		if ((i != 0) && (storing[i] == 0x80))
		{
			// if the value is -128, correct it to -127
			storing[i] = 0x81;
		}
		dscb.elems[dscb.end].value = storing[i];
		//printf(" %x, ", storing[i]);
		dscb.end = (dscb.end + 1) % CBDATA_SIZE;
		if (dscb.end == dscb.start)
		{
			dscb.start = (dscb.start + 1) % CBDATA_SIZE; // full, overwrite 
		}
	}
	
}

/*------------------------------------------------------------------
 * Data sending of log
 * Store each parameter individually in arrays
 * by Imara Speek 1506374
 *------------------------------------------------------------------
 */
void send_data(void)
{
	// send data from the data log untill it is empty
	while (dscb.end != dscb.start)
	{

		if (X32_rs232_txready)
		{
			X32_rs232_data = dscb.elems[dscb.start].value;
			dscb.start = (dscb.start + 1) % CBDATA_SIZE;
		}	
	}

}

/*------------------------------------------------------------------
 * Send the telemetry at 10 Hz max 
 * by Imara Speek 1506374
 *------------------------------------------------------------------
 */
void send_telemetry(void)
{
	BYTE telem[nParams];
	int j, i, sum;
	sum = 0;

	// Checks if more than or 100 ms have passed since the last 
	// telemetry
	if (X32_ms_clock - polltime >= 100)
	{
		j = 0;
		telem[j++] = STARTING_BYTE;
		telem[j++] = X32_ms_clock >> 8;
		telem[j++] = package[MODE];
		telem[j++] = package[LIFT];
		telem[j++] = package[ROLL];
		telem[j++] = package[PITCH];
		telem[j++] = package[YAW];

		// calculate the checksum, dont include starting byte
		for (i = 1; i < j ; i++)
		{
			sum += telem[i];
		}
		sum = ~sum;
		telem[j++] = sum;

		// send the data
		for (i = 0; i < j; i++)
		{
			// wait untill tx is ready to send
			while ( !X32_rs232_txready ) ;

			X32_rs232_data = telem[i];
			dscb.start = (dscb.start + 1) % CBDATA_SIZE;
		}
/*
		if (X32_rs232_txready)
		{
			X32_rs232_data = X32_ms_clock;
			dscb.start = (dscb.start + 1) % CBDATA_SIZE;
		}
*/	
		polltime = X32_ms_clock;
	}
	
}

/*------------------------------------------------------------------
 * main 
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */
int main() 
{
	int i;	
	// Character to store bytes from buffer	
	BYTE c;
	// Initialize the Circular buffer and elem to write from
	ElemType elem;

	BYTE testelems[8];

	// prepare QR rx interrupt handler
        SET_INTERRUPT_VECTOR(INTERRUPT_XUFO, &isr_qr_link);
        SET_INTERRUPT_PRIORITY(INTERRUPT_XUFO, 21);
	isr_qr_counter = isr_qr_time = 0;
	ae[0] = ae[1] = ae[2] = ae[3] = 0;
        ENABLE_INTERRUPT(INTERRUPT_XUFO);
 	
	// timer interrupt - less high priority
        //X32_timer_per = 100 * CLOCKS_PER_MS;
        //SET_INTERRUPT_VECTOR(INTERRUPT_TIMER1, &isr_qr_timer);
        //SET_INTERRUPT_PRIORITY(INTERRUPT_TIMER1, 18);
        //ENABLE_INTERRUPT(INTERRUPT_TIMER1);

	// prepare button interrupt handler
        SET_INTERRUPT_VECTOR(INTERRUPT_BUTTONS, &isr_button);
        SET_INTERRUPT_PRIORITY(INTERRUPT_BUTTONS, 8);
	button = 0;
        ENABLE_INTERRUPT(INTERRUPT_BUTTONS);	

	// prepare rs232 rx interrupt and getchar handler
        SET_INTERRUPT_VECTOR(INTERRUPT_PRIMARY_RX, &isr_rs232_rx);
        SET_INTERRUPT_PRIORITY(INTERRUPT_PRIMARY_RX, 20);
	while (X32_rs232_char) c = X32_rs232_data; // empty buffer
        ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);

	// prepare rs232 tx interrupt and getchar handler
        SET_INTERRUPT_VECTOR(INTERRUPT_PRIMARY_TX, &isr_rs232_tx);
        SET_INTERRUPT_PRIORITY(INTERRUPT_PRIMARY_TX, 15);
        ENABLE_INTERRUPT(INTERRUPT_PRIMARY_TX);        

        // prepare wireless rx interrupt and getchar handler
        //SET_INTERRUPT_VECTOR(INTERRUPT_WIRELESS_RX, &isr_wireless_rx);
        //SET_INTERRUPT_PRIORITY(INTERRUPT_WIRELESS_RX, 19);
        //while (X32_wireless_char) c = X32_wireless_data; // empty buffer
        //ENABLE_INTERRUPT(INTERRUPT_WIRELESS_RX);

	// initialize some other stuff
	X32_leds = 0;
	program_done = 0;

	// clean the buffer
	cbClean(&rxcb);
	cbClean(&txcb);
	dscbClean(&dscb);
	// initialize the buffer
	cbInit(&rxcb);
	cbInit(&txcb);
	dscbInit(&dscb);


	// DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG 
	testcbInit(&testcb, 7);
	testcb.elems = testelems;
	testcbClean(&testcb);

	// Initialize value to write
	//elem.value = 0;

	
	for (i = 0; i < nParams; i++)
	{
		package[i] = 0;

		// DEBUG DEBUG DEBUG
/*
		testcb.elems[testcb.end] = i;
		testcb.end = (testcb.end + 1) % testcb.size;
		if (testcb.end == testcb.start)
		{
			testcb.start = (testcb.start + 1) % testcb.size; // full, overwrite 
		}
*/
	}

	// Print to indicate start
	//printf("Hello! \nMode, Parameter 1, Parameter 2, Parameter 3, Parameter 4, Checksum");

	// Enable all interrupts, starting the system
        ENABLE_INTERRUPT(INTERRUPT_GLOBAL); 

	while (! program_done) {		
		// reset the commflag to check communication
		if (commflag++ > commthres)
		{
			package[MODE] = PANIC_MODE;
		}	

		// See if there is a character in the buffer
		// and check whether that is the starting byte		
		c = cbGet(&rxcb);
		if (c == STARTING_BYTE)
		{
			decode();
			if (check_sum())
			{
				//store_data();
				// Check if data is ready to be sent, and send	
/*	
				if (X32_rs232_stat & 0x01)
				{
					//X32_rs232_data = package[CHECKSUM];//package[txcount+1];
					//X32_rs232_data =  sizeof(X32_ms_clock);
					//X32_rs232_data = sizeof(BYTE);
					if(!testcbIsEmpty(&testcb))
					{
						X32_rs232_data = testcb.elems[testcb.start];
						testcb.start = (testcb.start + 1) % testcb.size;
					}				
				}		
*/

				switch (package[MODE])
				{
					case SAFE_MODE:
						safe_mode();
// TODO isn't this better in the safe mode function?
						if (sel_mode == SAFE_MODE) calibration_counter = 0; //Sets that the user can enter again calibration mode
						last_control_mode = 0; //reset last_control_mode variable
						// safe
						break;
					case PANIC_MODE:
						panic_mode();	
						// panic
						break;
					case MANUAL_MODE:
						manual_mode();
						// manual
						break;
					case CALIBRATION_MODE:
						calibration_mode();
						// calibrate
						break;
					case YAW_CONTROL_MODE:
						last_control_mode = YAW_CONTROL_MODE;						
						// yaw
						break;
					case FULL_CONTROL_MODE:
						last_control_mode = FULL_CONTROL_MODE;						
						// full
						break;
					case P_CONTROL_MODE:
						// p
						break;
					case ABORT_MODE:
						program_done++;
// TODO, doesn't it has to change into panic mode? Maybe abort mode is not the right word
// if this needs to be done, it has to be done as in the panic mode, with care, not force
						for (i=0; i < 4; i++) ae[i]=0;
						break;					
					default :
						// safe
						break;
				}
				
				// if the package was correct, store the correct data
				store_data();
				// sends the telemetry at 10Hz
				send_telemetry();			
			}
		}
	}

	// send the data log to the pc
	send_data();	

        DISABLE_INTERRUPT(INTERRUPT_GLOBAL);

	return 0;
}

