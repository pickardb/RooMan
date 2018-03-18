/**
 * Simple struct for the room object. Note that all members
 * are used as a boolean besides temp, which may be any int.
 *
 * temp			Temperature reading
 * lights 		Indicates whether room is on or off
 * door 		Indicates whether room is locked or unlocked
 * requested 	Indicates whether room has a pending request
 * in_use 		Indicates whether the room has been claimed by somebody or not
 * occupied 	Indicates whether someone is present in the room or not
 */
struct room {
	int temp;
	int lights;
	int door;
	int requested;
	int in_use;
	int occupied;
};

struct room roomArray[10];


