#define RS232_Control 	(*(volatile unsigned char *)(0x84000200))
#define RS232_Status 	(*(volatile unsigned char *)(0x84000200))
#define RS232_TxData 	(*(volatile unsigned char *)(0x84000202))
#define RS232_RxData 	(*(volatile unsigned char *)(0x84000202))
#define RS232_Baud 	 	(*(volatile unsigned char *)(0x84000204))
#define solvedCommand 'a'
#define updateRoomStatusCommand 'b'
#define lightOnCommand 'c'
#define lightOffCommand 'd'
#define doorOpenCommand 'e'
#define doorCloseCommand 'f'
#define tempRequestCommand 'h'
#define doorOpenBuzzerCommand 'g'
#define occupiedMessage '5'
#define freeMessage '2'
#define requestRoomMessage '3'
#define lightChangeMessage 'L'

void Init_RS232(void) {
	/**
	 * Set up 6850 Control register to utilise a dive by 16 clock.
	 * set RTS low, use 8 bits of data, no parity, 1 stop bit,
	 * transmitter interrupt disabled
	 * program baud rate generator to use 115k baud
	 */
	printf("Initializing control register\n");
	RS232_Control = 0b00000011;
	RS232_Control = 0b10010101;
	RS232_Baud 	  = 0b00000111; // program for 9600 baud
}


int putcharRS232(char c) {
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

char getcharRS232(void) {
	// poll Rx bit in 6850 status register. Wait for it to become '1'
	// read received characer from 6850 RxData resgister.
	int read_status_bit = 0;
	while(read_status_bit == 0) {
		read_status_bit = RS232_Status & 0b01;
	}
	char character = RS232_RxData;
	return character;
}

int RS232TestForReceivedData(void) {
	return RS232_Status & 0b1;
}

int GetRangeData(void) {
	char received_data;
	received_data = getcharRS232();
	if(received_data=='5'){
		return 1;
	}
	return 0;
}
void SendRangeRequest(void){
	putcharRS232(updateRoomStatusCommand);
}
void OpenServo(void) {
	putcharRS232(doorOpenCommand);
}

void OpenServoBuzzer(void){
	putcharRS232(doorOpenBuzzerCommand);
}
void CloseServo(void) {
	putcharRS232(doorCloseCommand);
}

void TurnOnLights(void) {
	putcharRS232(lightOnCommand);
}

void TurnOffLights(void) {
	putcharRS232(lightOffCommand);
}

void sendTempRequest(void){
	printf("Sending Temp Request\n");
	putcharRS232(tempRequestCommand);
}

int GetTemp(void){
	printf("\nGetting temp\n");
	int tens = getcharRS232()-48;
	//printf("tens is %d\n",tens);
	int ones = getcharRS232()-48;
	//printf("ones is %d\n",ones);
	printf("Temp is %d\n\n",10*tens+ones);
	return 10*tens+ones;
}

void SendSolved (void){
	putcharRS232(solvedCommand);
}

/*int sendLightRequest(){
	putcharRS232(requestLight);
	int lightStatus = getcharRS232()- 48;
	if(lightStatus) {
		printf("Light Status %i, ON \n",lightStatus);
	}
	else {
		printf("Light Status %i, OFF \n",lightStatus);
	}
	return lightStatus;
}*/
