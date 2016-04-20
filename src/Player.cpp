#include "Player.h"
#include "Room.h"
#include "Enemy.h"
#include "core/log.h"
#include "SoundManager.h"

Player::Player(Room& startingRoom)
{
	currentRoom = &startingRoom;
	health = 10;
}

Player::~Player()
{
}

bool Player::move(string dir)
{
	// Check room to see if you can go that direction
	if (dir == "south" || dir == "South") {
		dir = "s";
	}
	else if (dir == "north" || dir == "North") {
		dir = "n";
	}
	else if (dir == "east" || dir == "East") {
		dir = "e";
	}
	else if (dir == "west" || dir == "West") {
		dir = "w";
	}

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
    SoundManager::playEvent("event:/Sfx/Attack");
	health -= damage;
    
    oxygine::log::messageln("Player attacked");

	if (health <= 0) {
		return "The player has been killed.";
	}

	return "The player has recieved " + to_string(damage) + ".";
}
