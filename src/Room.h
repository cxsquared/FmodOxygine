#pragma once
#include <map>
#include <string>

using namespace std;

class Room {
public:
	Room(int floor=0);
	map<string, Room> exits;
	string description = "";
	string enteringRoom();
	string getExits();
	int getFloor();

private:
	int _floor;
	void generateDescritption();
};
