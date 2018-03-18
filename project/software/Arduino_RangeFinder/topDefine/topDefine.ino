#include <Servo.h>
#include <SoftwareSerial.h>
#include <dht.h>

//all the pin definitions
#define pulse 0
#define ledPin 2
#define DHTpin 3
#define buttonPin 4
#define lightButton 5
#define buzzerPin 6
#define uSonicEcho 8
#define uSonicTrig 9
#define servoPin 12 

dht DHT;
// serial pin
#define RxPin 10
#define TxPin 11
#define ROOM_TEMP 22


//input commands
#define solvedCommand 'a'
#define updateRoomStatusCommand 'b' //free or occupied
#define lightOnCommand 'c'
#define lightOffCommand 'd'
#define doorOpenCommand 'e'
#define doorCloseCommand 'f'
#define tempRequestCommand 'h'
#define requestLightCommand 'z'

//output message
#define occupiedMessage '5'
#define freeMessage '2'
#define requestRoomMessage '3'
#define tempMessage 't'
#define lightChangeMessage 'L'
                                                                                                                                                          

//magic numbers
#define rangerFinderThreshold 15
#define openPos 180
#define closedPos 100
Servo door;
