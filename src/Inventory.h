#pragma once
#include <vector>
#include <memory>
#include <string>

using namespace std;

class PickupItem;

class Inventory {
public:
	bool hasItem(string itemName);
	shared_ptr<PickupItem> getItem(string itemName);
	void storeItem(shared_ptr<PickupItem> item);
private:
	vector<shared_ptr<PickupItem>> _items;

};
