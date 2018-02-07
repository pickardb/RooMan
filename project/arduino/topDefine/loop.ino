//global value
unsigned int dist;
char incomingCommand;
int request = 0;  
int sent = 0;
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button



void loop() {
  // check the button
  checkRequest();
 
  if (Serial.available() > 0) {
    incomingCommand = Serial.read();
    //incomingCommand = mySerial.read();
    executeCommand(incomingCommand);
   }
}


void checkRequest(void){
  buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      request = 1;
      sent = 0;
      Serial.println("Room requested\n");
    } else {
      //falling edge
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;

   if (request && !sent) {
    sendSingleSerialCommand(requestRoomMessage);
    sent = 1;
  }
  
}

  
void executeCommand(char incomingCommand){
  switch(incomingCommand){
    case solvedCommand:
      Serial.println("received solved command \n");
      if(!request){
        Serial.println("No Room Request\n");
      }
      else{
        request = 0;
        Serial.println("Room request solved\n");
      }
      break;
    case updateRoomStatusCommand:
      Serial.println("Update Room Status");
      dist = RunRange();
      if(dist < rangerFinderThreshold){
          Serial.println("Room Occupied");
          sendSingleSerialCommand(occupiedMessage);
        }
      else{
          Serial.println("Room free");
          sendSingleSerialCommand(freeMessage);
        }
      break;

    case doorOpenComand:
      Serial.println("Door Open");
      turnServo(openPos);
      break;
    case doorCloseComand:
      Serial.println("Door Closed");
      turnServo(closedPos);
      break;

    case lightOnCommand:
      Serial.println("Lights ON");
      digitalWrite(ledPin, HIGH);
      break;
    case lightOffComand:
      Serial.println("Lights OFF");
      digitalWrite(ledPin, LOW);
      break;

    default:
      Serial.print("Unknow command: ");
      Serial.println(incomingCommand);
    }  
}

void sendSingleSerialCommand(char command) {
  mySerial.print(command);
  }

void sendStringSerialCommand(char* command){
  
  }
