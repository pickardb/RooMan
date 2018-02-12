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
  digitalWrite(uSonicTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(uSonicTrig, LOW);

  //calculate the duration of the echo pulse
  echoPulseWidth = pulseIn(uSonicEcho, HIGH, 38500);

  //check if echo pulse was within a reasonable range
  //(info sheet states range of echo is about 150us-25ms)
  if (echoPulseWidth > 100 && echoPulseWidth < 26000) {
    distance = getDist(echoPulseWidth);
    //Serial.println(distance);
    //---------Serial.println(" ");

    return (distance);
  }
}
