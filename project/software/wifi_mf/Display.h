#include "Room.h"

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



