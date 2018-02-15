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


//for the heart rate
int heartRateButtonState = 0;
int lastHeartButtonState = 0;
int firstPulse = 0;
int secondPulse = 0;
int sleep = 0;
int pulseStart = 0;
int heartRate = -1;
int heartRateState = 1;
SoftwareSerial mySerial(RxPin, TxPin); // RX, TX


void loop() {
  // check the button
  /*if(tempCounter<1000){
    tempCounter++;
    delay(10);
    }
    else{
    Serial.println(" ");
    tempCounter=0;
    Temp();
    }*/
  
  //Serial.println("Start of loop");
  checkRequest();
  checkLight();
  //checkHeartRateRequest();
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

    /*case requestLightCommand:
      Serial.println("Update Light Status");
      if(lightStatus) {
        sendSingleSerialCommand('1');
        }
      else{
        sendSingleSerialCommand('0');
        }*/
      break;
      
    default:
      Serial.print("Unknow command: ");
      Serial.println(incomingCommand);
  }
}

void sendSingleSerialCommand(char command) {
  Serial.print("Sending Command:");
  Serial.println(command);
  mySerial.print(command);
  //mySerial.print('\0');
}


void Temp (void) {
  int chk = DHT.read11(DHTpin);
  int temp;
  int tens;
  int ones;
  switch (chk)
  {
    case DHTLIB_OK:
      temp = DHT.temperature;
      tens = temp / 10;
      ones = temp % 10;
      Serial.print("temp: ");
      Serial.print(temp);
      Serial.print(", tens: ");
      Serial.print(tens);
      Serial.print(", ones: ");
      Serial.println(ones);
      sendSingleSerialCommand(tempMessage);
      delay(100);
      sendSingleSerialCommand(tens + 48);
      delay(100);
      sendSingleSerialCommand(ones + 48);
      break;
  }
}

/*void checkHeartRateRequest(void) {
  //Serial.println("Test 1 ==========");
  heartRateButtonState = digitalRead(heartRateButton);
  if (heartRateButtonState != lastHeartButtonState) {
    // if the state has changed, start the heart rate recording
    if (heartRateButtonState == HIGH) {
      while (heartRateState == 1) {
        Serial.println("Heart rate reading is started\n");
        heartRate = analogRead(pulse);
        Serial.println(heartRate);
        if (heartRate == 0 || heartRate < 10) {
          pulseStart = 1;
          firstPulse = millis();
        }

        if (heartRate < 400 && pulseStart == 1) {
          secondPulse = millis();
          sleep = secondPulse - firstPulse;
          if (sleep > 2000) {
            tone(buzzerPin, 1000, 1000);
            heartRateState = 0;
          }
         else if (heartRate > 900 && pulseStart == 1 && sleep>2000) {
          tone(buzzerPin, 1000, 1000);
          heartRateState = 0;
        }
        if(heartRateState == 0)
        break;
      }
    }
    }
    else {
      //falling edge
        heartRateState = 0;
        pulseStart = 0;
    }
    
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastHeartButtonState = heartRateButtonState;
  
}*/

void doorUnlockTone(void) {
  tone(buzzerPin, 1000, 1500);
}
