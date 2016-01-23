#pragma once
#include "Player.h"
#include <vector>

class Room;
class GameState;

class Level {
public:
	Level(GameState *state);
	Level(int minRooms, int maxRooms, int minFloors, int maxFloors);
	~Level();

	Player* player;
    GameState *state;
private:
	vector<Room*> rooms;
	Room* generateLevel(int roomsPerFloor, int floors);
	Room* makeRoom(Room& connectingRoom, string direction, int floor);
	string getRandomDirection(Room connectionRoom);
};
