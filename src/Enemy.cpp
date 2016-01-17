#include "Enemy.h"
#include "Level.h"
#include <iostream>

using namespace std;

Enemy::Enemy(Level& level)
	:_level(level)
{
	//TODO: Figure out a better way for health and what not
	//TODO: Figure out why health is being set so weird
	_health = 10;
	isAlive = false;

	cout << "Enemy created with " << _health << " health" << endl;
}

Enemy::~Enemy()
{
}

string Enemy::getName()
{
	// TODO: Generate names
	return "Bob";
}

string Enemy::getDescription()
{
	// TODO: Generate better descriptions
	return getName() + " is looking scarly at you. They have " + to_string(getHealth()) + " hp left";
}

string Enemy::attack()
{
	return _level.player->hit(2);
}

void Enemy::update()
{
}

string Enemy::hit(int damage)
{
	if (isAlive) {
		// TODO: Add cover and armor values
		_health -= damage;
		if (_health <= 0) {
			isAlive = false;
			return "You've killed " + getName();
		}

		return getName() + " took " + to_string(damage) + " points of damage.";
	}

	return getName() + " is already dead!";
}

int Enemy::getHealth()
{
	return _health;
}
