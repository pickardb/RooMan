

/**************************************************************************
** Subroutine to initialize the RS232 Port by writing some data
** to the internal registers.
** Call this function at the start of the program before you attempt
** to read or write to data via the RS232 port
**
** Refer to 6850 data sheet for details of registers and
***************************************************************************/
#include <stdio.h>
#include <string.h>

#define Wifi_Control (*(volatile unsigned char *)(0x84000210))
#define Wifi_Status (*(volatile unsigned char *)(0x84000210))
#define Wifi_TxData (*(volatile unsigned char *)(0x84000212))
#define Wifi_RxData (*(volatile unsigned char *)(0x84000212))
#define Wifi_Baud (*(volatile unsigned char *)(0x84000214))

char WIFI_MESSAGE_DEPENDENT_IS_SAFE[] =
		"The dependent has confirm their safety.";

void Wifi_Init(void) {

	Wifi_Control = 0x03;
	Wifi_Control = 0x15;
	// Program baud rate generator to use 115k baud.
	Wifi_Baud = 0x01;


}

/**
 * Send a single character to the RS232 chip.
 */
int Wifi_Send_Char(int char_data) {
	// Poll Tx bit in 6850 status register and await for it to become '1'
	while (!(0x02 & Wifi_Status)) {
	};

	// Write the character to the 6850 TxData register.
	Wifi_TxData = char_data;

	return char_data;
}

int Wifi_Get_Char(void) {
	// Poll RX bit in 6850 status register and await for it to become '1'
	while (!(0x01 & Wifi_Status)) {
	};

	// Read the received character from 6850 RxData register.
	return (int) Wifi_RxData;
}

void Wifi_Send_String(char command[]) {
	int i;

	for (i = 0; i < strlen(command); i++) {
		Wifi_Send_Char(command[i]);
	}

	// Send the termination flags, without them the RS232 chip won't know when
	// the command ends and when the next command starts.
	Wifi_Send_Char('\r');
	Wifi_Send_Char('\n');
}





/*
 * Receive a string form the RS232 chip by polling until we reach a newline
 */
void Wifi_Print_Response(){
	char response;
	while(response != '\n'){
		response = (char)Wifi_Get_Char();
		printf("%c", response);
	}
	printf("\n");
}




/**
 * Polls the 6850 to determine if any character has been received.
 * It doesn't wait for one, or read it, it simply tests to see if one is available to read
 */
int Wifi_For_Received_Data(void) {
	// Test Rx bit in the STATUS register
	// if RX bit is set, return TRUE, otherwise return FALSE
	return 0x01 & Wifi_Status;
}

void Wifi_Send_Sms(char message[]) {
	//char lua_sms_command[256];
	printf("Sending sms \n");
	Wifi_Send_String("dofile(\"project2.lua\")");
	Wifi_Send_String("check_wifi()");
	//sprintf(message,
		//	"send_sms(\"%s\")", message);

	Wifi_Send_String(message);
	printf("sms sent \n");
	Wifi_Print_Response();
}


