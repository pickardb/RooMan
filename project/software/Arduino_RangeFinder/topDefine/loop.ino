//global value
unsigned int dist;
char incomingCommand;
int request = 0;
int sent = 0;
int buttonState = 0;         // current state of the button
int lastButtonState = 0;  // previous state of the button
int requestCounter = 0;
int tempCounter = 0;

int lightButtonState = 0;         // current state of the button
int lastLightButtonState = 0;  // previous state of the button
int lightStatus = 0;


SoftwareSerial mySerial(RxPin, TxPin); // RX, TX


void loop() {

  //check request button
  checkRequest();
  //check light switch
  checkLight();

  //reading serial command if there is a incoming message
  if (mySerial.available()) {

    Serial.println("trying to read command");
    //incomingCommand = Serial.read();
    incomingCommand = mySerial.read();

    Serial.print("Received command:");
    Serial.print(incomingCommand);
    Serial.print("\n");
    executeCommand(incomingCommand);
  }

}


void checkRequest(void) {
  buttonState = digitalRead(buttonPin);
  //check if the button is pressed
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      request = 1;
      sent = 0;
      Serial.println("Room requested\n");
    } else {
      //falling edge
    }
    // Delay a little bit to avoid bouncing
    delay(50);

    //sending room request message via serial
    if (request && !sent) {
      sendSingleSerialCommand(requestRoomMessage);
      sent = 1;
    }
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;
}

void checkLight() {
   lightButtonState = digitalRead(lightButton);
   //check if the button is pressed
    if(lightButtonState != lastLightButtonState) {
    if (lightButtonState == HIGH) {
      if(lightStatus) {
          Serial.println("Light button pressed, turning OFF");
          lightStatus = 0;
          digitalWrite(ledPin, LOW);
          sendSingleSerialCommand(lightChangeMessage);
        }
      else{
          Serial.println("Light button pressed, turning ON");
          lightStatus = 1;
          digitalWrite(ledPin, HIGH); 
          sendSingleSerialCommand(lightChangeMessage);
        }
      sent = 0;
      
    } else {
      //falling edge
    }
    // Delay a little bit to avoid bouncing
    delay(50);

  }
  // save the current state as the last state, for next time through the loop
  lastLightButtonState = lightButtonState;
}


void executeCommand(char incomingCommand) {
  switch (incomingCommand) {
    case solvedCommand:
      Serial.println("received solved command \n");
      if (!request) {
        Serial.println("No Room Request\n");
      }
      else {
        request = 0;
        Serial.println("Room request solved\n");
      }
      break;
    case updateRoomStatusCommand:
      Serial.println("Update Room Status");
      dist = RunRange();
      Serial.print("Range is: ");
      Serial.println(dist);
      if (dist < rangerFinderThreshold) {
        Serial.println("Room Occupied");
        sendSingleSerialCommand(occupiedMessage);
      }
      else {
        Serial.println("Room free");
        sendSingleSerialCommand(freeMessage);
      }
      break;

    case doorOpenCommand:
      Serial.println("Door Open");
      turnServo(openPos);
      break;

    case doorOpenBuzzerCommand:
      Serial.println("Door Open");
      turnServo(openPos);
      doorUnlockTone();
      break;
    case doorCloseCommand:
      Serial.println("Door Closed");
      turnServo(closedPos);
      break;

    case lightOnCommand:
      Serial.println("Lights ON");
      digitalWrite(ledPin, HIGH);
      lightStatus = 1;
      break;
    case lightOffCommand:
      Serial.println("Lights OFF");
      digitalWrite(ledPin, LOW);
      lightStatus = 0;
      break;
    case tempRequestCommand:
      Serial.println("Update Temperature");
      Temp();
      break;     
    default:
      Serial.print("Unknow command: ");
      Serial.println(incomingCommand);
      break;
  }
}

void sendSingleSerialCommand(char command) {
  Serial.print("Sending Command:");
  Serial.println(command);
  //send command via serial
  mySerial.print(command);
}


void Temp (void) {
  int chk = DHT.read11(DHTpin);
  int temp;
  int tens;
  int ones;
  switch (chk)
  {
    case DHTLIB_OK:
      //get the digits of the temperature
      temp = DHT.temperature;
      tens = temp / 10;
      ones = temp % 10;
      sendSingleSerialCommand(tempMessage);
      delay(100);
      //converting int to char
      sendSingleSerialCommand(tens + 48);
      delay(100);
      //converting int to char
      sendSingleSerialCommand(ones + 48);
      break;
  }
}

void doorUnlockTone(void) {
  tone(buzzerPin, 1000, 1500);
}
