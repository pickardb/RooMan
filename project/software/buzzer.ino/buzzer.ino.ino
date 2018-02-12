int buzzer = 3; // buzzer pin 

void setup() {
  // put your setup code here, to run once:
  pinMode(buzzer, OUTPUT);  
  Serial.begin(115200);     
}

void loop() {
  // put your main code here, to run repeatedly:
  tone(buzzer, 1000); // buzz with a frequency 1000
  //tone(buzzer, 500, 1000); // buzz for one sec with a frequency 500 (duration is optional)
  noTone(buzzer); // to stop the buzzer in case the duration is not specified  
}
