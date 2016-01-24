#pragma once
#include <map>
#include <string>
#include <vector>
#include <memory>
#include "oxygine-framework.h"

using namespace oxygine;
using namespace std;

class Enemy;
class Level;

class Room {
public:
	Room(int floor=0);
	~Room();
    map<string, Room*> exits;
	vector< shared_ptr<Enemy> > enemies;
	string enteringRoom();
	string getExits();
	int getFloor();
    string getShortDescription();
    string getDescritption();
    virtual void update(const UpdateState &us);
	void generateEnemies(Level& level);

private:
    int _roomDescription;
	int _floor;
};
