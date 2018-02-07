
#define RS232_Control 	(*(volatile unsigned char *)(0x84000200))
#define RS232_Status 	(*(volatile unsigned char *)(0x84000200))
#define RS232_TxData 	(*(volatile unsigned char *)(0x84000202))
#define RS232_RxData 	(*(volatile unsigned char *)(0x84000202))
#define RS232_Baud 	 	(*(volatile unsigned char *)(0x84000204))
#define lightOnCommand 'c'
#define lightOffCommand 'd'
#define doorOpenCommand 'e'
#define doorCloseCommand 'f'
#define occupiedMessage '1'
#define freeMessage '2'
#define requestRoomMessage '3'


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
	RS232_Baud 	  = 0b00000001; // program for 115k baud
}


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

int GetRangeData(void) {
	putcharRS232(0x44);
	int c;
	c = getbitRS232();
	printf("RangeData: %d\n", c);
	return c;
}

int OpenServo(void) {
	putcharRS232(doorOpenCommand);
	return 1;
}

int CloseServo(void) {
	putcharRS232(doorCloseCommand);
	return 1;
}

void TurnOnLights(void) {
	putcharRS232(lightOnCommand);
}

void TurnOffLights(void) {
	putcharRS232(lightOffCommand);
}

int GetButtonPress(void) {
	putcharRS232(0x47);
	char c = getbitRS232();
	printf("Button State: %d\n", c);
	if (c == 'a') {
		return 1;
	} else
		return 0;

}



