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
#include "Wifi.h"

int main()
{
	printf("Initializing...\n");
	rs232_init();
	//wifi_init();

	int temp;

	rs232_send_char((int)'\n');
	usleep(500);
	rs232_get_char();

	rs232_send_char(55);
	usleep(500);
	temp = rs232_get_char();

	if( temp != 55){
		printf("error, should be 55, received %i \n",temp);
	}else{
		printf("match \n");
	}



	rs232_send_char(60);
	usleep(500);
	temp = rs232_get_char();

	if(temp != 60){
		printf("error, should be 60, received %i \n",60);
	}else{
		printf("match \n");
	}

	rs232_send_char(70);
	usleep(500);
	temp = rs232_get_char();

	if(temp != 70){
		printf("error, should be 70, received %i \n",temp);
	}else{
		printf("match \n");
	}

	rs232_send_char(80);
	usleep(500);
	temp = rs232_get_char();

	if(temp != 80){
		printf("error,should be 80, received %i \n",temp);
	}else{
		printf("match \n");
	}

	printf("Done\n");
	return 0;
}

