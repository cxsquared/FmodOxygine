#include "Room.h"
#include <stdlib.h>

Room::Room(int floor)
{
	_floor = floor;
	generateDescritption();
}

string Room::enteringRoom()
{
	return description + " " +  getExits();
}

string Room::getExits()
{
	string exitDesc = "There's a door to the ";
	//Check all exits if they exists
	auto w = exits.find("n");
	if (w != exits.end()) {
		exitDesc += "north ";
	}
	w = exits.find("s");
	if (w != exits.end()) {
		exitDesc += "south ";
	}
	w = exits.find("e");
	if (w != exits.end()) {
		exitDesc += "east ";
	}
	w = exits.find("w");
	if (w != exits.end()) {
		exitDesc += "west ";
	}
	w = exits.find("up");
	if (w != exits.end()) {
		exitDesc += "and staris up ";
	}
	w = exits.find("down");
	if (w != exits.end()) {
		exitDesc += "and stairs down ";
	}

	return exitDesc;
}

int Room::getFloor()
{
	return _floor;
}

void Room::generateDescritption()
{
	int randDesc = rand() % 4;
	if (randDesc == 0) {
		description = "A normal looking room.";
	}
	else if (randDesc == 1) {
		description = "A cool looking room.";
	}
	else if (randDesc == 2) {
		description = "A boring looking room.";
	}
	else if (randDesc == 3) {
		description = "A room with butterflies everywhere.";
	}
}
