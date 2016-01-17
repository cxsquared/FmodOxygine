#pragma once
#include <map>
#include <string>
#include <vector>
#include <memory>

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
    virtual void update();
	void generateEnemies(Level& level);

private:
    int _roomDescription;
	int _floor;
};
