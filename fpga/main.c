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
TODO change modes to enum
TODO Send values to engines
TODO determine the period for the timer interrupt
TODO manual mode
TODO panic mode
TODO safe mode
TODO check what the & bytes are for the led function in main

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

#define X32_wireless_data	peripherals[PERIPHERAL_WIRELESS_DATA]
#define X32_wireless_stat	peripherals[PERIPHERAL_WIRELESS_STATUS]
#define X32_wireless_char	(X32_wireless_stat & 0x02)

#define X32_button		peripherals[PERIPHERAL_BUTTONS]
#define X32_switches		peripherals[PERIPHERAL_SWITCHES]

/*********************************************************************/

// BYTE and WORD sizes predefined
#define BYTE unsigned char
#define WORD unsigned short

// RX FIFO
#define FIFOSIZE 16
char	fifo[FIFOSIZE]; 
int	iptr, optr;

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

#define STARTING_BYTE			0x80

// Parameter list numbering
#define MODE		0x00
#define LIFT		0x01
#define ROLL		0x02
#define PITCH		0x03
#define YAW		0x04

#define PCONTROL	0x01
#define P1CONTROL	0x02
#define P2CONTROL	0x03

#define CHECKSUM	0x05

//RAMP-UP CHECK PARAMETERS
#define SAFE_INCREMENT 50

//BUTTERWORTH LOW PASS FILTER CONSTANTS
//for 25Hz cut-off frequency and 1266.5 Hz sampling freq.
//#define A0		969
//#define A1		969
//#define B0		16384
//#define B1		-14444
//for 10Hz cut-off frequency and 1266.5 Hz sampling freq.
#define A0		401
#define A1		401
#define B0		16384
#define B1		-15580


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

CircularBuffer cb;

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

int 	startflag = 0;
int	commflag = 1;
BYTE 	prev_mode, mode, roll, pitch, yaw, lift, pcontrol, p1control, p2control, checksum;

#include "safe_mode.h"
#include "manual_mode.h"

/*------------------------------------------------------------------
 * Fixed Point Multiplication
 * Multiplies the values and then shift them right by 14 bits
 * By Daniel Lemus
 *------------------------------------------------------------------
 */
unsigned int mult(unsigned int a,unsigned int b)
{
	unsigned int result;
	result = a * b;
	result = (result >> 14);
	return result;
}


/*------------------------------------------------------------------
 * 1st Order Butterworth filter
 * By Daniel Lemus
 *------------------------------------------------------------------
 */

void Butt2Filter()
{
	int i;
	for (i=0; i<6; i++) {
		y0[i] = (mult(A0,x0[i]) + mult(A1,x1[i]) - mult(B1,y1[i]));
		x1[i] = x0[i];
		x1[i] = x0[i];
	}
}

/*------------------------------------------------------------------
 * Data Logging Storage
 * Store each parameter individually in arrays
 * By Daniel Lemus
 *------------------------------------------------------------------
 */
void DataStorage(void)
{
	BYTE sum;
	int i;
	sum = 0;
	if (dl_count < DLOGSIZE) {
		//stores the starting bytes
		dl[dl_count++] = 0x80;
		dl[dl_count++] = 0x00;
		//stores time stamp
		dl[dl_count++] = timestamp;
		// Stores desired variables (Change if needed)
		// e.g filtered values
		for (i=0;i<6;i++){
			dl[dl_count++] = x0[i];
		}
		sum = timestamp + x0[0] + x0[1] + x0[2] + x0[3] + x0[4] + x0[5];
		sum = ~sum;
		dl[dl_count++] = sum;
	}
	
	//to send back it is necessary to typecast to BYTE
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
 * isr_rs232_tx -- QR link tx interrupt handler
 * By Daniel Lemus
 *------------------------------------------------------------------
 */
void isr_rs232_tx(void)
{
	//X32_rs232_data
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

/*------------------------------------------------------------------
 * Clean the buffer 
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */ 
void cbClean(CircularBuffer *cb) {
	memset(cb->elems, 0, sizeof(ElemType) * CB_SIZE); 	
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

/*------------------------------------------------------------------
 * Read from buffer and store in elem
 * Read oldest element. App must ensure !cbIsEmpty() first. 
 * By Imara Speek 1506374 
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
		cb.elems[cb.end].value = (BYTE)X32_rs232_data;
		cb.end = (cb.end + 1) % CB_SIZE;
		if (cb.end == cb.start)
		{
			cb.start = (cb.start + 1) % CB_SIZE; /* full, overwrite */
		}
// TODO determine if we want it to overwrite		
	}

}


/*------------------------------------------------------------------
 * isr_wireless_rx -- wireless rx interrupt handler - not used
 *------------------------------------------------------------------
 */
void isr_wireless_rx(void)
{
	BYTE c;

	// signal interrupt
	toggle_led(4);


	// may have received > 1 char before IRQ is serviced so loop
	while (X32_wireless_char) {
		fifo[iptr++] = X32_wireless_data;
		if (iptr > FIFOSIZE)
			iptr = 0;
	}

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

	DISABLE_INTERRUPT(INTERRUPT_GLOBAL); 

	// Safe the current mode to determine mode changes
	prev_mode = mode;

	// Take the value from the buffer and reset the elem
	// buffer value to make sure it isn't read multiple times
	mode 	= cbGet(&cb);
	lift 	= cbGet(&cb);
	roll 	= cbGet(&cb);
	pitch 	= cbGet(&cb);
	yaw 	= cbGet(&cb);
/*
	pcontrol 	= getchar();
	p1control 	= getchar();
	p2control 	= getchar();
 */
	checksum = cbGet(&cb);

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
	sum = 0;

	sum = mode + lift + roll + pitch + yaw; // + pcontrol + p1control + p2control;
	sum = ~sum;

	if (checksum != sum) {
//		printf("\nInvalid Pkg");
		return 0;
	}
	else
		return 1;
}

/*------------------------------------------------------------------
 * Check the mode and reset parameters if mode has changed
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */
void check_mode(void)
{
	if (mode != prev_mode)
	{
		lift 	= 0;
		roll 	= 0;
		pitch 	= 0;
		yaw 	= 0;
		pcontrol 	= 0;
		p1control 	= 0;
		p2control 	= 0;
	}
}

/*------------------------------------------------------------------
 * main 
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */
int main() 
{
	// Character to store bytes from buffer	
	BYTE c;
	// Initialize the Circular buffer and elem to write from
	ElemType elem;

	// prepare QR rx interrupt handler
        SET_INTERRUPT_VECTOR(INTERRUPT_XUFO, &isr_qr_link);
        SET_INTERRUPT_PRIORITY(INTERRUPT_XUFO, 21);
	isr_qr_counter = isr_qr_time = 0;
	ae[0] = ae[1] = ae[2] = ae[3] = 0;
        ENABLE_INTERRUPT(INTERRUPT_XUFO);
 	
	// timer interrupt
	// TODO find most optimal timing interval for this
        //X32_timer_per = 5 * CLOCKS_PER_MS;
        //SET_INTERRUPT_VECTOR(INTERRUPT_TIMER1, &isr_qr_timer);
        //SET_INTERRUPT_PRIORITY(INTERRUPT_TIMER1, 21);
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

        // prepare wireless rx interrupt and getchar handler
        SET_INTERRUPT_VECTOR(INTERRUPT_WIRELESS_RX, &isr_wireless_rx);
        SET_INTERRUPT_PRIORITY(INTERRUPT_WIRELESS_RX, 19);
        while (X32_wireless_char) c = X32_wireless_data; // empty buffer
        ENABLE_INTERRUPT(INTERRUPT_WIRELESS_RX);

	// initialize some other stuff
	X32_leds = 0;
	program_done = 0;

	// clean the buffer
	cbClean(&cb);
	// initialize the buffer
	cbInit(&cb);
	// Initialize value to write
	elem.value = 0;

	// Print to indicate start
	printf("Hello! \nMode, Parameter 1, Parameter 2, Parameter 3, Parameter 4, Checksum");

	// Enable all interrupts, starting the system
        ENABLE_INTERRUPT(INTERRUPT_GLOBAL); 

	while (! program_done) {
		// reset the commflag to check communication
		commflag++;		

		// See if there is a character in the buffer
		// and check whether that is the starting byte		
		c = cbGet(&cb);
		if (c == STARTING_BYTE)
		{
			decode();
			if (check_sum())
			{
//				printf("\nYay! [%x][%x][%x][%x][%x][%x]\n", mode, lift, roll, pitch, yaw, checksum);
				printf("\nmode: %x", mode); 
				switch (mode)
				{
					case SAFE_MODE:
						safe_mode();
						printf("\nSafe! [%x][%x][%x][%x][%x][%x]   engines: [%d][%d][%d][%d]\n", mode, lift, roll, pitch, yaw, checksum, ae[0], ae[1], ae[2], ae[3]);	
						// safe
						break;
					case PANIC_MODE:
						// panic
						break;
					case MANUAL_MODE:
						check_mode();
						manual_mode();
						printf("\nManual! [%x][%x][%x][%x][%x][%x]   engines: [%d][%d][%d][%d]\n", mode, lift, roll, pitch, yaw, checksum, ae[0], ae[1], ae[2], ae[3]);
						// manual
						break;
					case CALIBRATION_MODE:
						// calibrate
						break;
					case YAW_CONTROL_MODE:
						// yaw
						break;
					case FULL_CONTROL_MODE:
						// full
						break;
					case P_CONTROL_MODE:
						// p
						break;
					default :
						// safe
						break;
				}			
			}
		}
// TODO switch case in or out all the if statements?  
		// Delay 20 micro second = 50 Hz according to the sending of the packages
		delay_us(20);
	}

	printf("Exit\r\n");

        DISABLE_INTERRUPT(INTERRUPT_GLOBAL);

	return 0;
}

