/*
 * This is the Bluetooth Configuration file. It provides the means
 * to setup the Bluetooth modules, Master and Slave and to connect
 * one to the other.
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "io.h"


#define RS232_Control 	(*(volatile unsigned char *)(0x84000200))
#define RS232_Status 	(*(volatile unsigned char *)(0x84000200))
#define RS232_TxData 	(*(volatile unsigned char *)(0x84000202))
#define RS232_RxData 	(*(volatile unsigned char *)(0x84000202))
#define RS232_Baud 	 	(*(volatile unsigned char *)(0x84000204))

#define MAX_RANDOM 100
#define TEST_ITERATIONS 10
#define ERROR -1

/**
 * Subroutine to initialize the RS232 Port by writing some data
 * to the internal registers
 * call this function at the start of the program before you attempt
 * to read or write to data via the RS232 port
 *
 * Refer to 6850 data sheet for details of registers
 */

int putcharBluetooth(int c) {
	// Poll tx bit in 6850 status register. Wait for it to become '1'
	// Write 'c' to the 6850 TxData register to output the character
	int read_status_bit = 0;
	while(read_status_bit == 0) {
		read_status_bit = RS232_Status & 0b10;
	}
	RS232_TxData = c;
	//printf("set char %c", c);
	return c;
}

char getcharBluetooth(void) {
	// poll Rx bit in 6850 status register. Wait for it to become '1'
	// read received characer from 6850 RxData resgister.
	int read_status_bit = 0;
	while(read_status_bit == 0) {
		read_status_bit = RS232_Status & 0b01;
	}
	char character = RS232_RxData;
	//printf("char %c", character);
	return character;
}

/**
 * Stalls a NIOS II f processor for roughly 2 seconds.
 */
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

/**
 * Sends a string of data through the GPIO trasmitter pin.
 */
void writeDataToTx(char * data){
	int i;
	printf("Sending %s\n", data);
	for(i=0; data[i]!=NULL; i++){
		putcharBluetooth(data[i]);
	}
}

/**
 * Reads a string of data through the GPIO receiver pin.
 * Note the maximum readible string is 364 characters.
 */
void readDataFromTx(){
	int bufferSize = 365;
	char buffer[bufferSize];
	int i;

	//Read no more than bufferSize-1 characters to allow room for
	//the string terminating character
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
		i++;
	}
	printf("\n");
}

void initBluetooth(){
	printf("Initializing bluetooth control register\n");
	RS232_Control = 0b00000011;
	RS232_Control = 0b10010101;
	RS232_Baud = 0b00000001; //Setup the 115k baud rate to match the Bluetooth module
}

void enterCommandMode(){
	writeDataToTx("$$$");
	getcharBluetooth(); //Pull garbage value
}

void reset(){
	writeDataToTx("SF,1\r\n");
}

void setAuthenticationMode(){
	writeDataToTx("SA,4\r\n");
}

void name(){
	writeDataToTx("SN,group14\r\n");
}

void setPassword(){
	writeDataToTx("SP,1414\r\n");
}

/**
 * Attempts to connect the wired Master Bluetooth module
 * to the Slave Bluetooth module
 */
void AttemptBluetoothConnection(){
	wait();wait();
	initBluetooth();

	wait();wait();
	enterCommandMode();

	wait();wait();
	writeDataToTx("C\r\n");
}

/**
 * Sends a command to the wired Bluetooth module to
 * exit command mode and return to data mode.
 */
void exitCommandMode(){
	writeDataToTx("---\r\n");
}

/**
 * Configures the Bluetooth module to be named "group14",
 * set with password "1414", and disable authentication to
 * allow a straight-forward connection between Bluetooth
 * modules 
 */
int configureBluetoothDongle(){

	wait();wait();
	initBluetooth();

	wait();wait();
	enterCommandMode();
	readDataFromTx();

	wait();wait();
	name();
	readDataFromTx();

	wait();wait();
	setPassword();
	readDataFromTx();

	wait();wait();
	setAuthenticationMode();
	readDataFromTx();

	wait();wait();
	exitCommandMode();
	readDataFromTx();

	return 1;
}

