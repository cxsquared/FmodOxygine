#include "Level.h"
#include <vector>
#include "Room.h"
#include <algorithm>
#include <stdlib.h>

using namespace std;

Level::Level() {
	player = new Player(*generateLevel(12, 6));
}

Level::Level(int minRooms, int maxRooms, int minFloors, int maxFloors) {
	
}

Level::~Level()
{
	// Deleting all rooms in vector
	vector<Room*>::iterator it;
	for (it = rooms.begin(); it != rooms.end();) {
		delete *it;
		it = rooms.erase(it);
	}

	delete player;
}

Room* Level::generateLevel(int roomsPerFloor, int floors)
{
	int currentFloor = 0;
	Room* exitRoom;
	Room* startingRoom;

	while (currentFloor < floors) {
		int numberOfRooms = 0;
		// make entrance room
		if (currentFloor == 0) {
			Room* room = new Room(currentFloor);
			startingRoom = room;
			rooms.push_back(room);
		} else {
			rooms.push_back(makeRoom(*exitRoom, "up", currentFloor));
		}
		numberOfRooms++;
		while (numberOfRooms < roomsPerFloor) {
			random_shuffle(rooms.begin(), rooms.end());
			for (vector<Room*>::iterator it = rooms.begin(); it != rooms.end(); ++it) {
				if ((*it)->getFloor() == currentFloor && (*it)->exits.size() < 4) {
					rooms.push_back(makeRoom(*(*it), getRandomDirection(*(*it)), currentFloor));
					numberOfRooms++;
					break;
				}
			}
		}
		// make exit
		random_shuffle(rooms.begin(), rooms.end());
		for (vector<Room*>::iterator it = rooms.begin(); it != rooms.end(); ++it) {
			if ((*it)->getFloor()== currentFloor) {
				auto w = (*it)->exits.find("down");
				if (w == (*it)->exits.end()) {
					exitRoom = (*it);
					break;
				}
			}
		}
		currentFloor++;
	}

	return startingRoom;
}

Room* Level::makeRoom(Room& connectingRoom, string direction, int floor)
{
	Room* room = new Room(floor);
	if (direction == "n") {
		room->exits["s"] = &connectingRoom;
	}
	else if (direction == "s") {
		room->exits["n"] = &connectingRoom;
	}
	else if (direction == "w") {
		room->exits["e"] = &connectingRoom;
	}
	else if (direction == "e") {
		room->exits["w"] = &connectingRoom;
	}
	else if (direction == "up") {
		room->exits["down"] = &connectingRoom;
	}
	else if (direction == "down") {
		room->exits["up"] = &connectingRoom;
	}
	room->generateEnemies(*this);
	connectingRoom.exits[direction] = room;
	return room;
}

string Level::getRandomDirection(Room connectionRoom)
{
	int tries = 0;
	bool exitFound = false;
	string exit = "";
	while (!exitFound && tries < 16) {
		int randDir = rand() % 4; // Getting a number 0 to 4;
		if (randDir == 0) {
			// Check North
			auto w = connectionRoom.exits.find("n");
			if (w == connectionRoom.exits.end()) {
				exit = "n";
				exitFound = true;
			}
		}
		else if (randDir == 1) {
			// Check South
			auto w = connectionRoom.exits.find("s");
			if (w == connectionRoom.exits.end()) {
				exit = "s";
				exitFound = true;
			}
		}
		else if (randDir == 2) {
			// Check West
			auto w = connectionRoom.exits.find("w");
			if (w == connectionRoom.exits.end()) {
				exit = "w";
				exitFound = true;
			}
		}
		else if (randDir == 3) {
			// Check East
			auto w = connectionRoom.exits.find("e");
			if (w == connectionRoom.exits.end()) {
				exit = "e";
				exitFound = true;
			}
		}

		tries++;
	}
	return exit;
}

