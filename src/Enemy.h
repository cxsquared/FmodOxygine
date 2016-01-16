#pragma once
#include <string>

using namespace std;

class Player;
class Level;

class Enemy {
public:
	Enemy(Level& level);
	~Enemy();
	bool isAlive;

	string getName();
	string getDescription();
	string attack();
	void update();
	string hit(int damage);
	int getHealth();
private:
	Level& _level;
	int _health;
};