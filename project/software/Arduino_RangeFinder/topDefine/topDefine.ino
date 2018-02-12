
#include <Servo.h>
#include <SoftwareSerial.h>
#include <dht.h>

//all the pin

#define buttonPin 3
#define ledPin 2
#define buzzerPin 13

#define servoPin 12

#define uSonicTrig 8
#define uSonicEcho 9

#define DHTpin 4
dht DHT;

// serial pin
#define RxPin 10
#define TxPin 11

#define ROOM_TEMP 22


//input command
#define solvedCommand 'a'
#define updateRoomStatusCommand 'b' //free or occupied
#define lightOnCommand 'c'
#define lightOffCommand 'd'
#define doorOpenCommand 'e'
#define doorCloseCommand 'f'

//output message
#define occupiedMessage '1'
#define freeMessage '2'
#define requestRoomMessage '3'

                                                                                                                                                           

//magic numbers
#define rangerFinderThreshold 30
#define openPos 180
#define closedPos 20






Servo door;
