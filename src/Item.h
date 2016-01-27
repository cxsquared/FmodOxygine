#pragma once
#include <string>

using namespace std;

class Level;

class Item {
public:
	Item();
	~Item();
	virtual string Interact(Level &level) = 0;
	string getName();
	string getDescription();
protected:
	bool _canPickup = false;
	string _name;
	string _description;
};