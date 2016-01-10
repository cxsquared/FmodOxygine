#include "Player.h"
#include "Room.h"

Player::Player(Room& startingRoom)
{
	currentRoom = &startingRoom;
}

Player::~Player()
{
}

bool Player::move(string dir)
{
	//TODO: allow players to type in full words instead of one letter
	// Check room to see if you can go that direction
	auto w = currentRoom->exits.find(dir);
	if (w != currentRoom->exits.end()) {
		currentRoom = currentRoom->exits[dir];
		return true;
	}
	// return false if could move
	return false;
}
