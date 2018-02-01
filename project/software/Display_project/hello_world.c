#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "Colours.h"
#include "Fonts.h"
#include "ultrasonic_direct.h"

// graphics register addresses all begin with '8' to bypass data cache on NIOS
#define GraphicsCommandReg (*(volatile unsigned short int *)(0x84000000))
#define GraphicsStatusReg (*(volatile unsigned short int *)(0x84000000))
#define GraphicsX1Reg (*(volatile unsigned short int *)(0x84000002))
#define GraphicsY1Reg (*(volatile unsigned short int *)(0x84000004))
#define GraphicsX2Reg (*(volatile unsigned short int *)(0x84000006))
#define GraphicsY2Reg (*(volatile unsigned short int *)(0x84000008))
#define GraphicsColourReg (*(volatile unsigned short int *)(0x8400000E))
#define GraphicsBackGroundColourReg (*(volatile unsigned short int *)(0x84000010))
/************************************************************************************************
 ** This macro pauses until the graphics chip status register indicates that it is idle
 ***********************************************************************************************/
#define WAIT_FOR_GRAPHICS while((GraphicsStatusReg & 0x0001) != 0x0001);
// #defined constants representing values we write to the graphics 'command' register to get
// it to draw something. You will add more values as you add hardware to the graphics chip
// Note DrawHLine, DrawVLine and DrawLine at the moment do nothing - you will modify these
#define DrawHLine 1
#define DrawVLine 2
#define DrawLine 3
#define DrawFilledRect  4
#define PutAPixel 0xA
#define GetAPixel 0xB
#define ProgramPaletteColour 0x10

#define XRES 800
#define YRES 480


#define Touchscreen_Status 	(*(volatile unsigned char *)(0x84000230))
#define Touchscreen_Control 	(*(volatile unsigned char *)(0x84000230))
#define Touchscreen_TxData 		(*(volatile unsigned char *)(0x84000232))
#define Touchscreen_RxData 		(*(volatile unsigned char *)(0x84000232))
#define Touchscreen_Baud 		(*(volatile unsigned char *)(0x84000234))

#define RS232_Control 	(*(volatile unsigned char *)(0x84000200))
#define RS232_Status 	(*(volatile unsigned char *)(0x84000200))
#define RS232_TxData 	(*(volatile unsigned char *)(0x84000202))
#define RS232_RxData 	(*(volatile unsigned char *)(0x84000202))
#define RS232_Baud 	 	(*(volatile unsigned char *)(0x84000204))

typedef struct { int x, y; } Point ;
struct room {
		short lights;
		short door;
		short requested;
		short in_use;
		short occupied;
	};
	struct room roomArray[10];
/*void Init_RS232(void) {
	/**
	 * Set up 6850 Control register to utilise a dive by 16 clock.
	 * set RTS low, use 8 bits of data, no parity, 1 stop bit,
	 * transmitter interrupt disabled
	 * program baud rate generator to use 115k baud
	 *
	printf("Initializing control register\n");
	RS232_Control = 0b00000011;
	RS232_Control = 0b10010101;
	RS232_Baud 	  = 0b00000001; // program for 115k baud
}*/

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

int getbitRS232(void) {
	// poll Rx bit in 6850 status register. Wait for it to become '1'
	// read received characer from 6850 RxData resgister.
	int read_status_bit = 0;
	while(read_status_bit == 0) {
		read_status_bit = RS232_Status & 0b01;
	}
	int character = RS232_RxData & 0b00000001;
	return character;
}

int getcharRS232(void) {
	// poll Rx bit in 6850 status register. Wait for it to become '1'
	// read received characer from 6850 RxData resgister.
	int read_status_bit = 0;
	while(read_status_bit == 0) {
		read_status_bit = RS232_Status & 0b01;
	}
	int character = RS232_RxData & 0b00000001;
	return character;
}

int RS232TestForReceivedData(void) {
	return RS232_RxData & 0b1;
}


int GetRangeData (void){
	putcharRS232(0x44);
	int c;
	c = getbitRS232();
	printf("RangeData: %d\n",c);
	return c;
}

int TurnServo (void){
	putcharRS232(0x45);
	return 1;
}

void TurnOnLights(void){
	putcharRS232(0x46);
}

int GetButtonPress(void){
	putcharRS232(0x47);
	char c = getbitRS232();
	printf("Button State: %d\n",c);
	if(c=='a'){
		return 1;
	}
	else
	return 0;

}

void ResolvedRequest(void){
	putcharRS232(0x48);

}
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

	usleep(10000);
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

Point GetPen(void){
	Point p1;
	int packets[4];
	// wait for a pen down command then return the X,Y coord of the point
	// calibrated correctly so that it maps to a pixel on screen

	// Wait for first packet of touch

	if(GetRangeData()){
		roomArray[0].occupied=1;
	}
	else {
		roomArray[0].occupied = 0;
	}
	wait();
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

	printf("x = %d ", p1.x);
	printf("y = %d\n", p1.y);

	/*if(GetButtonPress()){
			roomArray[0].requested = 1;
		}
		else {
			roomArray[0].requested = 0;
		}*/

	return p1;
}


/*****************************************************************************
 * This function waits for a touch screen press event and returns X,Y coord
 *****************************************************************************/
Point GetPress(void)
{
	Point p1;
	printf("Getting Press\n");
	// wait for a pen down command then return the X,Y coord of the point
	// calibrated correctly so that it maps to a pixel on screen
	p1 = GetPen();
	return p1;
}
/*****************************************************************************
 * This function waits for a touch screen release event and returns X,Y coord
 *****************************************************************************/


void WriteAPixel(int x, int y, int Colour)
{
	WAIT_FOR_GRAPHICS; // is graphics ready for new command
	GraphicsX1Reg = x; // write coords to x1, y1
	GraphicsY1Reg = y;
	GraphicsColourReg = Colour; // set pixel colour
	GraphicsCommandReg = PutAPixel; // give graphics "write pixel" command
}
/*********************************************************************************************
 * This function read a single pixel from the x,y coords specified and returns its colour
 * Note returned colour is a byte and represents a palette number (0-255) not a 24 bit RGB value
 *********************************************************************************************/
int ReadAPixel(int x, int y)
{
	WAIT_FOR_GRAPHICS; // is graphics ready for new command
	GraphicsX1Reg = x; // write coords to x1, y1
	GraphicsY1Reg = y;
	GraphicsCommandReg = GetAPixel; // give graphics a "get pixel" command
	WAIT_FOR_GRAPHICS; // is graphics done reading pixel
	return (int)(GraphicsColourReg) ; // return the palette number (colour)
}
/**********************************************************************************
 ** subroutine to program a hardware (graphics chip) palette number with an RGB value
 ** e.g. ProgramPalette(RED, 0x00FF0000) ;
 **
 ************************************************************************************/
void ProgramPalette(int PaletteNumber, int RGB)
{
	WAIT_FOR_GRAPHICS;
	GraphicsColourReg = PaletteNumber;
	GraphicsX1Reg = RGB >> 16 ; // program red value in ls.8 bit of X1 reg
	GraphicsY1Reg = RGB ; // program green and blue into ls 16 bit of Y1 reg
	GraphicsCommandReg = ProgramPaletteColour; // issue command
}
/*********************************************************************************************
This function draw a horizontal line, 1 pixel at a time starting at the x,y coords specified
 *********************************************************************************************/
void DrawHorizontalLine(int x1, int x2, int y, int Colour)
{
	WAIT_FOR_GRAPHICS;

	GraphicsX1Reg = x1;
	GraphicsX2Reg = x2;
	GraphicsY1Reg = y;
	GraphicsColourReg = Colour;
	GraphicsCommandReg = DrawHLine;
}

/*********************************************************************************************
This function draw a vertical line, 1 pixel at a time starting at the x,y coords specified
 *********************************************************************************************/
void DrawVerticalLine(int y1, int y2, int x, int Colour)
{
	WAIT_FOR_GRAPHICS;

	GraphicsY1Reg = y1;
	GraphicsY2Reg = y2;
	GraphicsX1Reg = x;
	GraphicsColourReg = Colour;
	GraphicsCommandReg = DrawVLine;
}

int sign (int n1, int n2){
	if(n2-n1>0){
		return 1;
	}
	else if (n2==n1){
		return 0;
	}
	else {
		return -1;
	}
}

/*******************************************************************************
 ** Implementation of Bresenhams line drawing algorithm
 *******************************************************************************/
void DrawBresenhamLine(int x1, int x2, int y1, int y2, int Colour)
{
	WAIT_FOR_GRAPHICS;

	GraphicsX1Reg = x1;
	GraphicsX2Reg = x2;
	GraphicsY1Reg = y1;
	GraphicsY2Reg = y2;
	GraphicsColourReg = Colour;
	GraphicsCommandReg = DrawLine;
}

void DrawFillRect(int x1, int x2, int y1, int y2, int colour){
	int i;
	int j;
	for(j=y1;j<+y2;j++){
		DrawHorizontalLine(x1,x2,j,colour);
	}
}

void DrawRect(int x1, int x2, int y1, int y2, int borderColour){
	DrawHorizontalLine(x1,x2,y1,borderColour);
	DrawHorizontalLine(x1,x2,y2,borderColour);
	DrawVerticalLine(y1,y2,x1,borderColour);
	DrawVerticalLine(y1,y2,x2,borderColour);
}

void DrawFillRectBorder(int x1, int x2, int y1, int y2, int colour, int borderColour){
	DrawHorizontalLine(x1,x2,y1,borderColour);
	DrawHorizontalLine(x1,x2,y2,borderColour);
	DrawVerticalLine(y1,y2,x1,borderColour);
	DrawVerticalLine(y1,y2,x2,borderColour);
	int i;
	int j;
	for(i=x1+1;i<+x2;i++){
		for(j=y1+1;j<+y2;j++){
			WriteAPixel(i,j,colour);
		}
	}
}

/*************************************************************************************************
 ** This function draws a single ASCII character at the coord and colour specified
 ** it optionally ERASES the background colour pixels to the background colour
 ** This means you can use this to erase characters
 **
 ** e.g. writing a space character with Erase set to true will set all pixels in the
 ** character to the background colour
 **
 *************************************************************************************************/
void OutGraphicsCharFont1(int x, int y, int fontcolour, int backgroundcolour, int c, int Erase)
{
	// using register variables (as opposed to stack based ones) may make execution faster
	// depends on compiler and CPU
	register int row, column, theX = x, theY = y ;
	register int pixels ;
	register char theColour = fontcolour ;
	register int BitMask, theC = c ;
	// if x,y coord off edge of screen don't bother
	// XRES and YRES are #defined to be 800 and 480 respectively
	if(((short)(x) > (short)(XRES-1)) || ((short)(y) > (short)(YRES-1)))
		return ;
	// if printable character subtract hex 20
	if(((short)(theC) >= (short)(' ')) && ((short)(theC) <= (short)('~'))) {
		theC = theC - 0x20 ;
		for(row = 0; (char)(row) < (char)(7); row ++) {
			// get the bit pattern for row 0 of the character from the software font
			pixels = Font5x7[theC][row] ;
			BitMask = 16 ;
			for(column = 0; (char)(column) < (char)(5); column ++) {
				// if a pixel in the character display it
				if((pixels & BitMask))
					WriteAPixel(theX+column, theY+row, theColour) ;
				else {
					if(Erase == 1)
						// if pixel is part of background (not part of character)
						// erase the background to value of variable BackGroundColour
						WriteAPixel(theX+column, theY+row, backgroundcolour) ;
				}
				BitMask = BitMask >> 1 ;
			}
		}
	}
}

void OutGraphicsCharFont2(int x, int y, int fontcolour, int backgroundcolour, int c, int Erase)
{
	// using register variables (as opposed to stack based ones) may make execution faster
	// depends on compiler and CPU
	register int row, column, theX = x, theY = y ;
	register int pixels ;
	register char theColour = fontcolour ;
	register int BitMask, theC = c ;
	// if x,y coord off edge of screen don't bother
	// XRES and YRES are #defined to be 800 and 480 respectively
	if(((short)(x) > (short)(XRES-1)) || ((short)(y) > (short)(YRES-1)))
		return ;
	// if printable character subtract hex 20
	if(((short)(theC) >= (short)(' ')) && ((short)(theC) <= (short)('~'))) {
		theC = theC - 0x20 ;
		for(row = 0; (char)(row) < (char)(14); row ++) {
			// get the bit pattern for row 0 of the character from the software font
			pixels = Font10x14[theC][row] ;
			BitMask = 1 << 9 ;
			for(column = 0; (char)(column) < (char)(10); column ++) {
				// if a pixel in the character display it
				if((pixels & BitMask))
					WriteAPixel(theX+column, theY+row, theColour) ;
				else {
					if(Erase == 1)
						// if pixel is part of background (not part of character)
						// erase the background to value of variable BackGroundColour
						WriteAPixel(theX+column, theY+row, backgroundcolour) ;
				}
				BitMask = BitMask >> 1 ;
			}
		}
	}
}



void DrawString(int x,int y, int colour, int background, char* string, int font, int erase){
	int i;
	if(font==1){
		for(i=0; string[i] != '\0';i++){
			OutGraphicsCharFont1(x,y,colour,background,string[i],erase);
			x += 10;
		}
	}
	else if (font ==2){
		for(i=0; string[i] != '\0';i++){
			OutGraphicsCharFont2(x,y,colour,background,string[i],erase);
			x += 10;
		}
	}
}




void BaseDisplay (void){
	int i;
	int j;

	for(i=0; i<800; i++)
		DrawHorizontalLine(0,800,i,TEAL);
	char* roomString;
	for(j=1;j<=10;j++){
		sprintf(roomString,"Room %d",j);
		DrawRect(100,300,40*j,40*(j+1),BLACK);
		DrawString(105, 40*j+40/4,BLACK,TEAL,roomString,2,1);
	}
	printf("Base Displayed \n");
}


void InfoDisplay (int room_num, short lights, short door, short occupied, short in_use){
	int i;
	int j;

	//Cover Last Data
	for(i=0; i<480; i++)
		DrawHorizontalLine(0,800,i,TEAL);
	for(j=1;j<=10;j++){
		DrawRect(100,300,40*j,40*(j+1),BLACK);
	}
	//Change Colour of selected Room
	DrawFillRect(101,299,40*room_num+1,40*(room_num+1)-1,NAVY);

	char* roomDeetsString;
	sprintf(roomDeetsString,"Room %d Details & Options", room_num);
	DrawRect(350,750,40,40*11,BLACK);
	DrawString(360,40*1.25,BLACK,TEAL,roomDeetsString,2,1);
	DrawString(400,40*2.25-14,BLACK,TEAL,"In Use: ",2,1);
	if(in_use){
		DrawString(530,40*2.25-14,GREEN,TEAL,"YES",2,1);
	}
	else{
		DrawString(530,40*2.25-14,CRIMSON,TEAL,"NO",2,1);

	}
	DrawString(400,40*3.25-14,BLACK,TEAL,"Occupied: ",2,1);
	if(occupied){
		DrawString(530,40*3.25-14,GREEN,TEAL,"YES",2,1);
	}
	else{
		DrawString(530,40*3.25-14,CRIMSON,TEAL,"NO",2,1);
	}

	DrawString(400,40*4.25-14,BLACK,TEAL,"Lights: ",2,1);
	if(lights){
		DrawString(530,40*4.25-14,GREEN,TEAL,"ON",2,1);
	}
	else{
		DrawString(530,40*4.25-14,CRIMSON,TEAL,"OFF",2,1);
	}


	DrawString(400,40*5.25-14,BLACK,TEAL,"Door: ",2,1);
	if(door){
		DrawString(530,40*5.25-14,GREEN,TEAL,"UNLOCKED",2,1);
	}
	else{
		DrawString(530,40*5.25-14,CRIMSON,TEAL,"LOCKED",2,1);
	}



	DrawString(400,40*6.25-14,BLACK,TEAL,"Lights: ",2,1);
	DrawFillRect(480,560,40*6.25-34,40*7.25-24,CRIMSON);
	DrawString(490,40*6.25-14,BLACK,CRIMSON,"OFF",2,1);
	DrawFillRect(570,650,40*6.25-34,40*7.25-24,GREEN);
	DrawString(580,40*6.25-14,BLACK,GREEN,"ON",2,1);

	DrawString(400,40*7.75-14,BLACK,TEAL,"Door: ",2,1);
	DrawFillRect(480,560,40*7.75-34,40*8.75-24,CRIMSON);
	DrawString(490,40*7.75-14,BLACK,CRIMSON,"LOCK",2,1);
	DrawFillRect(570,650,40*7.75-34,40*8.75-24,GREEN);
	DrawString(580,40*7.75-14,BLACK,GREEN,"UNLOCK",2,1);


}

void RequestDisplay (){

	DrawFillRect(200,600,90,390,TEAL);
	DrawFillRect(250,375,190,290,RED);
	DrawFillRect(425,550,190,290,GREEN);
	return;

}
void RequestDotDisplay (int room_num){

	DrawFillRect(315,335,40*room_num+10,40*(room_num+1)-10,RED);
	return;

}

void RequestStringDisplay (int room_number){
	char* requestString;
	sprintf(requestString,"Access Request for room %d",room_number);
	DrawString(260,110,BLACK,WHITE,requestString,2,1);
	DrawString(290,230,BLACK,RED,"DENY",2,1);
	DrawString(455,230,BLACK,GREEN,"APPROVE",2,1);
	return;
}

int RequestChoice (void){
	Point p1;
	while(1){
		p1 = GetPress();
		if(p1.x>=250&&p1.x<=375&&p1.y>=190&&p1.y<=290){
			return 0;
		}
		else if(p1.x>=425&&p1.x<=550&&p1.y>=190&&p1.y<=290){
			return 1;
		}
	}
	return -1;
}

int BaseChoice(void){
	Point p1;
	while(1){
		printf("Base Choice\n");
		p1 = GetPress();
		if(p1.x >= 100 && p1.x <= 300 && p1.y>=40 && p1.y<=440){
			printf("Chose #%d\n", p1.y/40);
			return floor(p1.y/40);
		}
	}
	return -1;
}

int InfoChoice (void){
	Point p1;
	while(1){
		printf("Info Choice\n");
		p1 = GetPress();
		if(p1.x >= 100 && p1.x <= 300 && p1.y>=40 && p1.y<=440){
			printf("Room #%d\n",(p1.y)/40);
			return floor((p1.y-10)/40);
		}
		else if (p1.x>=480&&p1.x<=560&&p1.y<=40*7.25-24&&p1.y>=40*6.25-34){
			//Lights off Function
			printf("Turning Lights Off\n");
			return 11;
		}
		else if (p1.x>=570&&p1.x<=650&&p1.y<=40*7.25-24&&p1.y>=40*6.25-34){
			//Lights on function
			printf("Turning Lights On\n");
			return 12;
		}
		else if (p1.x>=480&&p1.x<=560&&p1.y<=40*8.75-24&&p1.y>=40*7.75-34){
			//Door lock Function
			printf("lock Door\n");
			return 13;
		}
		else if (p1.x>=570&&p1.x<=650&&p1.y<=40*8.75-24&&p1.y>=40*7.75-34){
			//Door Lock function
			printf("Turning Lights Off\n");
			return 14;
		}
	}
	return -1;
}

void PrintNumbers (int room_num){
	int j;
	char* roomString;
	for(j=1;j<=10;j++){
		sprintf(roomString,"Room %d",j);
		if(j==room_num){
			DrawString(105, 40*j+40/4,BLACK,NAVY,roomString,2,1);

		}
		else{
			DrawString(105, 40*j+40/4,BLACK,TEAL,roomString,2,1);
		}
	}
}

void RunDisplay (void){
	int last_room_num;
	int curr_room_num;


	int i;
	for(i=0;i<10;i++){
		roomArray[i].lights = 0;
		roomArray[i].door = 0;
		roomArray[i].requested = 0;
		roomArray[i].in_use = 0;
		roomArray[i].occupied = 0;
	}
	roomArray[3].requested = 1;
	roomArray[4].requested = 1;

	Init_Touch();
	Init_RS232();
	BaseDisplay();
	last_room_num = BaseChoice();
	while(1){
		if(last_room_num>=1&&last_room_num<=10){
			curr_room_num = last_room_num;
		}
		else if (last_room_num==-1){
			printf("Error, my dude\n");
		}
		else if (last_room_num==11){
			roomArray[curr_room_num-1].lights = 0;
			if(curr_room_num==1){
				TurnOnLights();
			}
		}
		else if (last_room_num==12){
			roomArray[curr_room_num-1].lights = 1;
			if(curr_room_num==1){
				TurnOnLights();
			}
		}
		else if (last_room_num==13){
			roomArray[curr_room_num-1].door = 0;
			if(curr_room_num==1){
				TurnServo();
			}
			if(roomArray[curr_room_num-1].requested){
				roomArray[curr_room_num-1].requested = 0;
				ResolvedRequest();
			}
		}
		else if (last_room_num==14){
			roomArray[curr_room_num-1].door = 1;
			if(curr_room_num==1){
					TurnServo();
				}
			if(roomArray[curr_room_num-1].requested){
				roomArray[curr_room_num-1].requested = 0;
				roomArray[curr_room_num-1].in_use = 1;
				ResolvedRequest();
			}
		}


		InfoDisplay(curr_room_num,roomArray[curr_room_num-1].lights,roomArray[curr_room_num-1].door,roomArray[curr_room_num-1].occupied, roomArray[curr_room_num-1].in_use);
		int k;
		for(k=0;k<10;k++){
			if(roomArray[k].requested==1){
				RequestDotDisplay(k+1);
			}
		}
		PrintNumbers(curr_room_num);
		last_room_num = InfoChoice();
	}
}



int main (void) {

	RunDisplay();

	printf("Finished");

	return 0;
}


