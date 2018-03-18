#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/alt_irq.h>

//Custom inlcude Files
#include "Colours.h"
#include "Fonts.h"
#include "Display.h"
#include "Touch.h"
#include "Serial.h"
#include "Bluetooth-Configuration.h"
#include "ISR.h"
#include "WiFi.h"


#define LIGHTS_OFF 11
#define LIGHTS_ON 12
#define LOCK_DOOR 13
#define UNLOCK_DOOR 14
#define AUTO_APPROVE 15
#define ERROR -1

int auto_approve = 0;

/*
 * Display The room selection column on the left hand side of the screen
 */
void BaseDisplay(void) {
	int i;
	char* roomString;

	for (i = 0; i < 800; i++)
		DrawHorizontalLine(0, 800, i, TEAL);
	for (i = 1; i <= 10; i++) {
		sprintf(roomString, "Room %d", i);
		DrawRect(100, 300, 40 * i, 40 * (i + 1), BLACK);
		DrawString(105, 40 * i + 40 / 4, BLACK, TEAL, roomString, 2, 1);
	}
	printf("Base Displayed \n");
}

/*
 * Full Room status Display. Base display on the left, and room status on the right
 * Takes in the current room number and info about that room and displays it on the screen
 */
void InfoDisplay(int room_num, int lights, int door, int occupied, int in_use, int temp) {
	int i;

	//Cover Last Data
	for (i = 0; i < 480; i++)
		DrawHorizontalLine(0, 800, i, TEAL);
	for (i = 1; i <= 10; i++) {
		DrawRect(100, 300, 40 * i, 40 * (i + 1), BLACK);
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

	if(temp>0){
		DrawString(400,40*8.5+12,BLACK,TEAL,"Temperature:",2,1);
		sprintf(roomDeetsString, "%d", temp);
		DrawString(530,40*8.5+12,BLACK,TEAL,roomDeetsString,2,1);
	}


	if(auto_approve){
		DrawFillRect(480,650,40*9.5,40*10.5,GREEN);
		DrawString(505,40*9.5+12,BLACK,GREEN,"Auto-Approve",2,1);
	}
	else{
		DrawFillRect(480,650,40*9.5,40*10.5,CRIMSON);
		DrawString(505,40*9.5+12,BLACK,CRIMSON,"Auto-Approve",2,1);
	}

}

/*
 * Draws a small Red square next to the given room number
 */
void RequestCloseDisplay(int room_num) {

	DrawFillRect(315, 335, 40 * room_num + 10, 40 * (room_num + 1) - 10, RED);
	return;

}

/*
 * Draws a small green square next to the given room number
 */
void RequestOpenDisplay(int room_num) {

	DrawFillRect(315, 335, 40 * room_num + 10, 40 * (room_num + 1) - 10, GREEN);
	return;

}

/*
 * A function to check where the user has pressed after the base display.
 * If the user selects a room, it returns the appropriate room number
 */
int BaseChoice(void) {
	Point p1;
	while (1) {
		printf("Base Choice\n");
		p1 = GetBasePress();
		if (p1.x >= 100 && p1.x <= 300 && p1.y >= 40 && p1.y <= 440) {
			printf("Chose room# %d\n", p1.y / 40);
			return floor(p1.y / 40);
		}
	}
	return -1;
}

/*
 * A function which waits for either the screen to be touched or receive a serial command
 * upon receving a command or touch data, it returns the command/data
 */
char waitForInterrupt (void){
	char received_data;
	while(!ScreenTouched()){
		if(RS232TestForReceivedData()){
			//delay_double(0.1);
			received_data = getcharRS232();
			printf("Received: %c\n", received_data);
			return received_data;
		}
	}
	return 0;
}

/*
 * A function to check where the user has pressed on the room status screen
 * Returns the room number if pressed, or a specific command for certain button presses
 */

int InfoSelect (Point p1){
	if (p1.x >= 100 && p1.x <= 300 && p1.y >= 40 && p1.y <= 440) {
		printf("Chose room# %d\n", (p1.y) / 40);
		return floor((p1.y - 10) / 40);
	} else if (p1.x >= 480 && p1.x <= 560 && p1.y <= 40 * 7.25 - 24
			&& p1.y >= 40 * 6.25 - 34) {
		//Lights off Function
		printf("Turning Lights Off\n");
		return LIGHTS_OFF;
	} else if (p1.x >= 570 && p1.x <= 650 && p1.y <= 40 * 7.25 - 24
			&& p1.y >= 40 * 6.25 - 34) {
		//Lights on function
		printf("Turning Lights On\n");
		return LIGHTS_ON;
	} else if (p1.x >= 480 && p1.x <= 560 && p1.y <= 40 * 8.75 - 24
			&& p1.y >= 40 * 7.75 - 34) {
		//Door lock Function
		printf("Locking Door\n");
		return LOCK_DOOR;
	} else if (p1.x >= 570 && p1.x <= 650 && p1.y <= 40 * 8.75 - 24
			&& p1.y >= 40 * 7.75 - 34) {
		//Door unlock function
		printf("Unlocking door\n");
		return UNLOCK_DOOR;
	}
	else if (p1.x>=480 && p1.x<=650 && p1.y >= 40*9 && p1.y <= 40*10){
		printf("Toggle Auto Approve\n");
		return AUTO_APPROVE;
	}
	return 0;
}

/*
 * Calls wait for interrupt, and receives the returned command
 * Determines the correct command to execute and returns a corresponding command
 * to the main loop which will execute the command
 */
int InfoChoice( int room_num) {
	Point p1;
	char command;
	int ret;
	char *message = (char*)malloc(100 * sizeof(char));
	while (1) {
		//printf("Info Choice\n");
		command = waitForInterrupt();
		if(command==0){
			p1 = GetBasePress();
			ret = InfoSelect(p1);
			if(ret)return ret;
		}
		else if(command=='t'){
			//TempReadingFunction
			roomArray[room_num-1].temp = GetTemp();
			return 99;
		}
		else if(command=='5'){
			//Room is occupied
			roomArray[room_num-1].occupied = 1;
		}
		else if(command=='2'){
			//Room is free
			roomArray[room_num-1].occupied = 0;
		}
		// If the request a room button was pressed   
		else if (command=='3'){
			roomArray[curr_room_num-1].requested = 1;
			//sprintf(message, "send_sms(\"The room requested: %d\")", curr_room_num);	// Customizes the message to specify which room was requested
			//Wifi_Send_Sms(message);								// Sends the message through Wi-Fi chip
			if(auto_approve && roomArray[curr_room_num-1].in_use == 0){
				return UNLOCK_DOOR;
			}
			else if(auto_approve && roomArray[room_num-1].in_use == 1){
				return LOCK_DOOR;
			}
			return 99;
		}
		else if (command == 'L'){
			if(roomArray[0].lights){
				roomArray[0].lights = 0;
			}
			else{
				roomArray[0].lights = 1;
			}
			return 99;
		}

	}
	return -1;
}

/*
 * Prints the room numbers on top of the left hand room select column,
 * with the selected room having a different background
 */
void PrintNumbers(int room_num) {
	int j;
	char* roomString = NULL;
	for (j = 1; j <= 10; j++) {
		sprintf(roomString, "Room %d", j);
		if (j == room_num) {
			DrawString(105, 40 * j + 40 / 4, BLACK, NAVY, roomString, 2, 1);

		} else {
			DrawString(105, 40 * j + 40 / 4, BLACK, TEAL, roomString, 2, 1);
		}
	}
}

/*
 * Initializes all the values in the room data array.
 * Also hardcodes requests into room 3 and 4 for testing and demo purposes
 */
void InitRoomArray(void){
	int i;
	for (i = 0; i < 10; i++) {
		roomArray[i].lights = 0;
		roomArray[i].door = 0;
		roomArray[i].requested = 0;
		roomArray[i].in_use = 0;
		roomArray[i].occupied = 0;
		roomArray[i].temp = 0;
	}
	roomArray[3].requested = 1;
	roomArray[4].requested = 1;
}

/*
 * Takes in a command, and executes the command, either selecting a room or changing some hardware
 */
void executeCommand (int last_room_num){
	int k;
	if (last_room_num >= 1 && last_room_num <= 10) {
				curr_room_num = last_room_num;
			}
			else if (last_room_num == ERROR) {
				printf("Error, my dude\n");
			}
			else if (last_room_num == LIGHTS_OFF) {
				roomArray[curr_room_num - 1].lights = 0;
				if (curr_room_num == 1) {
					TurnOffLights();
				}
			}
			else if (last_room_num == LIGHTS_ON) {
				roomArray[curr_room_num - 1].lights = 1;
				if (curr_room_num == 1) {
					TurnOnLights();
				}
			}
			else if (last_room_num == LOCK_DOOR) {
				roomArray[curr_room_num - 1].door = 0;
				if (curr_room_num == 1) {
					CloseServo();
				}
				if (roomArray[curr_room_num - 1].in_use==0 && roomArray[curr_room_num - 1].requested ) {
					roomArray[curr_room_num - 1].requested = 0;
				}
				else if (roomArray[curr_room_num - 1].in_use==1 && roomArray[curr_room_num - 1].requested ) {
					roomArray[curr_room_num - 1].requested = 0;
					roomArray[curr_room_num-1].in_use = 0;
					roomArray[curr_room_num-1].lights = 0;
					if (curr_room_num == 1) {
						CloseServo();
						TurnOffLights();
					}
				}
			} else if (last_room_num == UNLOCK_DOOR) {
				roomArray[curr_room_num - 1].door = 1;
				if (curr_room_num == 1) {
					OpenServo();
				}
				if (roomArray[curr_room_num - 1].requested && roomArray[curr_room_num - 1].in_use==0) {
					roomArray[curr_room_num - 1].requested = 0;
					roomArray[curr_room_num - 1].in_use = 1;
					roomArray[curr_room_num - 1].lights = 1;
					if (curr_room_num == 1) {
						OpenServo();
						TurnOnLights();

					}
				}
			}
			else if (last_room_num == AUTO_APPROVE){
				if(auto_approve){
					auto_approve=0;
				}
				else{
					auto_approve = 1;
					for (k = 0; k < 10; k++) {
						if(roomArray[k].requested){
							roomArray[k].requested=0;
							roomArray[k].in_use = 1;
							roomArray[k].lights = 1;
							roomArray[k].door = 1;
							if (k == 0) {
								OpenServo();
								TurnOnLights();
							}
						}
					}
				}
			}
}

/*
 * For each room, checks if there is a request. If so, prints the request indicator on the screen
 */
void displayRequests (void){
	int k;
	for (k = 0; k < 10; k++) {
		if (roomArray[k].requested == 1 && roomArray[k].in_use==1) {
			RequestCloseDisplay(k +1);
		}
		else if (roomArray[k].requested == 1 && roomArray[k].in_use==0) {
			RequestOpenDisplay(k +1);
		}
	}
}

/*
 * Main loop to run the program.
 * Starts off initialzing all the serial outputs and bluetooth
 * Continues into the display and gets the users touchscreen press
 * Executes the commands returned from InfoChoice() and updates the roomArray as necessary
 */
void RunDisplay(void) {

	int last_room_num;

	// Initialization of all internal and external components
	AttemptBluetoothConnection();
	InitRoomArray();
	Init_Touch();
	//Wifi_Init();
	Init_ISR();

	BaseDisplay();
	last_room_num = BaseChoice();

	while (1) {
		executeCommand(last_room_num);
		printf("Starting Info Display\n");
		Wifi_update_database(curr_room_num);
		InfoDisplay(curr_room_num, roomArray[curr_room_num - 1].lights,roomArray[curr_room_num - 1].door,roomArray[curr_room_num - 1].occupied,roomArray[curr_room_num - 1].in_use, roomArray[curr_room_num - 1].temp);
		displayRequests();
		PrintNumbers(curr_room_num);
		last_room_num = InfoChoice(curr_room_num);
	}
}


int main(void) {

	RunDisplay();

	printf("Finished");

	return 0;
}
