#include "Point.h"

#define Touchscreen_Status 	(*(volatile unsigned char *)(0x84000230))
#define Touchscreen_Control 	(*(volatile unsigned char *)(0x84000230))
#define Touchscreen_TxData 		(*(volatile unsigned char *)(0x84000232))
#define Touchscreen_RxData 		(*(volatile unsigned char *)(0x84000232))
#define Touchscreen_Baud 		(*(volatile unsigned char *)(0x84000234))


/*******************************************************************************************
 * This function writes a single pixel to the x,y coords specified using the specified colour
 * Note colour is a byte and represents a palette number (0-255) not a 24 bit RGB value
 ********************************************************************************************/
int putCharTouch(char c){
	// Wait for TX bit in status register to turn 1 (means transmit is empty)
	while((Touchscreen_Status & 0x02) != 0x02);
	// Send the data to TX (reduce to 8 bits)
	Touchscreen_TxData = c;
	return c & 0xFF;
}

int getCharTouch(void)
{
	// poll Rx bit in 6850 status register. Wait for it to become '1'
	// read received character from 6850 RxData register.
	while ((Touchscreen_Status & 0x01) != 0x01);
	return Touchscreen_RxData;
}

/*****************************************************************************
 ** Initialise touch screen controller
 *****************************************************************************/
void Init_Touch(void)
{
	// Program 6850 and baud rate generator to communicate with touchscreen
	// send touchscreen controller an "enable touch" command

	// Divide by 16 clock, RTS Low, 8 bits of data, no parity,
	// 1 stop bit, transmitter interrupt disabled
	Touchscreen_Control = 0x15;
	// 9600 BAUD
	Touchscreen_Baud = 0x05;

	// slight delay to process
	usleep(10000);

	// Send TOUCH_ENABLE command
	putCharTouch(0x55);
	putCharTouch(0x01);
	putCharTouch(0x12);

}

/*****************************************************************************
 ** test if screen touched
 *****************************************************************************/
int ScreenTouched( void )
{
	// return TRUE if any data received from 6850 connected to touchscreen
	// or FALSE otherwise

	return (Touchscreen_RxData == 0x80);
}

/*****************************************************************************
 ** wait for screen to be touched
 *****************************************************************************/
void WaitForTouch()
{
	while(!ScreenTouched()){
	}
}


/*****************************************************************************
 * This function waits for a touch screen press event and returns X,Y coord
 *****************************************************************************/

Point GetBasePress(void){
	Point p1;
	int packets[4];
	char command;

	//Wait for the touchscreen to return 0x80
	WaitForTouch();

	//Get the 4 ints for the position
	int i;
	for(i = 0; i < 4; i++){
		packets[i] = getCharTouch();
	}

	//combine the two packets for each int into the actual position value
	p1.x = (packets[1] << 7) | packets[0];
	p1.y = (packets[3] << 7) | packets[2];

	// Map from resolution to pixel
	p1.x = p1.x * 799 / 4095;
	p1.y = p1.y  * 479 / 4095;

	printf("x = %d ", p1.x);
	printf("y = %d\n", p1.y);


	return p1;
}



