/*------------------------------------------------------------------
 *  Imara Speek
 *  Embedded Real Time Systems
 *
 *  Version Feb 26, 2014
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
#define PERIPHERAL_DISPLAY	0x05
#define X32_display		peripherals[PERIPHERAL_DISPLAY]

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
/*
#define A0		969
#define A1		969
#define B0		16384
#define B1		14444
*/
#define A0		2240
#define A1		2240
#define B0		16384
#define B1		11903
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


//KALMAN FILTER CONSTANTS
#define P2PHI   8192 //0.5 * 2¹⁴
#define C1      7 // 2⁷ = 123
#define C2      10 // 2¹⁰ = 1023

//KALMAN FILTER GLOBAL VARIABLES
int   sp = 0;
int   sphi = 0;
int   sq = 0;
int   stheta = 0;
int   p_b = 0;
int   q_b = 0;

//initialize previous state (To prevent ramp-up)
int   prev_ae[4] = {0, 0, 0, 0};


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

CBuffer testcb, txcb, rxcb, dscb;

/*********************************************************************/

// Globals
int	program_done;
int	ae[4];
int	s0, s1, s2, s3, s4, s5, timestamp;
int	isr_qr_counter;
int	isr_qr_time;
int	button;
int	inst;
int 	sumae;

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
BYTE	telemetry_flag = 0x00;

// Profiling variables
long 	controltime = 0;
long 	maxtime = 0;
long 	storetime = 0;
long	functiontime = 0;
long	starttime = 0;

// array to save rx packet in 
BYTE 	package[nParams];

//GLOBALS FOR CONTROL MODES
long int Z = 0;		// LIFT FORCE
long int L = 0;		// ROLL MOMENTUM
long int M = 0;		// PICTH MOMENTU
long int N = 0;		// YAW MOMENTUM
int 	phi = 0; 	// ROLL ANGLE
int 	theta = 0; 	// PITCH ANGLE
int 	p = 0; 		// ROLL RATE
int 	q = 0; 		// PITCH RATE
int 	r = 0; 		// YAW RATE
long int ww[4] = {0, 0, 0, 0};

// Own written functions
#include "calibration_mode.h"
#include "manual_mode.h"
#include "panic_mode.h"
#include "safe_mode.h"
#include "p_control_mode.h"
#include "yaw_control_mode.h"
#include "full_control_mode.h"

// Time variables for integration
int t0 = 0;
int t1 = 0;
int dt = 0;
int integral[3] = {0,0,0};

// initialize arrays to which the circular buffers are going to point
#define DLOGSIZE	50000 
#define TXSIZE		32
#define RXSIZE		64

BYTE txelems[TXSIZE];
BYTE rxelems[RXSIZE];
BYTE   dl[DLOGSIZE];

// polling time to determine frequency for telemetry and datastoring
#define DATASTORETIMEMS	150
#define POLLTIMEMS	200

// variable to save buffer return in
BYTE 	c;

#define DEBUGGING


/******************************MACROS*****************************************/

/*------------------------------------------------------------------
 * Write an element, overwriting oldest element if buffer is full. 
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */ 
#define cbWrite(CB, VAL, sum){ 				\
	CB.elems[CB.end] = VAL; 				\
	CB.end = (CB.end + 1) % CB.size;			\
	if (CB.end == CB.start)				\
	{        					\
		CB.start = (CB.start + 1) % CB.size; 	\
	}						\
	*sum ^= VAL;					\
}

/*------------------------------------------------------------------
 * Write an element, overwriting oldest element if buffer is full. 
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */ 
#define cbWritenoSum(CB, VAL){ 				\
	CB.elems[CB.end] = VAL; 				\
	CB.end = (CB.end + 1) % CB.size;			\
	if (CB.end == CB.start)				\
	{        					\
		CB.start = (CB.start + 1) % CB.size; 	\
	}						\
}


/*------------------------------------------------------------------
 * get oldest element from buffer 
 * By Imara Speek 1506374
 *------------------------------------------------------------------  
 */
#define cbGet(CB, c) {						\
	*c = CB.elems[CB.start];				\
	CB.start = (CB.start + 1) % CB.size;			\
}


/*------------------------------------------------------------------
 * Fixed Point Multiplication
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */
#define mult(a, b) (((a) * (b)) >> 14)	

/*------------------------------------------------------------------
 * Check whether checksum is starting byte and if so change
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */
#define checkcheck(sum){ 			\
	if (*sum == (BYTE)STARTING_BYTE){	\
	 *sum = 0; 				\
	}					\
}



/*****************************************************************************/


/*------------------------------------------------------------------
 * 1st Order Butterworth filter
 * By Daniel Lemus
 *------------------------------------------------------------------
 */
// TODO rewrite to marco or position in between switch
void Butt2Filter(void)
{
	int i;
	for (i=0; i<2; i++) {
		y0[i] = (mult(A0,x0[i]-OFFSET_x0[i]) + mult(A1,x1[i]) + mult(B1,y1[i]));
 		x1[i] = x0[i]-OFFSET_x0[i];
		y1[i] = y0[i];
	}
}


/*------------------------------------------------------------------
 * Kalman Filter
 * By Diogo Monteiro (21-03-2014)
 *------------------------------------------------------------------
 */
void KalmanFilter(void)
{
 // TODO rewrite to marco or position in between switch       
    //Kalman for p, phi    
    sphi = y0[1];//TODO CONFIRM SENSOR SIGNALS 
    sp = x0[3] - OFFSET_x0[3];
    
    p = sp - p_b;
    phi = phi + mult(p,P2PHI);
    phi = phi - ((phi - sphi) >> C1);
    p_b = p_b + ((phi - sphi) >> C2);
 
    //Kalman for q, theta
    stheta = y0[0]; //TODO CONFIRM SENSOR SIGNALS    
    sq = x0[4] - OFFSET_x0[4];
    
    q = sq - q_b;
    theta = theta + mult(q,P2PHI);
    theta = theta - ((theta - stheta) >> C1);
    q_b = q_b + ((theta - stheta) >> C2);

}

/*------------------------------------------------------------------
 * Sensor Handling
 * By Diogo Monteiro (21-03-2014)
 *------------------------------------------------------------------
 */
void sensor_handling(void)
{
    
	//DISABLE_INTERRUPT(INTERRUPT_GLOBAL); 
    	// get sensor and timestamp values
	/*x0[0] = X32_QR_s0; x0[1] = X32_QR_s1; //x0[2] = X32_QR_s2; 
	x0[3] = X32_QR_s3; x0[4] = X32_QR_s4; x0[5] = X32_QR_s5;*/
	// get sensor and timestamp values
	x0[0] = 500; x0[1] = 501; //x0[2] = X32_QR_s2; 
	x0[3] = 503; x0[4] = 504; x0[5] = 505;
	
	//in case of erros, sensors must go to the main function

	//if(calibration_done)
    	//{
		// TODO remove this from the qr IR
		Butt2Filter();
		KalmanFilter();
	    	//Yaw Rate
	    	r = x0[5] - OFFSET_x0[5];
	//}
	//ENABLE_INTERRUPT(INTERRUPT_GLOBAL); 	

}

 
/*------------------------------------------------------------------
 * Circular buffer initialization 
 * Point start and end to the adress of the allocated vector of elements
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */
void cbInit(CBuffer *cb, int size, BYTE* array) {
	cb->size  = size + 1;
	cb->start = 0;
	cb->end   = 0;
	cb->elems = array;
}

/*------------------------------------------------------------------
 * Clean the buffer 
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */ 
void cbClean(CBuffer *cb) {
	memset(cb->elems, 0, sizeof(BYTE) * cb->size); 	
}

/*------------------------------------------------------------------
 * isr_qr_link -- QR link rx interrupt handler
 * By Daniel Lemus
 *------------------------------------------------------------------
 */
void isr_qr_link(void)
{
	int	ae_index;
	

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


	//functiontime = X32_us_clock - starttime;
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
		cbWritenoSum(rxcb, (BYTE)X32_rs232_data)
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
 * led on 
 * by Imara Speek 1506374
 *------------------------------------------------------------------
 */
void on_led(int i) 
{
	X32_leds = (1 << i);
}

/*------------------------------------------------------------------
 * led off
 * by Imara Speek 1506374
 *------------------------------------------------------------------
 */
void off_led(int i) 
{
	X32_leds = (0 << i);
}

/*------------------------------------------------------------------
 * Decoding function
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

	// Changing of the mode is taken care of in the pc part
	for (i = 0; i < nParams; i++)
	{
		 cbGet(rxcb, &package[i]);
	}
	
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

	// independent of the package structure
	for (i = 0; i < (nParams - 1); i++)
	{
		sum += package[i];
	}	
	sum = ~sum;
	
	checkcheck(&sum);
/*
	if (sum == 0x80)
	{
		sum = 0x00;
	}
*/
	
	if (package[CHECKSUM] != sum) {
		return 0;
	}
	else
		return 1;
}

/*------------------------------------------------------------------
 * Data Logging Storage
 * Store each parameter individually in arrays
 * by Imara Speek 1506374
 *------------------------------------------------------------------
 */
void store_data(void)
{
	BYTE sum;
	sum = 0;

	if (X32_ms_clock - storetime >= DATASTORETIMEMS)
	{
		//starttime = X32_ms_clock;

		cbWritenoSum(dscb, (BYTE)STARTING_BYTE);
		// the ms clock is actually 4 bytes, so takes least significant 2 bytes and log
		/*cbWrite(dscb, (BYTE)(X32_ms_clock >> 8), &sum);
		cbWrite(dscb, (BYTE)(X32_ms_clock), &sum);
		cbWrite(dscb, package[MODE], &sum);
		cbWrite(dscb, package[LIFT], &sum);
		cbWrite(dscb, package[ROLL], &sum);
		cbWrite(dscb, package[PITCH], &sum);
		cbWrite(dscb, package[YAW], &sum);
		cbWrite(dscb, (BYTE)(ae[0] >> 8), &sum);
		cbWrite(dscb, (BYTE)(ae[0]), &sum);

		cbWrite(dscb, (BYTE)(ae[1] >> 8), &sum);
		cbWrite(dscb, (BYTE)(ae[1]), &sum);
		cbWrite(dscb, (BYTE)(ae[2] >> 8), &sum);
		cbWrite(dscb, (BYTE)(ae[2]), &sum);
		cbWrite(dscb, (BYTE)(ae[3] >> 8), &sum);
		cbWrite(dscb, (BYTE)(ae[3]), &sum);
		cbWrite(dscb, (BYTE)(x0[0] >> 8), &sum);
		cbWrite(dscb, (BYTE)(x0[0]), &sum);
		cbWrite(dscb, (BYTE)(x0[1] >> 8), &sum);
		cbWrite(dscb, (BYTE)(x0[1]), &sum);

		cbWrite(dscb, (BYTE)(x0[2] >> 8), &sum);
		cbWrite(dscb, (BYTE)(x0[2]), &sum);
		cbWrite(dscb, (BYTE)(x0[3] >> 8), &sum);
		cbWrite(dscb, (BYTE)(x0[3]), &sum);
		cbWrite(dscb, (BYTE)(x0[4] >> 8), &sum);
		cbWrite(dscb, (BYTE)(x0[4]), &sum);
		cbWrite(dscb, (BYTE)(x0[5] >> 8), &sum);
		cbWrite(dscb, (BYTE)(x0[5]), &sum);
		cbWrite(dscb, (BYTE)(y0[0]), &sum);
		cbWrite(dscb, (BYTE)(y0[1]), &sum);

		cbWrite(dscb, (BYTE)(y0[2]), &sum);
		cbWrite(dscb, (BYTE)(y0[3]), &sum);
		cbWrite(dscb, (BYTE)(y0[4]), &sum);
		cbWrite(dscb, (BYTE)(y0[5]), &sum);
		cbWrite(dscb, (BYTE)(phi >> 8), &sum);
		cbWrite(dscb, (BYTE)(phi), &sum);
		cbWrite(dscb, (BYTE)(theta >> 8), &sum);
		cbWrite(dscb, (BYTE)(theta), &sum);
		cbWrite(dscb, (BYTE)(p), &sum);
		cbWrite(dscb, (BYTE)(q), &sum);

		cbWrite(dscb, (BYTE)(pcontrol >> 8), &sum);
		cbWrite(dscb, (BYTE)(pcontrol), &sum);
		cbWrite(dscb, (BYTE)(p1control >> 8), &sum);
		cbWrite(dscb, (BYTE)(p1control), &sum);
		cbWrite(dscb, (BYTE)(p2control >> 8), &sum);
		cbWrite(dscb, (BYTE)(p2control), &sum);
		cbWrite(dscb, (BYTE)(controltime), &sum);*/

        //FINAL DATALOG
        cbWrite(dscb, (BYTE)(X32_ms_clock - storetime), &sum);
		cbWrite(dscb, package[MODE], &sum);
		cbWrite(dscb, package[LIFT], &sum);
		cbWrite(dscb, package[ROLL], &sum);
		cbWrite(dscb, package[PITCH], &sum);
		cbWrite(dscb, package[YAW], &sum);
		cbWrite(dscb, (BYTE)(ae[0] >> 8), &sum);
		cbWrite(dscb, (BYTE)(ae[0]), &sum);
		cbWrite(dscb, (BYTE)(ae[1] >> 8), &sum);

		cbWrite(dscb, (BYTE)(ae[1]), &sum);
		cbWrite(dscb, (BYTE)(ae[2] >> 8), &sum);
		cbWrite(dscb, (BYTE)(ae[2]), &sum);
		cbWrite(dscb, (BYTE)(ae[3] >> 8), &sum);
		cbWrite(dscb, (BYTE)(ae[3]), &sum);
		cbWrite(dscb, (BYTE)(x0[0] - OFFSET_x0[0]), &sum);
		cbWrite(dscb, (BYTE)(x0[1] - OFFSET_x0[1]), &sum);
		// dont nee x02 
		cbWrite(dscb, (BYTE)(x0[3] - OFFSET_x0[3]), &sum);
		cbWrite(dscb, (BYTE)(x0[4] - OFFSET_x0[4]), &sum);
		cbWrite(dscb, (BYTE)(x0[5] - OFFSET_x0[5]), &sum);

		cbWrite(dscb, (BYTE)(y0[0]), &sum);
		cbWrite(dscb, (BYTE)(y0[1]), &sum);
		cbWrite(dscb, (BYTE)(phi >> 8), &sum);
		cbWrite(dscb, (BYTE)(phi), &sum);
		cbWrite(dscb, (BYTE)(theta >> 8), &sum);
		cbWrite(dscb, (BYTE)(theta), &sum);
		cbWrite(dscb, (BYTE)(p), &sum);
		cbWrite(dscb, (BYTE)(q), &sum);
		cbWrite(dscb, (BYTE)(pcontrol), &sum);
		cbWrite(dscb, (BYTE)(p1control), &sum);

		cbWrite(dscb, (BYTE)(p2control), &sum);
		cbWrite(dscb, (BYTE)(controltime), &sum);

		// check whether the checksum  is the same as the starting byte
/*
	   	if (sum == 0x80)
		{
			sum = 0;
	    	}
*/
		checkcheck(&sum);
		cbWritenoSum(dscb, (BYTE)(sum));
	
		storetime = X32_ms_clock;
		//functiontime = X32_ms_clock - starttime;
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

		while ( !X32_rs232_txready ) ;

		X32_rs232_data = dscb.elems[dscb.start];
		dscb.start = (dscb.start + 1) % dscb.size;	
		//on_led(6);
	}
}

/*------------------------------------------------------------------
 * Send the telemetry at 10 Hz max 
 * by Imara Speek 1506374
 *------------------------------------------------------------------
 */
void send_telemetry(void)
{
	BYTE sum;

	// Checks if more than or 100 ms have passed since the last 
	// telemetry
	if (X32_ms_clock - polltime >= POLLTIMEMS)
	{
		// initialize checksum
		sum = 0;

		// set a flag when the sum is biger than 0
		sumae = ae[0] + ae[1] + ae[2] + ae[3];
		(sumae > 0) ? (telemetry_flag |= 0x04) : (telemetry_flag &= 0x03); 

#ifdef DEBUGGING
		// profiling
		//starttime = X32_us_clock;

		cbWritenoSum(txcb, (BYTE)STARTING_BYTE);
		//cbWrite(txcb, (BYTE)(X32_ms_clock >> 8), &sum);
		cbWrite(txcb, package[MODE], &sum);
		cbWrite(txcb, (BYTE)(controltime >> 8), &sum);
		cbWrite(txcb, (BYTE)controltime, &sum);
		cbWrite(txcb, 0x01, &sum);
		cbWrite(txcb, 0x02, &sum);
		cbWrite(txcb, (BYTE)telemetry_flag, &sum);

		//functiontime = X32_us_clock - starttime;
		
// Code for the final lab
#else
		cbWritenoSum(txcb, (BYTE)STARTING_BYTE);
		//cbWrite(txcb, (BYTE)(package[MODE]), &sum);
		cbWrite(txcb, (BYTE)(r), &sum);
		cbWrite(txcb, (BYTE)(phi >> 8), &sum);
		cbWrite(txcb, (BYTE)(phi), &sum);
		cbWrite(txcb, (BYTE)(theta >> 8), &sum);
		cbWrite(txcb, (BYTE)(theta), &sum);
		cbWrite(txcb, (BYTE)telemetry_flag, &sum);

#endif
		// make sure the checksum isn't the starting byte 0x80
		/*
		if (sum == STARTING_BYTE)
		{
			sum = 0x00;
		}
		*/
		checkcheck(&sum);

		cbWritenoSum(txcb, (BYTE)sum);
		
		// Send the data untill the buffer is empty
		while (txcb.end != txcb.start)
		{
			// wait untill tx is ready to send
			while ( !X32_rs232_txready ) ;

			cbGet(txcb, &X32_rs232_data);	
		}
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


	/**********************************************************/
	/*              INTERRUPT INITIALIZING                    */

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

	// prepare rs232 rx interrupt and getchar handler
        SET_INTERRUPT_VECTOR(INTERRUPT_PRIMARY_RX, &isr_rs232_rx);
        SET_INTERRUPT_PRIORITY(INTERRUPT_PRIMARY_RX, 20);
	while (X32_rs232_char) c = X32_rs232_data; // empty buffer
        ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);

	// prepare rs232 tx interrupt and getchar handler
        SET_INTERRUPT_VECTOR(INTERRUPT_PRIMARY_TX, &isr_rs232_tx);
        SET_INTERRUPT_PRIORITY(INTERRUPT_PRIMARY_TX, 15);
        ENABLE_INTERRUPT(INTERRUPT_PRIMARY_TX);       

	/**********************************************************/
	

	// initialize some other stuff
	X32_leds = 0;
	program_done = 0;

	// initializing of the buffers
	cbInit(&txcb, (TXSIZE - 1), txelems);
	cbInit(&rxcb, (RXSIZE - 1), rxelems);
	cbInit(&dscb, (DLOGSIZE - 1), dl);

	// clean the buffers
	cbClean(&txcb);
	cbClean(&rxcb);
	cbClean(&dscb);

	// profiling variables
	controltime = 0;
	maxtime = 0;

	// Enable all interrupts, starting the system
        ENABLE_INTERRUPT(INTERRUPT_GLOBAL); 

	while (! program_done) 
	{		
		// reset the commflag to check communication
		if (commflag++ > commthres)
		{
			package[MODE] = PANIC_MODE;
		}	

		// See if there is a character in the buffer
		// and check whether that is the starting byte		
		cbGet(rxcb, &c);

		if (c == STARTING_BYTE)
		{
			decode();
			if (check_sum())
			{
				switch (package[MODE])
				{
					case SAFE_MODE:
						safe_mode();
						break;
					case PANIC_MODE:
						//on_led(1);
						panic_mode();	
						break;
					case MANUAL_MODE:
						//on_led(2);
						manual_mode();
						break;
					case CALIBRATION_MODE:
					       	if(calibration_counter < CALIBRATION_THRESHOLD) 
						{	
						
						    calibration_mode();
						}
						break;
					case YAW_CONTROL_MODE:
                        			sensor_handling();
                        			yaw_control_mode();		
						break;
					case FULL_CONTROL_MODE:
                        			sensor_handling();
						full_control_mode();
						break;
					case P_CONTROL_MODE:
               					p_control_mode();
						break;
					case ABORT_MODE:
						program_done++;
						for (i = 0; i < 4; i++)
						{
							ae[i]=0;
						}
						break;					
					default :
						break;
				}
				
				// TODO put these in main code where they are necessary
				//starttime = X32_us_clock;

		        	

				//functiontime = X32_us_clock - starttime;

				// Current time of the control loop
				controltime = X32_us_clock - controltime;
				
				
				// if the package was correct, store the correct data
				store_data();

				

				if ((controltime > maxtime) && controltime < 5000)
				{
					maxtime = controltime;
				}
				X32_display = maxtime;

				// sends the telemetry at 10Hz
				send_telemetry();

				// profiling the control time	
				controltime = X32_us_clock;
				
				// turn l the leds off
				// X32_leds = 0;		
			}
		}
	}
	// send the data log to the pc
	send_data();	
	
	DISABLE_INTERRUPT(INTERRUPT_GLOBAL);

	// turn on led 7 when program is finished
	on_led(7);

	return 0;
}


