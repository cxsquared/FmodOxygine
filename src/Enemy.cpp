#include "Enemy.h"
#include "Level.h"
#include <iostream>
#include "core/log.h"
#include "ScreenState.h"
#include "GameState.h"
#include "ScreenActor.h"

using namespace std;


Enemy::Enemy(Level& level)
	:_level(level)
{
	//TODO: Figure out a better way for health and what not
	_health = 10;
	isAlive = true;

	cout << "Enemy created with " << _health << " health" << endl;
    
    _attackTimer = Timer();
    _attackTimer.start((rand() % 6)*.5, 1, [this](Timer t) {return this->onAttackTimer(t);});
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
    _attackTimer.update();
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

void Enemy::onAttackTimer(Timer t) {
    ScreenState& screen = dynamic_cast<ScreenState&>(*_level.state);
    screen.getScreen().addText(this->attack());
    t.start((rand() % 6)*.5, 1, [this](Timer t) {return this->onAttackTimer(t);});
    oxygine::log::messageln ("Timer callback called");
}
