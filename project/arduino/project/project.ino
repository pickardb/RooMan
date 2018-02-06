int buttonPin = 4;
int ledPin = 5;

// Variables will change:
int request = 0;   
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
char solvedCommand = 'a';
char incomingCommand = NULL;
void setup() {
  pinMode(buttonPin, INPUT);
  // initialize the LED as an output:
  pinMode(ledPin, OUTPUT);
  // initialize serial communication:
  Serial.begin(9600);

}

void loop() {
   buttonState = digitalRead(buttonPin);
   //Serial.println(buttonState);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      request = 1;
      Serial.print("Room requested\n");
    } else {
      //falling edge
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;


   if (Serial.available() > 0) {
    incomingCommand = Serial.read();
    if(incomingCommand = solvedCommand) {
      if(!request){
        Serial.print("No Room Request\n");
        }
       else{
          request = 0;
          Serial.print("Room request solved\n");
        }
      }
   }
  if (request) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

}


