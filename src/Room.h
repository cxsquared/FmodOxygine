#pragma once
#include <map>
#include <string>

using namespace std;

class Room {
public:
	Room(int floor=0);
    map<string, Room*> exits;
	string enteringRoom();
	string getExits();
	int getFloor();
    string getShortDescription();
    string getDescritption();
    virtual void update();

private:
    int _roomDescription;
	int _floor;
};
