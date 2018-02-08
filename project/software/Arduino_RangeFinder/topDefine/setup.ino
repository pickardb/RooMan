void setup() {
  
  // initialize serial communication:
  Serial.begin(9600);
  while(!mySerial){}
  
  mySerial.begin(115200);
  while(!mySerial){
    }
    
  Serial.println("Setup complete!");
  
  pinMode(uSonicTrig, OUTPUT);
  pinMode(uSonicEcho, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  door.attach(servoPin);
  pinMode(buzzerPin,OUTPUT);

}
