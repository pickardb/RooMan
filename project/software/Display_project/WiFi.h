

/**************************************************************************
** Subroutine to initialize the Wi-Fi Port by writing some data
** to the internal registers.
***************************************************************************/
#include <stdio.h>
#include <string.h>

#define Wifi_Control (*(volatile unsigned char *)(0x84000210))
#define Wifi_Status (*(volatile unsigned char *)(0x84000210))
#define Wifi_TxData (*(volatile unsigned char *)(0x84000212))
#define Wifi_RxData (*(volatile unsigned char *)(0x84000212))
#define Wifi_Baud (*(volatile unsigned char *)(0x84000214))

char response[1000];

// Wi-Fi and baud rate initialization 
void Wifi_Init(void) {

	Wifi_Control = 0x03;	// Reset the wifi control 
	Wifi_Control = 0x15;	// Setup the wifi control 
	Wifi_Baud = 0x01;	// Program baud rate generator to use 115k baud.
	Wifi_Send_String("dofile(\"project2.lua\")");

}

// Sends a single character to the Wi-Fi chip.
int Wifi_Send_Char(int char_data) {
	// Poll Tx bit in Wi-Fi status register and await for it to become '1'
	while (!(0x02 & Wifi_Status)) {
	};

	// Write the character to the Wi-Fi TxData register.
	Wifi_TxData = char_data;
	return char_data;
}

// Reads the received character from Wi-Fi RxData register.
char Wifi_Get_Char(void) {
	// Poll RX bit in Wi-Fi status register and await for it to become '1'
	int i = 0;
	while (!(0x01 & Wifi_Status)) {
		i++;
		if(i>=5000)return 0;
	};
	return (char) Wifi_RxData;
}
// Sends the string command to the Wi-Fi chip 
void Wifi_Send_String(char command[]) {
	
	int i;
	// Send the whole string command by sending each character
	for (i = 0; i < strlen(command); i++) {
		Wifi_Send_Char(command[i]);
	}

	// Send the termination flags, without them the Wi-Fi chip won't know when
	// the command ends and when the next command starts.
	Wifi_Send_Char('\r');
	Wifi_Send_Char('\n');
}

void Wifi_Send_String_without(char command[]) {

	int i;
	// Send the whole string command by sending each character
	for (i = 0; i != '\0'; i++) {
		Wifi_Send_Char(command[i]);
	}

	// Send the termination flags, without them the Wi-Fi chip won't know when
	// the command ends and when the next command starts.
}

 // Receives a string form the Wi-Fi chip by polling until we reach a newline 
void Wifi_Print_Response(){
	char response;
	printf("Wifi response is: ");
	//int timeout = 0;
	while(response != '\n'){
		response = (char)Wifi_Get_Char();
		printf("%c", response);
		/*timeout++;
		if(timeout>300){
			printf("(Print Response Timeout)");
			break;
		}*/
	}
	printf("\n");
}

void Wifi_Print_Response_Hex(){
	//char response[1000];
	printf("Wifi (hex) response is: ");
	int i=0;
	while(response != '}'){
		response[i] = (char)Wifi_Get_Char();
		i++;
		if(i==999){
			break;
		}

		//timeout++;
		//if(response=='\n')line_count++;
		//if(line_count==15)break;
		//if(line_count>=14){

		//}
		//if(timeout>2000)break;

	}
	for (i=0;i<1000;i++){
		printf("%c",response[i]);
	}
	printf("\n");
}


// Polls the Wi-Fi to determine if any character has been received. 
int Wifi_For_Received_Data(void) {
	// Test Rx bit in the Wi-Fi status register
	// if RX bit is set, return TRUE, otherwise return FALSE
	return 0x01 & Wifi_Status;
}

/*
 * Activates the file saved in the Wi-Fi chip and checks for connectivity, if everything is fine, sends the message to the Wi-Fi
 * which is then will be sent to the admin's phone 
 */
void Wifi_Send_Sms(char message[]) {
	printf("Sending sms \n");
	
	//Wi-Fi configuration file

	Wifi_Send_String("check_wifi()");
	Wifi_Send_String(message);
	//Wifi_Send_String("dofile(\"project2.lua\")");

	printf("sms sent \n");
	Wifi_Print_Response();
}

void Wifi_Patch_Rooms(char message[]) {
	printf("Writing data to db \n");
	
	//Wi-Fi configuration file
	Wifi_Send_String("check_wifi()");
	Wifi_Print_Response();
	//Wifi_Send_String("check_wifi()");
	Wifi_Send_String(message);
	//Wifi_Send_String(message);
	printf(message);
	printf("data written \n");
	Wifi_Print_Response();

}

void Wifi_Get_Rooms(char message[]) {
	printf("Reading data from db \n");
	printf("Check_wifi()\n");
	Wifi_Send_String("check_wifi()");

	printf("%s \n",message);
	Wifi_Send_String(message);
	Wifi_Print_Response();


	printf("get_data()\n");
	Wifi_Send_String_without("get_data()");
	Wifi_Print_Response_Hex();

	int err = parse_get_data(response);
	if(err==0)printf("Error Parsing HTML data \n");


}

void update_room (int room_num, int door, int lights){
	if(roomArray[room_num-1].door==1&&door==0){
		if(room_num==1)CloseServo();
	}
	else if(roomArray[room_num-1].door==0&&door==1){
		if(room_num==1)OpenServo();
	}
	if(roomArray[room_num-1].lights==1&&lights==0){
		if(room_num==1)TurnOffLights();
	}
	else if(roomArray[room_num-1].lights==0&&lights==1){
		if(room_num==1)TurnOnLights();
	}
}

int parse_get_data (char response[1000]){
	int i=0;
	int roomnum=-1;
	int tens=0;
	int ones=0;
	int inUse=-1;
	int lights=-1;
	int locked=-1;
	int occupied=-1;
	int temp=-1;

	while(response[i]!='{'){
		i++;
		if(i>=1000){
			return 0;
		}
	}
	i+=8;
	printf("I am at position %d with characters %c%c%c\n",i,response[i-1],response[i],response[i+1]);
	tens = response[i]-48;
	if(response[i+1]!='"'){
		ones=response[i+1]-48;
		roomnum=10*tens+ones;
		i++;
	}
	else if(response[i+1]=='"'){
		roomnum = tens;
	}
	else return 0;
	i+=11;
	printf("I am at position %d with characters %c%c%c\n",i,response[i-1],response[i],response[i+1]);
	if(response[i]=='t'){
		inUse=1;
		i+=16;
	}
	else if(response[i]=='f'){
		inUse=0;
		i+=17;
	}
	else return 0;
	printf("I am at position %d with characters %c%c%c\n",i,response[i-1],response[i],response[i+1]);
	if(response[i]=='t'){
		lights=1;
		i+=14;
	}
	else if(response[i]=='f'){
		lights=0;
		i+=15;
	}
	printf("I am at position %d with characters %c%c%c\n",i,response[i-1],response[i],response[i+1]);
	if(response[i]=='t'){
		locked=1;
		i+=16;
	}
	else if(response[i]=='f'){
		locked=0;
		i+=17;
	}
	else return 0;
	printf("I am at position %d with characters %c%c%c\n",i,response[i-1],response[i],response[i+1]);
	if(response[i]=='t'){
		occupied=1;
		i+=19;
	}
	else if(response[i]=='f'){
		occupied=0;
		i+=20;
	}
	else return 0;
	printf("I am at position %d with characters %c%c%c\n",i,response[i-1],response[i],response[i+1]);
	tens = response[i]-48;
	if(response[i+1]!='"'){
		ones = response[i+1]-48;
		temp = 10*tens+ones;
	}
	else if(response[i+1]=='"'){
		temp=tens;
	}
	else return 0;
	printf("%d %d %d %d %d %d \n",roomnum,inUse,lights,locked,occupied,temp);
	update_room(roomnum,!locked,lights);
	roomArray[roomnum-1].in_use = inUse;
	roomArray[roomnum-1].lights = lights;
	if(locked)roomArray[roomnum-1].door = 0;
	else roomArray[roomnum-1].door=1;
	roomArray[roomnum-1].occupied = occupied;
	//roomArray[roomnum-1].temp = temp;

	return 1;
}


