#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

//Custom inlcude Files
#include "Colours.h"
#include "Fonts.h"
#include "Display.h"
#include "Touch.h"
#include "Serial.h"
#include "Bluetooth-Configuration.h"

#define LIGHTS_OFF 11
#define LIGHTS_ON 12
#define LOCK_DOOR 13
#define UNLOCK_DOOR 14
#define AUTO_APPROVE 15
#define ERROR -1

int auto_approve = 0;



void delay(int number_of_seconds) {
	int milli_seconds = 1000 * number_of_seconds;
	clock_t start_time = clock();
	while (clock() < start_time + milli_seconds){}
}

void delay_double(double number_of_seconds) {
	int milli_seconds = 1000 * number_of_seconds;
	clock_t start_time = clock();
	while (clock() < start_time + milli_seconds){}
}


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

void RequestDotDisplay(int room_num) {

	DrawFillRect(315, 335, 40 * room_num + 10, 40 * (room_num + 1) - 10, RED);
	return;

}

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

char waitForInterrupt (void){
	char received_data;
	while(!ScreenTouched()){
		if(RS232TestForReceivedData()){
			delay_double(0.1);
			received_data = getcharRS232();
			printf("Received: %c\n", received_data);
			return received_data;
		}
	}
	return 0;
}

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

int InfoChoice( int curr_room_num) {
	Point p1;
	char command;
	int ret;
	while (1) {
		printf("Info Choice\n");
		//p1 = GetPress();
		command = waitForInterrupt();
		if(command==0){
			p1 = GetBasePress();
			ret = InfoSelect(p1);
			if(ret)return ret;
		}
		else if(command=='t'){
			//TempReadingFunction
			roomArray[curr_room_num-1].temp = GetTemp();
			return 99;
		}
		else if (command=='3'){
			roomArray[curr_room_num-1].requested = 1;
			if(auto_approve && roomArray[curr_room_num-1].in_use == 0){
				return UNLOCK_DOOR;
			}
			else if(auto_approve && roomArray[curr_room_num-1].in_use == 1){
				return LOCK_DOOR;
			}
			return 99;
		}

	}
	return -1;
}

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

void RunDisplay(void) {
	int last_room_num;
	int curr_room_num;
	int k;

	AttemptBluetoothConnection();
	InitRoomArray();
	Init_Touch();
	//Init_RS232();
	BaseDisplay();
	last_room_num = BaseChoice();


	while (1) {
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
				//SendSolved();
			}
			else if (roomArray[curr_room_num - 1].in_use==1 && roomArray[curr_room_num - 1].requested ) {
				roomArray[curr_room_num - 1].requested = 0;
				roomArray[curr_room_num-1].in_use = 0;
			}
		} else if (last_room_num == UNLOCK_DOOR) {
			roomArray[curr_room_num - 1].door = 1;
			if (curr_room_num == 1) {
				OpenServo();
			}
			if (roomArray[curr_room_num - 1].requested && roomArray[curr_room_num - 1].in_use==0) {
				roomArray[curr_room_num - 1].requested = 0;
				roomArray[curr_room_num - 1].in_use = 1;
				//SendSolved();
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
						roomArray[k].door = 1;
					}
				}
			}
		}
		//delay_double(0.1);
		roomArray[curr_room_num-1].occupied = GetRangeData();
		printf("Starting Info Display\n");
		InfoDisplay(curr_room_num, roomArray[curr_room_num - 1].lights,roomArray[curr_room_num - 1].door,roomArray[curr_room_num - 1].occupied,roomArray[curr_room_num - 1].in_use, roomArray[curr_room_num - 1].temp);
		for (k = 0; k < 10; k++) {
			if (roomArray[k].requested == 1) {
				RequestDotDisplay(k +1);
			}
		}
		//delay(2);
		PrintNumbers(curr_room_num);
		//sendTempRequest();
		last_room_num = InfoChoice(curr_room_num);
	}
}

void TestSerial(void){
	Init_RS232();
	while(1){
		TurnOnLights();
		delay_double(0.1);
		TurnOffLights();
		delay_double(0.1);
	}
}

int main(void) {

	//TestSerial();
	RunDisplay();

	printf("Finished");

	return 0;
}

