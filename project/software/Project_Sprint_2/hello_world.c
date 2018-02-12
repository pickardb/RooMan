#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

//Custom inlcude Files
#include "Colours.h"
#include "Fonts.h"
//#include "ultrasonic_direct.h"
#include <altera_up_avalon_character_lcd.h>
#include "Display.h"
#include "Touch.h"
#include "Serial.h"



void delay(int number_of_seconds) {
	int milli_seconds = 1000 * number_of_seconds;
	clock_t start_time = clock();
	while (clock() < start_time + milli_seconds)
		;
}



void ResolvedRequest(void) {
	putcharRS232(0x48);

}

void BaseDisplay(void) {
	int i;
	int j;

	for (i = 0; i < 800; i++)
		DrawHorizontalLine(0, 800, i, TEAL);
	char* roomString;
	for (j = 1; j <= 10; j++) {
		sprintf(roomString, "Room %d", j);
		DrawRect(100, 300, 40 * j, 40 * (j + 1), BLACK);
		DrawString(105, 40 * j + 40 / 4, BLACK, TEAL, roomString, 2, 1);
	}
	printf("Base Displayed \n");
}

void InfoDisplay(int room_num, short lights, short door, short occupied,
		short in_use) {
	int i;
	int j;

	//Cover Last Data
	for (i = 0; i < 480; i++)
		DrawHorizontalLine(0, 800, i, TEAL);
	for (j = 1; j <= 10; j++) {
		DrawRect(100, 300, 40 * j, 40 * (j + 1), BLACK);
	}
	//Change Colour of selected Room
	DrawFillRect(101, 299, 40 * room_num + 1, 40 * (room_num + 1) - 1, NAVY);

	char* roomDeetsString;
	sprintf(roomDeetsString, "Room %d Details & Options", room_num);
	DrawRect(350, 750, 40, 40 * 11, BLACK);
	DrawString(360, 40 * 1.25, BLACK, TEAL, roomDeetsString, 2, 1);
	DrawString(400, 40 * 2.25 - 14, BLACK, TEAL, "In Use: ", 2, 1);
	if (in_use) {
		DrawString(530, 40 * 2.25 - 14, GREEN, TEAL, "YES", 2, 1);
	} else {
		DrawString(530, 40 * 2.25 - 14, CRIMSON, TEAL, "NO", 2, 1);

	}
	DrawString(400, 40 * 3.25 - 14, BLACK, TEAL, "Occupied: ", 2, 1);
	if (occupied) {
		DrawString(530, 40 * 3.25 - 14, GREEN, TEAL, "YES", 2, 1);
	} else {
		DrawString(530, 40 * 3.25 - 14, CRIMSON, TEAL, "NO", 2, 1);
	}

	DrawString(400, 40 * 4.25 - 14, BLACK, TEAL, "Lights: ", 2, 1);
	if (lights) {
		DrawString(530, 40 * 4.25 - 14, GREEN, TEAL, "ON", 2, 1);
	} else {
		DrawString(530, 40 * 4.25 - 14, CRIMSON, TEAL, "OFF", 2, 1);
	}

	DrawString(400, 40 * 5.25 - 14, BLACK, TEAL, "Door: ", 2, 1);
	if (door) {
		DrawString(530, 40 * 5.25 - 14, GREEN, TEAL, "UNLOCKED", 2, 1);
	} else {
		DrawString(530, 40 * 5.25 - 14, CRIMSON, TEAL, "LOCKED", 2, 1);
	}

	DrawString(400, 40 * 6.25 - 14, BLACK, TEAL, "Lights: ", 2, 1);
	DrawFillRect(480, 560, 40 * 6.25 - 34, 40 * 7.25 - 24, CRIMSON);
	DrawString(490, 40 * 6.25 - 14, BLACK, CRIMSON, "OFF", 2, 1);
	DrawFillRect(570, 650, 40 * 6.25 - 34, 40 * 7.25 - 24, GREEN);
	DrawString(580, 40 * 6.25 - 14, BLACK, GREEN, "ON", 2, 1);

	DrawString(400, 40 * 7.75 - 14, BLACK, TEAL, "Door: ", 2, 1);
	DrawFillRect(480, 560, 40 * 7.75 - 34, 40 * 8.75 - 24, CRIMSON);
	DrawString(490, 40 * 7.75 - 14, BLACK, CRIMSON, "LOCK", 2, 1);
	DrawFillRect(570, 650, 40 * 7.75 - 34, 40 * 8.75 - 24, GREEN);
	DrawString(580, 40 * 7.75 - 14, BLACK, GREEN, "UNLOCK", 2, 1);

}

void RequestDotDisplay(int room_num) {

	DrawFillRect(315, 335, 40 * room_num + 10, 40 * (room_num + 1) - 10, RED);
	return;

}

int BaseChoice(void) {
	Point p1;
	while (1) {
		printf("Base Choice\n");
		p1 = GetPress();
		if (p1.x >= 100 && p1.x <= 300 && p1.y >= 40 && p1.y <= 440) {
			printf("Chose #%d\n", p1.y / 40);
			return floor(p1.y / 40);
		}
	}
	return -1;
}

int InfoChoice(void) {
	Point p1;
	while (1) {
		printf("Info Choice\n");
		p1 = GetPress();
		if (p1.x >= 100 && p1.x <= 300 && p1.y >= 40 && p1.y <= 440) {
			printf("Room #%d\n", (p1.y) / 40);
			return floor((p1.y - 10) / 40);
		} else if (p1.x >= 480 && p1.x <= 560 && p1.y <= 40 * 7.25 - 24
				&& p1.y >= 40 * 6.25 - 34) {
			//Lights off Function
			printf("Turning Lights Off\n");
			return 11;
		} else if (p1.x >= 570 && p1.x <= 650 && p1.y <= 40 * 7.25 - 24
				&& p1.y >= 40 * 6.25 - 34) {
			//Lights on function
			printf("Turning Lights On\n");
			return 12;
		} else if (p1.x >= 480 && p1.x <= 560 && p1.y <= 40 * 8.75 - 24
				&& p1.y >= 40 * 7.75 - 34) {
			//Door lock Function
			printf("lock Door\n");
			return 13;
		} else if (p1.x >= 570 && p1.x <= 650 && p1.y <= 40 * 8.75 - 24
				&& p1.y >= 40 * 7.75 - 34) {
			//Door Lock function
			printf("Turning Lights Off\n");
			return 14;
		}
	}
	return -1;
}

void PrintNumbers(int room_num) {
	int j;
	char* roomString;
	for (j = 1; j <= 10; j++) {
		sprintf(roomString, "Room %d", j);
		if (j == room_num) {
			DrawString(105, 40 * j + 40 / 4, BLACK, NAVY, roomString, 2, 1);

		} else {
			DrawString(105, 40 * j + 40 / 4, BLACK, TEAL, roomString, 2, 1);
		}
	}
}

void RunDisplay(void) {
	int last_room_num;
	int curr_room_num;

	int i;
	for (i = 0; i < 10; i++) {
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
	while (1) {
		if (last_room_num >= 1 && last_room_num <= 10) {
			curr_room_num = last_room_num;
		} else if (last_room_num == -1) {
			printf("Error, my dude\n");
		} else if (last_room_num == 11) {
			roomArray[curr_room_num - 1].lights = 0;
			if (curr_room_num == 1) {
				TurnOnLights();
			}
		} else if (last_room_num == 12) {
			roomArray[curr_room_num - 1].lights = 1;
			if (curr_room_num == 1) {
				TurnOnLights();
			}
		} else if (last_room_num == 13) {
			roomArray[curr_room_num - 1].door = 0;
			if (curr_room_num == 1) {
				TurnServo();
			}
			if (roomArray[curr_room_num - 1].requested) {
				roomArray[curr_room_num - 1].requested = 0;
				ResolvedRequest();
			}
		} else if (last_room_num == 14) {
			roomArray[curr_room_num - 1].door = 1;
			if (curr_room_num == 1) {
				TurnServo();
			}
			if (roomArray[curr_room_num - 1].requested) {
				roomArray[curr_room_num - 1].requested = 0;
				roomArray[curr_room_num - 1].in_use = 1;
				ResolvedRequest();
			}
		}

		InfoDisplay(curr_room_num, roomArray[curr_room_num - 1].lights,
				roomArray[curr_room_num - 1].door,
				roomArray[curr_room_num - 1].occupied,
				roomArray[curr_room_num - 1].in_use);
		int k;
		for (k = 0; k < 10; k++) {
			if (roomArray[k].requested == 1) {
				RequestDotDisplay(k + 1);
			}
		}
		PrintNumbers(curr_room_num);
		last_room_num = InfoChoice();
	}
}

int main(void) {

	alt_up_character_lcd_dev *lcd;

	lcd = alt_up_character_lcd_open_dev("/dev/character_lcd_0");
	alt_printf("Test");
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

	RunDisplay();

	printf("Finished");

	return 0;
}

