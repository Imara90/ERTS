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

// Indicater variables
int 	startflag = 0;
int	commflag = 0;

// If no rx IR has occured in 20 loops, go into panic_mode
int 	comm_thres = 20;

// Package bytes
BYTE 	prevmode, mode, roll, pitch, yaw, lift, pcontrol, p1control, p2control, checksum;
//char 	prevmode, mode, roll, pitch, yaw, lift, pcontrol, p1control, p2control, checksum;


// TODO Put first three modes in one file, less function calling"
#include "safe_mode.h"
#include "manual_mode.h"
#include "panic_mode.h"

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
	
	// record time
	isr_qr_time = X32_us_clock;
        inst = X32_instruction_counter;

	// get sensor and timestamp values
	s0 = X32_QR_s0; s1 = X32_QR_s1; s2 = X32_QR_s2; 
	s3 = X32_QR_s3; s4 = X32_QR_s4; s5 = X32_QR_s5;
	timestamp = X32_QR_timestamp;

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
	/*
	BYTE c;

	// signal interrupt
	toggle_led(4);


	// may have received > 1 char before IRQ is serviced so loop
	while (X32_wireless_char) {
		fifo[iptr++] = X32_wireless_data;
		if (iptr > FIFOSIZE)
			iptr = 0;
	}
	*/
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
 * Reset control values
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */
void reset_comm(void)
{
	// To make sure RPMs are not ramped up too fast
	lift 	= 0;
	roll 	= 0;
	pitch 	= 0;
	yaw 	= 0;
}

/*------------------------------------------------------------------
 * Send to TX buffer
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */
void send_tx(void)
{
	int i;
	// For the entire data log size
	for (i = 0; i < DLOGSIZE; i++)
	{	
		// Check if the tx is ready to send;
		if (X32_rs232_stat & 0x01)
		{
			X32_rs232_data = dl[i];
		}
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
		// add to the commflag to check communication
		if (commflag++ > comm_thres)
		{
			mode = PANIC_MODE;
		}		

		// See if there is a character in the buffer
		// and check whether that is the starting byte		
		c = cbGet(&cb);
		if (c == STARTING_BYTE)
		{
			decode();
			if (check_sum())
			{
//				printf("\nmode: %x, previous mode %x", mode, prevmode); 
				switch (mode)
				{
					case SAFE_MODE:
						safe_mode();
//						printf("\nSize int: %d, size char: %d, size unsigned char: %d, size short: %d, size byte: %d", sizeof(int), sizeof(char), sizeof(unsigned char), sizeof(short), sizeof(BYTE)); 
						printf("\nSafe! [%x][%x][%x][%x][%x][%x]   engines: [%d][%d][%d][%d]\n", mode, lift, roll, pitch, yaw, checksum, ae[0], ae[1], ae[2], ae[3]);	
						// safe
						break;
					case PANIC_MODE:
//						if (prev_mode != SAFE_MODE)
//						{
							panic_mode();
							printf("\nPanic! [%x][%x][%x][%x][%x][%x]   engines: [%d][%d][%d][%d]\n", mode, lift, roll, pitch, yaw, checksum, ae[0], ae[1], ae[2], ae[3]);	
							// panic
//						}
//						else 
//						{
//							mode = prev_mode;
//						}							
						break;
					case MANUAL_MODE:
//						if ((prev_mode == SAFE_MODE && (ae[0] == 0 && ae[1] == 0 && ae[2] == 0 && ae[3] == 0)) || prev_mode == MANUAL_MODE)
//						{
							manual_mode();
							printf("\nLift = %x", lift);
//							printf("\nManual! [%x][%x][%x][%x][%x][%x]   engines: [%d][%d][%d][%d]\n", mode, lift, roll, pitch, yaw, checksum, ae[0], ae[1], ae[2], ae[3]);
//printf("\nManual! [%d][%d][%d][%d][%d][%d]   engines: [%d][%d][%d][%d]\n", mode, lift, roll, pitch, yaw, checksum, ae[0], ae[1], ae[2], ae[3]);
														
							// manual
//						}						
//						else 
//						{
//							mode = prev_mode;
//						}
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
		// Safe the current mode to determine mode changes
		prevmode = mode;
//		printf("\nmode: %x, previous mode %x", mode, prevmode); 

		// Check button whether to send data to tx
		if (button == 1)
		{
			send_tx();
		}

		// Delay 20 micro second = 50 Hz according to the sending of the packages
		delay_us(20);
	}

	printf("Exit\r\n");

        DISABLE_INTERRUPT(INTERRUPT_GLOBAL);

	return 0;
}

