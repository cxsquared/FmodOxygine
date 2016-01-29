#pragma once
#include "Room.h"
#include <memory>

using namespace std;

class Enemy;
class PickupItem;

class Player {
public:
	Player(Room& startingRoom);
	~Player();
	Room* currentRoom;
	int health;

	bool move(string dir);

	string attack(Enemy& enemy);
	string hit(int damage);
private:
	// TODO: Implement an inventory system
	vector<shared_ptr<PickupItem>> inventory;

};
