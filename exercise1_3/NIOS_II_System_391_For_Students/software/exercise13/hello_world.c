/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>


#define RS232_Control 	(*(volatile unsigned char *)(0x84000200))
#define RS232_Status 	(*(volatile unsigned char *)(0x84000200))
#define RS232_TxData 	(*(volatile unsigned char *)(0x84000202))
#define RS232_RxData 	(*(volatile unsigned char *)(0x84000202))
#define RS232_Baud 	 	(*(volatile unsigned char *)(0x84000204))

#define MAX_RANDOM 100
#define TEST_ITERATIONS 10
#define ERROR -1

int main()
{
	printf("Starting test\n");
	int i = 0;
	while(i < TEST_ITERATIONS) {
		if (test() == ERROR) {
			printf("TEST FAILED \n");
			return 1;
		}
		i ++;
	}
	printf("ALL TESTS PASSED\n");
	return 0;
}


/**
 * Subroutine to initialize the RS232 Port by writing some data
 * to the internal registers
 * call this function at the start of the program before you attempt
 * to read or write to data via the RS232 port
 *
 * Refer to 6850 data sheet for details of registers
 */

void Init_RS232(void) {
	/**
	 * Set up 6850 Control register to utilise a dive by 16 clock.
	 * set RTS low, use 8 bits of data, no parity, 1 stop bit,
	 * transmitter interrupt disabled
	 * program baud rate generator to use 115k baud
	 */
	printf("Initializing control register\n");
	RS232_Control = 0b00000011;
	RS232_Control = 0b10010101;
	RS232_Baud 	  = 0b00000001; // program for 115k baud
}

int putcharRS232(int c) {
	// Poll tx bit in 6850 status register. Wait for it to become '1'
	// Write 'c' to the 6850 TxData register to output the character
	int read_status_bit = 0;
	while(read_status_bit == 0) {
		read_status_bit = RS232_Status & 0b10;
	}
	RS232_TxData = c;
	return c;
}

int getcharRS232(void) {
	// poll Rx bit in 6850 status register. Wait for it to become '1'
	// read received characer from 6850 RxData resgister.
	int read_status_bit = 0;
	while(read_status_bit == 0) {
		read_status_bit = RS232_Status & 0b01;
	}
	int character = RS232_RxData;
	return character;
}

/**
 * The following function polls the 6850 to determine if any character
 * has been received. It doesn't wait for one, or read it, it simple tests
 * to see if one is available to read
 */
int RS232TestForReceivedData(void) {
	return RS232_RxData & 0b1;
}


int test() {
	Init_RS232();
	int data = rand() % MAX_RANDOM;
	printf("Testing with %i\n", data);
	putcharRS232(data);
	int i=0;
	while(i<1000){
		i++;
	}
	int retrievedData = getcharRS232();			// Garbage number first
	//retrievedData = getcharRS232();
	if (data == retrievedData) {
		printf("Test passed \n");
		return 0;
	} else {
		printf("Something went wrong, expected: %i actual: %i \n", data, retrievedData);
		return -1;
	}
}

