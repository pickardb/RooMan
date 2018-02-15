/**************************************************************************
/* Subroutine to initialise the RS232 Port by writing some data
** to the internal registers.
** Call this function at the start of the program before you attempt
** to read or write to data via the RS232 port
**
** Refer to 6850 data sheet for details of registers and
***************************************************************************/
#include <stdio.h>
#include "io.h"

#define LEDs (*(volatile unsigned char *)(0x00002010))
#define RS232_Control (*(volatile unsigned char *)(0x84000200))
#define RS232_Status (*(volatile unsigned char *)(0x84000200))
#define RS232_TxData (*(volatile unsigned char *)(0x84000202))
#define RS232_RxData (*(volatile unsigned char *)(0x84000202))
#define RS232_Baud (*(volatile unsigned char *)(0x84000204))

void Init_RS232(void)
{
 // set up 6850 Control Register to utilise a divide by 16 clock,
 // set RTS low, use 8 bits of data, no parity, 1 stop bit,
 // transmitter interrupt disabled
 // program baud rate generator to use 115k baud
	IOWR_8DIRECT(0x8400,0x0200,0x0003);
	IOWR_8DIRECT(0x8400,0x0204,0x0001);	//115k baud
	IOWR_8DIRECT(0x8400,0x0200,0x0055);
	printf("Finishing Init \n");

	//RS232 Control 0x00001000_000000_1_1_000000_0_0
}
int putcharRS232(int c)
{
 // poll Tx bit in 6850 status register. Wait for it to become '1'
 // write 'c' to the 6850 TxData register to output the character
	while (!(IORD_8DIRECT(0x8400,0x0200)&0x0002)){printf(" putchar\n");}
	IOWR_8DIRECT(0x8400,0x0202,c);

 return c ; // return c
}
int getcharRS232( void )
{
 // poll Rx bit in 6850 status register. Wait for it to become '1'
 // read received character from 6850 RxData register.
	while (!(IORD_8DIRECT(0x8400,0x0200)&0x0001)){printf(" getchar \n");}
	return IORD_8DIRECT(0x8400,0x0202);
}
// the following function polls the 6850 to determine if any character
// has been received. It doesn't wait for one, or read it, it simply tests
// to see if one is available to read
int RS232TestForReceivedData(void)
{
 // Test Rx bit in 6850 serial comms chip status register
 // if RX bit is set, return TRUE, otherwise return FALSE
	return (IORD_8DIRECT(0x8400,0x0200)&0x01);
}


int main (void){
	printf("testing LEDs\n");
	LEDs = 0x0055;
	printf("Initializing\n");
	Init_RS232();
	putcharRS232((int)"C");
	printf("Sent 'C'\n");
	int received = getcharRS232();
	printf("Received : %d \n", received);
	putcharRS232((int)"A");
	printf("Sent 'A'\n");
	received = getcharRS232();
	printf("Received : %d \n", received);
	putcharRS232((int)"T");
	printf("Sent 'T'\n");
	received = getcharRS232();
	printf("Received : %d \n", received);
	return 0;
}
