/*------------------------------------------------------------------
 *  qrtest.c -- test QR engines and sensors
 *
 *  reads ae[0-3] from stdin
 *  (q,w,e,r increment ae[0-3], a,s,d,f decrement)
 *
 *  prints ae[0-3],sax,say,saz,sp,sq,sr,delta_t on stdout
 *  where delta_t is the qr-isr exec time
 *
 *  Arjan J.C. van Gemund
 *  Embedded Software Lab
 *
 *  Adjusted by Imara Speek for decoding in the FPGA

    version 2.1

TODO determine priorities
TODO control values have to be send
TODO what is the first bit? Cannot occur in the others bytes
TODO ask for log that is saved during running
TODO also want telemetry set
TODO Need to determine a protocol for that
TODO check checksum before decoding

 *------------------------------------------------------------------
 */

#include <stdio.h>
#include <x32.h>
#include "assert.h"

#define BYTE unsigned char
#define WORD unsigned short

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

// mode, lift ,roll, pitch, yaw, P, P1, P2, checksum
#define nParams		0x09

// RX FIFO
#define FIFOSIZE 16

// TODO changed the fifo type to unsigned characters
// Still need to adjust the fifosize properly

unsigned char	fifo[FIFOSIZE]; 
int	iptr, optr;

// MODES LIST
#define SAFE_MODE			0x00
#define PANIC_MODE			0x01
#define MANUAL_MODE			0x02
#define CALIBRATION_MODE		0x03
#define YAW_CONTROL_MODE		0x04
#define FULL_CONTROL_MODE		0x05

// Parameter list numbering
#define MODE		0x00
#define LIFT		0x01
#define ROLL		0x02
#define PITCH		0x03
#define YAW		0x04
#define PCONTROL	0x05
#define P1CONTROL	0x06
#define P2CONTROL	0x07
#define CHECKSUM	0x08

// Globals
char	c;
int	demo_done;
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

// Package variables (no available byte variable, so using unsigned char typedef)
int 	ignore = 0, startflag = 0;
BYTE 	mode, roll, pitch, yaw, lift, pcontrol, p1control, p2control, checksum;

// TODO change this to enum


/*------------------------------------------------------------------
 * isr_qr_link -- QR link rx interrupt handler
 *------------------------------------------------------------------
 */
void isr_button(void)
{
	button = 1;
}

/*------------------------------------------------------------------
 * isr_qr_timer -- QR timer interrupt handler
 *------------------------------------------------------------------
 */
void isr_qr_timer(void)
{
//	decode();
//	print_comm();
}

/*------------------------------------------------------------------
 * isr_qr_link -- QR link rx interrupt handler
 * arrive at the bitrate of the sensors
 * at about 1270 Hz 
 *------------------------------------------------------------------
 */
void isr_qr_link(void)
{
	int	ae_index;
	/* record time
	 */
	isr_qr_time = X32_us_clock;
        inst = X32_instruction_counter;
	/* get sensor and timestamp values
	 */
	s0 = X32_QR_s0; s1 = X32_QR_s1; s2 = X32_QR_s2; 
	s3 = X32_QR_s3; s4 = X32_QR_s4; s5 = X32_QR_s5;
	timestamp = X32_QR_timestamp;

	/* monitor presence of interrupts 
	 */
	isr_qr_counter++;
	if (isr_qr_counter % 500 == 0) {
		toggle_led(2);
	}	

	/* Clip engine values to be positive and 10 bits.
	 */
	for (ae_index = 0; ae_index < 4; ae_index++) 
	{
		if (ae[ae_index] < 0) 
			ae[ae_index] = 0;
		
		ae[ae_index] &= 0x3ff;
	}

// TODO based on readings set appropriate values to each rotor 

	/* Send actuator values
	 * (Need to supply a continous stream, otherwise
	 * QR will go to safe mode, so just send every ms)
	 */
	X32_QR_a0 = ae[0];
	X32_QR_a1 = ae[1];
	X32_QR_a2 = ae[2];
	X32_QR_a3 = ae[3];

	/* record isr execution time (ignore overflow)
	 */
        inst = X32_instruction_counter - inst;
	isr_qr_time = X32_us_clock - isr_qr_time;
}


/*------------------------------------------------------------------
 * isr_rs232_rx -- rs232 rx interrupt handler
 * on every received character (8n1 bit)
 *------------------------------------------------------------------
 */

void isr_rs232_rx(void)
{
	//Check Data
	int i;

	/* signal interrupt
	 */
	toggle_led(3);

	/* may have received > 1 char before IRQ is serviced so loop
	 * search for the startbytes and then adjust pointer
	 */

	while (X32_rs232_char) {
		fifo[iptr++] = X32_rs232_data;
		if (iptr > FIFOSIZE)
			iptr = 0;
	}

/*	TODO Read the Characters whenever a character arrived, timing can be strange
	if we read it in a loop in this interrup
	iptr = 0;
	startflag = 0;
	while (X32_rs232_char && (startflag == 0)) {
		printf("\nwhile loop");
		fifo[iptr] = X32_rs232_data;

		if ((fifo[iptr] == 0x00) && (fifo[iptr - 1] == 0x80))
		{
			printf("\nstarting header found!");
			startflag = 1;
			/* read the entire package and place in the fifo
	 		*/
/*
			for (iptr = 0 ; iptr < nParams; iptr++)
			{
				fifo[iptr] = X32_rs232_data;
				printf("\nparameter %d = %x", iptr, fifo[iptr]);
			}
			iptr = 0;
		}
		iptr++;
		if (iptr > FIFOSIZE)
			iptr = 0;
	}

	/* Clean the fifo
	 */
/*
	for (iptr = 0 ; iptr < FIFOSIZE; iptr++)
	{
		fifo[iptr] = 0x00;
	}
*/

	
	/* read the entire package and place in the fifo
	 */
/*
	for (iptr = 0 ; iptr < nParams; iptr++)
	{
		fifo[iptr] = X32_rs232_data;
		printf("\nparameter %d = %x", iptr, fifo[iptr]);
	}
	iptr = 0;
*/

	/* Checksum before decoding the package
	 */
/*
	checksum = fifo[CHECKSUM];
	sum = 0;
	// DEBUG DEBUG DEBUG DEBUG
	printf("\nChecksum = %x", checksum);

	for (i = 0; i < CHECKSUM; i++) {
		sum += fifo[i];
	}
	if (checksum != (BYTE)~sum) {
		printf("\nInvalid Pkg");
		return;
	}
*/
	/* If characters are received, switch on base of the first parameter, 
	   which is the mode and is defined at the top
	 */
/*
	switch (fifo[MODE])
	{
		case SAFE_MODE:
			mode = SAFE_MODE;
			printf("debugging, safe mode received");
			// no additional commands
			// ignore everything except for manual mode or exit
			break;
		case PANIC_MODE:
			mode = PANIC_MODE;
			printf("debugging, panic mode received");
			// no additional commands
			// ignore commands PC link and enter safe mode after few s
			break;
		case MANUAL_MODE:
			mode = MANUAL_MODE;
			printf("debugging, manual mode received");
			// read other commands in the fifo, so don't ignore
			break;
		case CALIBRATION_MODE:
			mode = CALIBRATION_MODE;
			printf("debugging, calibration mode received");
			// no additional commands given
			// reading sensors values
			break;
		case YAW_CONTROL_MODE:
			mode = YAW_CONTROL_MODE;
			printf("debugging, yaw mode received");
			// read other commands in the fifo, so don't ignore
			break;
		case FULL_CONTROL_MODE:
			mode = FULL_CONTROL_MODE;
			printf("debugging, full control mode received");
			// read other commands in the fifo, so don't ignore
			break;	
		default:
			mode = SAFE_MODE;
			printf("debugging, no mode received, switching to safe mode");
			// no additional commands
			// ignore everything except for manual mode or exit			
			break;
		// TODO might have to change this default statement
	}

	decode();
	print_comm();
*/
}

/*------------------------------------------------------------------
 * getchar -- read char from rx fifo, return -1 if no char available
 *------------------------------------------------------------------
 */
int 	getchar(void)
{
	int	c;

	if (optr == iptr)
		return -1;
	c = fifo[optr++];
	if (optr > FIFOSIZE)
		optr = 0;
	return c;
}


/*------------------------------------------------------------------
 * isr_wireless_rx -- wireless rx interrupt handler
 *------------------------------------------------------------------
 */
void isr_wireless_rx(void)
{
	int c;

	/* signal interrupt
	 */
	toggle_led(4);


	/* may have received > 1 char before IRQ is serviced so loop
	 */
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
 * process_key -- process command keys
 *------------------------------------------------------------------
 */
void process_key(char c) 
{
	switch (c) {
		case 'q':
			ae[0] += 10;
			break;
		case 'a':
			ae[0] -= 10;
			if (ae[0] < 0) ae[0] = 0;
			break;
		case 'w':
			ae[1] += 10;
			break;
		case 's':
			ae[1] -= 10;
			if (ae[1] < 0) ae[1] = 0;
			break;
		case 'e':
			ae[2] += 10;
			break;
		case 'd':
			ae[2] -= 10;
			if (ae[2] < 0) ae[2] = 0;
			break;
		case 'r':
			ae[3] += 10;
			break;
		case 'f':
			ae[3] -= 10;
			if (ae[3] < 0) ae[3] = 0;
			break;
		default:
			demo_done = 1;
	}
}

/*------------------------------------------------------------------
 * print_state -- print all sensors and actuators
 *------------------------------------------------------------------
 */

void print_state(void) 
{
	int i;
	char text[100] , a;
	printf("%3d %3d %3d %3d | ",ae[0],ae[1],ae[2],ae[3]);
	printf("%3d %3d %3d %3d %3d %3d (%3d, %d)\r\n",
		s0,s1,s2,s3,s4,s5,isr_qr_time, inst);
        
	sprintf(text, "%d %d %d %d \r\n",ae[0],ae[1],ae[2],ae[3]);
    	i = 0;
    	while( text[i] != 0) {
       		delay_ms(1);
		// if (X32_switches == 0x03)
		if (X32_wireless_stat & 0x01 == 0x01)
			X32_wireless_data = text[i];

		i++;
    	}
}

/*- -----------------------------------------------------------------
 * the execution level is preserved on function calls
 * print the execution level FOR DEBUGGING 
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */

void print_execution_level(void) {
	printf("The execution level is now: %d\r\n",
	get_execution_level());
}

/*------------------------------------------------------------------
 * Decoding function with a higher execution level
 * Function called in the timer ISR 
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */

void decode(void)
{	
	/* Decoding is a critical section, we therefore raise the execution level of the code
	 */

	// DEBUG DEBUG DEBUG DEBUG DEBUG
//	print_execution_level();

	/* raise the current code priority to 10, and
	   consequently, disable all interrupts with a
	   priority of 10 and lower */
	set_execution_level(10);
//	print_execution_level();

	/* Get the next character in the buffer after 
	 * the starting byte
	 */
	mode = getchar();
	lift = getchar();
	roll = getchar();
	pitch = getchar();
	yaw = getchar();
	pcontrol = getchar();
	p1control = getchar();
	p2control = getchar();
	checksum = getchar();

//	printf("\n we are decoding");

	/* restore to original level */
	restore_execution_level();

	// DEBUG DEBUG DEBUG DEBUG
//	print_execution_level();

}

/*------------------------------------------------------------------
 * Print commands
 * for DEBUGGING
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */

void print_comm(void)
{		
	printf("[%x][%x][%x][%x][%x][%x][%x][%x][%x]", mode, lift, roll, pitch, yaw, pcontrol, p1control, p2control, checksum);
}

/*------------------------------------------------------------------
 * Check for the starting byte
 * 
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */
void check_start(void)
{
	if (c == 0x80)
		startflag = 1;
	else 
		startflag = 0;
	printf("\nc = %x", c);
}

/*------------------------------------------------------------------
 * Check the checksum and return error message is package is corrupted
 * By Imara Speek 1506374
 *------------------------------------------------------------------
 */
int check_sum(void)
{
	int sum;
	/* Checksum before decoding the package
	 */
	sum = 0;
	// DEBUG DEBUG DEBUG DEBUG
	printf("\nChecksum = %x", checksum);
	
	sum = mode + lift + roll + pitch + yaw + pcontrol + p1control + p2control;
	
/*
	for (i = 0; i < CHECKSUM; i++) {
		sum += fifo[i];
	}
*/
	if (checksum != (BYTE)~sum) {
		printf("\nInvalid Pkg");
		return 0;
	}
	else
		return 1;
}

/*------------------------------------------------------------------
 * main -- do the test
 *------------------------------------------------------------------
 */
int main() 
{
	/* prepare QR rx interrupt handler
	 */
        SET_INTERRUPT_VECTOR(INTERRUPT_XUFO, &isr_qr_link);
        SET_INTERRUPT_PRIORITY(INTERRUPT_XUFO, 21);
	isr_qr_counter = isr_qr_time = 0;
	ae[0] = ae[1] = ae[2] = ae[3] = 0;
        ENABLE_INTERRUPT(INTERRUPT_XUFO);
 
// TODO check how this interrupt works, does the 200 determine how often?
	/* prepare timer interrupt for decoding the commands
	 */
        X32_timer_per = 200 * CLOCKS_PER_MS;
        SET_INTERRUPT_VECTOR(INTERRUPT_TIMER1, &isr_qr_timer);
        SET_INTERRUPT_PRIORITY(INTERRUPT_TIMER1, 21);
        ENABLE_INTERRUPT(INTERRUPT_TIMER1);

	/* prepare button interrupt handler
	 */
        SET_INTERRUPT_VECTOR(INTERRUPT_BUTTONS, &isr_button);
        SET_INTERRUPT_PRIORITY(INTERRUPT_BUTTONS, 8);
	button = 0;
        ENABLE_INTERRUPT(INTERRUPT_BUTTONS);	

	/* prepare rs232 rx interrupt and getchar handler
	 * TODO changed this priority to 22
	 */
        SET_INTERRUPT_VECTOR(INTERRUPT_PRIMARY_RX, &isr_rs232_rx);
        SET_INTERRUPT_PRIORITY(INTERRUPT_PRIMARY_RX, 22);
	while (X32_rs232_char) c = X32_rs232_data; // empty buffer
        ENABLE_INTERRUPT(INTERRUPT_PRIMARY_RX);

        /* prepare wireless rx interrupt and getchar handler
	 */
        SET_INTERRUPT_VECTOR(INTERRUPT_WIRELESS_RX, &isr_wireless_rx);
        SET_INTERRUPT_PRIORITY(INTERRUPT_WIRELESS_RX, 19);
        while (X32_wireless_char) c = X32_wireless_data; // empty buffer
        ENABLE_INTERRUPT(INTERRUPT_WIRELESS_RX);

	/* initialize some other stuff
	 */
        iptr = optr = 0;
	X32_leds = 0;
	demo_done = 0;

	/* start the test loop
	 */
        ENABLE_INTERRUPT(INTERRUPT_GLOBAL); 

	printf("Mode, Lift, Roll, Pitch, Yaw, Checksum:");

	while (! demo_done) {
		/* Check whether the starting byte is in the fifo
		 * The function will return a 0 or 1
		 */
		c = getchar();
		check_start();
		if (startflag)
		{		
		printf("\nStarting byte");

			decode();
			if(check_sum())
				print_comm(); 
			
		}
/*
		c = getchar();
		if (c != -1) {
			
			// process_key(c);
			
		}
		//print_state();
*/
// TODO check what this 0xFC and 0x03 is
                X32_leds = (X32_leds & 0xFC) | (X32_switches & 0x03 );
		if (button == 1){
			printf("You have pushed the button!!!\r\n");
			button = 0;
		}
		delay_ms(20);
	}

	printf("Exit\r\n");

        DISABLE_INTERRUPT(INTERRUPT_GLOBAL);

	return 0;
}

