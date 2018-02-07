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
#include "altera_up_avalon_character_lcd.h"
#include <string.h>
#include <time.h>

//#define LCD (*(volatile unsigned char *)(0x000020c0))
void delay(int number_of_seconds)
{
    int milli_seconds = 1000 * number_of_seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds)
    	;
}

int main()
{
	alt_up_character_lcd_dev *lcd;

	lcd = alt_up_character_lcd_open_dev("/dev/character_lcd_0");

	if(lcd==NULL)
		alt_printf ("Error: could not open character LCD device\n");
	else
		alt_printf ("Opened character LCD device\n");

	alt_up_character_lcd_init(lcd);
	alt_up_character_lcd_cursor_off(lcd);
	alt_up_character_lcd_set_cursor_pos(lcd, 8, 0);
	alt_up_character_lcd_string(lcd, "CLOSED");

	char second_row[] = "PRESS BUTTON TO ENTER\0";
	alt_up_character_lcd_set_cursor_pos(lcd, 1, 1);
	alt_up_character_lcd_string(lcd, second_row);

	delay(5);
	alt_up_character_lcd_init(lcd);
	alt_up_character_lcd_set_cursor_pos(lcd, 5, 0);
	alt_up_character_lcd_string(lcd, "ACCESS GRANTED");

	printf("Hello from Nios II!\n");

  return 0;
}
