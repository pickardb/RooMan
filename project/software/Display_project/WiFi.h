

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

// Wi-Fi and baud rate initialization 
void Wifi_Init(void) {

	Wifi_Control = 0x03;	// Reset the wifi control 
	Wifi_Control = 0x15;	// Setup the wifi control 
	Wifi_Baud = 0x01;	// Program baud rate generator to use 115k baud.


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
int Wifi_Get_Char(void) {
	// Poll RX bit in Wi-Fi status register and await for it to become '1'
	while (!(0x01 & Wifi_Status)) {
	};
	return (int) Wifi_RxData;
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

 // Receives a string form the Wi-Fi chip by polling until we reach a newline 
void Wifi_Print_Response(){
	char response;
	while(response != '\n'){
		response = (char)Wifi_Get_Char();
		printf("%c", response);
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
	Wifi_Send_String("dofile(\"project2.lua\")");
	Wifi_Send_String("check_wifi()");
	Wifi_Send_String(message);
	
	printf("sms sent \n");
	Wifi_Print_Response();
}

void Wifi_Patch_Rooms(char message[]) {
	printf("Writing data to db \n");
	
	//Wi-Fi configuration file
	Wifi_Send_String("dofile(\"project2.lua\")");
	Wifi_Send_String("check_wifi()");
	Wifi_Send_String(message);
	
	printf("data written \n");
	Wifi_Print_Response();
}


