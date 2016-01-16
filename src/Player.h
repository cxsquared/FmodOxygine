#pragma once
#include "Room.h"

class Enemy;

class Player {
public:
	Player(Room& startingRoom);
	~Player();
	Room* currentRoom;
	int health;

	bool move(string dir);

	// TODO: Implement an inventory system

	string attack(Enemy& enemy);
	string hit(int damage);

};
