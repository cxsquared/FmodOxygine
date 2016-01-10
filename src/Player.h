#pragma once
#include "Room.h"

class Player {
public:
	Player(Room& startingRoom);
	~Player();
	Room* currentRoom;
	int health;

	bool move(string dir);

	// TODO: Implement an inventory system
};
