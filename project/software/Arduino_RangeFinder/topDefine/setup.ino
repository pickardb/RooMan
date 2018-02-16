void setup() {
  
  // initialize serial communication:
  Serial.begin(9600);
  while(!Serial){}

  //init bluetooth serial connection 
  mySerial.begin(115200);
  while(!mySerial){
    }

  pinMode(uSonicEcho, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(lightButton,INPUT);
 
  pinMode(uSonicTrig, OUTPUT);
  pinMode(ledPin, OUTPUT);

  door.attach(servoPin);
  pinMode(buzzerPin,OUTPUT);
  Serial.println("Setup complete!");
}
