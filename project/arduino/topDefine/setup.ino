void setup() {
  
  // initialize serial communication:
  Serial.begin(9600);
  while(!mySerial){}
  
  mySerial.begin(9600);
  while(!mySerial){}

  pinMode(uSonicTrig, OUTPUT);
  pinMode(uSonicEcho, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

}
