#pragma once
#include <string>
#include "Timer.h"
#include "oxygine-framework.h"

using namespace oxygine;
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
	void onEnter();
	void update(const UpdateState &us);
	string hit(int damage);
	int getHealth();
private:
	Level& _level;
	int _health;
    Timer *_attackTimer;
    void onAttackTimer(Timer &t);
};