
#include <stdio.h>
#include <stdlib.h>

#define Touch_Status 	(*(volatile unsigned char *)(0x84000230))
#define Touch_Control 	(*(volatile unsigned char *)(0x84000230))
#define Touch_Tx 		(*(volatile unsigned char *)(0x84000232))
#define Touch_Rx 		(*(volatile unsigned char *)(0x84000232))
#define Touch_Baud 		(*(volatile unsigned char *)(0x84000234))

//Touch Enable 				0x55 0x01 0x12
//Touch Enable Response 	0x55 0x02 STATUS 0x12
//Touch Disable 			0x55 0x01 0x13
//Touch Disable Response 	0x55 0x02 STATUS 0x13

typedef struct { int x, y; } Point ;




/*****************************************************************************
 ** Initialise touch screen controller
 *****************************************************************************/
void Init_Touch(void)
{
	// Program 6850 and baud rate generator to communicate with touchscreen
	// send touchscreen controller an "enable touch" command
	printf("Initializing Touch Screen\n");
	Touch_Control = 0x03;
	Touch_Control = 0x15;
	Touch_Baud = 0b00000101; //Program for 9600 baud
	int i=0;
		while(i<1000){
			i++;
		}
	putCharTouch(0x55);
	putCharTouch(0x01);
	putCharTouch(0x12);
	i=0;
		while(i<1000){
			i++;
		}

}
/*****************************************************************************
 ** test if screen touched
 *****************************************************************************/
int ScreenTouched( void )
{
	// return TRUE if any data received from 6850 connected to touchscreen
	// or FALSE otherwise
	return (Touch_Rx == 0x80);

}
/*****************************************************************************
 ** wait for screen to be touched
 *****************************************************************************/
void WaitForTouch()
{
	while(!ScreenTouched()){}
}
/* a data type to hold a point/coord */
/*****************************************************************************
 * This function waits for a touch screen press event and returns X,Y coord
 *****************************************************************************/


int ScreenReleased( void )
{
	// return TRUE if any data received from 6850 connected to touchscreen
	// or FALSE otherwise
	return (Touch_Rx == 0x81);

}
/*****************************************************************************
 ** wait for screen to be released
 *****************************************************************************/
void WaitForRelease()
{
	while(!ScreenReleased()){}
}
/* a data type to hold a point/coord */
/*****************************************************************************
 * This function waits for a touch screen press event and returns X,Y coord
 *****************************************************************************/

Point GetPress(void)
{
	Point p1;
	int packets[4];
	// wait for a pen down command then return the X,Y coord of the point
	// calibrated correctly so that it maps to a pixel on screen

	// Wait for first packet of touch
	printf("Waiting for Touch \n");
	WaitForTouch();

	int i;
	for(i = 0; i < 4; i++){
		packets[i] = getCharTouch();
	}

	// Get x11 : x7 from 2nd packet, and concatenate to x6 : x0 from 1st packet
	p1.x = (packets[1] << 7) | packets[0];
	p1.y = (packets[3] << 7) | packets[2];

	// Map from controller resolution to screen pixel
	p1.x = p1.x * 799 / 4095;
	p1.y = p1.y  * 479 / 4095;

	printf("Press x = %d ", p1.x);
	printf("Press y = %d\n", p1.y);

	return p1;
}
/*****************************************************************************
 * This function waits for a touch screen release event and returns X,Y coord
 *****************************************************************************/
Point GetRelease(void)
{
	Point p1;
	int packets[4];
	// wait for a pen down command then return the X,Y coord of the point
	// calibrated correctly so that it maps to a pixel on screen

	// Wait for first packet of touch
	printf("Waiting for release\n");
	WaitForRelease();

	int i;
	for(i = 0; i < 4; i++){
		packets[i] = getCharTouch();
	}

	// Get x11 : x7 from 2nd packet, and concatenate to x6 : x0 from 1st packet
	p1.x = (packets[1] << 7) | packets[0];
	p1.y = (packets[3] << 7) | packets[2];

	// Map from controller resolution to screen pixel
	p1.x = p1.x * 799 / 4095;
	p1.y = p1.y  * 479 / 4095;

	printf("Release x = %d ", p1.x);
	printf("Release y = %d\n", p1.y);

	return p1;
}
int putCharTouch(int c) {
	// Poll tx bit in 6850 status register. Wait for it to become '1'
	// Write 'c' to the 6850 TxData register to output the character
	int read_status_bit = 0;
	while(read_status_bit == 0) {
		read_status_bit = Touch_Status & 0b10;
	}
	Touch_Tx = c;
	return c;
}

int getCharTouch(void) {
	// poll Rx bit in 6850 status register. Wait for it to become '1'
	// read received characer from 6850 RxData resgister.
	int read_status_bit = 0;
	while(read_status_bit == 0) {
		read_status_bit = Touch_Status & 0b01;
	}
	int character = Touch_Rx;
	return character;
}

int main (void){
	Init_Touch();
	while(1){
		GetPress();
		GetRelease();
	}
	return 0;

}
