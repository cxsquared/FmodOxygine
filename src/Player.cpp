#include "Player.h"
#include "Room.h"
#include "Enemy.h"
#include "core/log.h"

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

string Player::attack(Enemy & enemy)
{
	// TODO: Implemet a weapon system
	return enemy.hit(2);
}

string Player::hit(int damage)
{
	health -= damage;
    
    oxygine::log::messageln("Player attacked");

	if (health <= 0) {
		return "The player has been killed.";
	}

	return "The player has recieved " + to_string(damage) + ".";
}
