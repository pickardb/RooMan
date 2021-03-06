                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           //define IO pins as constants
const int uSonic_Trig = 8;
const int uSonic_Echo = 9;

const int ROOM_TEMP = 22;
#include <Servo.h>
#include <SoftwareSerial.h>


SoftwareSerial mySerial(10, 11); // RX, TX
Servo Servo1;
int pos = 0;
int buttonState = 0;
int lightState = 0;
int lastButton = 0;
int resolved = 1;
int showBox = 0;
char SerialByte;




void setup() {
  //initialize serial communications
  Serial.begin(9600); //TODO: SF change to 115200 baud rate when switching over to bluetooth
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Servo1.attach(12);
  mySerial.begin(9600);
  while(!mySerial){}

  //initialize Ultrasonic Range Finder pins
  pinMode(uSonic_Trig, OUTPUT);
  pinMode(uSonic_Echo, INPUT);
  pinMode(3, INPUT);
  pinMode(2, OUTPUT);

  //initialize temperature sensor
  //pinMode(A0, INPUT);

}
unsigned char dist;
void loop() {
  
  dist = RunRange();
  if (mySerial.available()) {
    SerialByte = mySerial.read();
    Serial.print("Serial received: ");
    Serial.println(SerialByte);
    if (SerialByte == 'b') {
      Serial.print("dist read: ");
      Serial.println(dist);
      if (dist < 30) {
        mySerial.print('1');
        Serial.print("Serial sent: ");
        Serial.println(0x01);
      }
      else {
        mySerial.print('2');
        Serial.print("Serial sent: ");
        Serial.println(0x00);
      }
    }
    else if (SerialByte == 'e' || SerialByte == 'f') {
      TurnServo();
    }
    else if (SerialByte == 'c' || SerialByte == 'd') {
      if (lightState) {
        digitalWrite(2, LOW);
        Serial.println("Lights OFF");
        lightState = 0;
      }
      else {
        digitalWrite(2, HIGH);
        Serial.println("Lights ON");
        lightState = 1;
      }
    }
  }

}

void TurnServo (void) {
  if (pos == 20) {
    pos = 180;
  }
  else {
    pos = 20;
  }
  Servo1.write(pos);
  delay(15);
}



/*
   Returns the distance of an object detected in centimeters.

   Requires: pulseWidth is around 150-25,000. Otherwise an inaccurate output
   may be returned.
   Parameter: pulseWidth length of pulse duration in microseconds.
   Returns: distance of object in centimeters.
*/
unsigned long getDist(unsigned long pulseWidth) {
  unsigned long tempC = ROOM_TEMP;
  return pulseWidth / (soundSpeed(tempC) * 29 / 343 * 2);
}

/*
   Returns the distance of an object detected in inches.

   Requires: pulseWidth is around 150-25,000. Otherwise an inaccurate output
   may be returned.
   Parameter: pulseWidth length of pulse duration in microseconds.
   Returns: distance of object in inches.
*/
unsigned long getDist_INCH(unsigned long pulseWidth) {
  return pulseWidth / (soundSpeed(ROOM_TEMP) * 148 / 343 * 2);
}

/*
   Returns the actual speed of sound.

   Parameter: tempC - temperature in degrees celsius
   Requires: N/A
   Returns: speed of sound in degrees celsius.
*/
unsigned long soundSpeed(unsigned long tempC) {
  return 331.5 + (0.6 * tempC);
}

unsigned int RunRange() {
  float echoPulseWidth;
  unsigned char distance = 0;
  delayMicroseconds(20);

  //send a trigger pulse for ultrasonic range finder to send
  //an ultrasonic pulse
  digitalWrite(uSonic_Trig, HIGH);
  delayMicroseconds(11);
  digitalWrite(uSonic_Trig, LOW);

  //calculate the duration of the echo pulse
  echoPulseWidth = pulseIn(uSonic_Echo, HIGH, 38500);

  //check if echo pulse was within a reasonable range
  //(info sheet states range of echo is about 150us-25ms)
  if (echoPulseWidth > 100 && echoPulseWidth < 26000) {
    distance = getDist(echoPulseWidth);
    //Serial.println(distance);
    //---------Serial.println(" ");

    return (distance);
  }
}
