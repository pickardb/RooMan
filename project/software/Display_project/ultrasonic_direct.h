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

//WARNING this code is for directly connecting the ultrasonic sensor to the De1-SoC board
//through the bluetooth serial. When we really do use the bluetooth module with the ultrasonic
//sensor, don't use this file as-is!!!

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "io.h"

#define HEX0_1 (*(volatile unsigned char *) (0x00002030))
#define HEX2_3 (*(volatile unsigned char *) (0x00002040))
#define HEX4_5 (*(volatile unsigned char *) (0x00002050))

#define RS232_Control 	(*(volatile unsigned char *)(0x84000200))
#define RS232_Status 	(*(volatile unsigned char *)(0x84000200))
#define RS232_TxData 	(*(volatile unsigned char *)(0x84000202))
#define RS232_RxData 	(*(volatile unsigned char *)(0x84000202))
#define RS232_Baud 	 	(*(volatile unsigned char *)(0x84000204))

#define Bluetooth_Control 	(*(volatile unsigned char *)(0x84000220))
#define Bluetooth_Status 	(*(volatile unsigned char *)(0x84000220))
#define Bluetooth_TxData (*(volatile unsigned char *)(0x84000222))
#define Bluetooth_RxData (*(volatile unsigned char *)(0x84000222))
#define Bluetooth_Baud (*(volatile unsigned char *)(0x84000224))

#define MAX_RANDOM 100
#define TEST_ITERATIONS 10
#define ERROR -1

int runUltrasonic()
{
	Init_RS232();
	wait();wait();

	int c = readDataFromTx();
	return c;
}

int putcharBluetooth(int c) {
	// Poll tx bit in 6850 status register. Wait for it to become '1'
	// Write 'c' to the 6850 TxData register to output the character
	int read_status_bit = 0;
	while(read_status_bit == 0) {
		read_status_bit = Bluetooth_Status & 0b10;
	}
	Bluetooth_TxData = c;
	//printf("set char %c", c);
	return c;
}

char getcharBluetooth(void) {
	// poll Rx bit in 6850 status register. Wait for it to become '1'
	// read received characer from 6850 RxData resgister.
	int read_status_bit = 0;
	while(read_status_bit == 0) {
		read_status_bit = Bluetooth_Status & 0b01;
	}
	char character = Bluetooth_RxData;
	//printf("char %c", character);
	return character;
}

void wait(){
	int i=0;
	while(i<1000){
		i++;

		int j=0;
		while(j<1000){
			j++;
		}
	}
}

void writeDataToTx(char * data){
	int i;
	printf("Sending %s\n", data);
	for(i=0; data[i]!=NULL; i++){
		// IORD_8DIRECT(Bluetooth_TxData, data[i]);
		// *Bluetooth_TxData = data[i];
		putcharBluetooth(data[i]);
	}
}

void readDataFromTx(){
	int bufferSize = 365;
	char buffer[bufferSize];
	int i;

	for(i=0; i<bufferSize-1; i++){
		buffer[i] = getcharBluetooth();
		if(buffer[i] == '\n'){
			break;
		}
	}
	buffer[i] = NULL;

	i=0;
	while(buffer[i]!=NULL){
		printf("%c", buffer[i]);
		//HEX0_1 = buffer[i];
		i++;
	}
	printf("\n");
}

void initUltrasonic(){
	printf("Initializing bluetooth control register\n");
	Bluetooth_Control = 0b00000011;
	Bluetooth_Control = 0b10010101;
	Bluetooth_Baud = 0b00000111;
}

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
	RS232_Baud 	  = 0b00000111; // program for 9600 baud
}
