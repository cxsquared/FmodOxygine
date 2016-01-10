#pragma once
#include "Player.h"
#include <vector>

class Room;

class Level {
public:
	Level();
	Level(int minRooms, int maxRooms, int minFloors, int maxFloors);
	~Level();

	Player* player;
private:
	vector<Room*> rooms;
	Room generateLevel(int roomsPerFloor, int floors);
	Room* makeRoom(Room connectingRoom, string direction, int floor);
	string getRandomDirection(Room connectionRoom);
};
