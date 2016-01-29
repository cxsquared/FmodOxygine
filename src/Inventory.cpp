#include "Inventory.h"
#include "PickupItem.h"

bool Inventory::hasItem(string itemName)
{
	for (auto item : _items) {
		if (item->getName() == itemName) {
			return true;
		}
	}

	return false;
}

shared_ptr<PickupItem> Inventory::getItem(string itemName)
{
	for (vector<shared_ptr<PickupItem>>::iterator it = _items.begin(); it != _items.end(); ++it) {
		if ((*it)->getName() == itemName) {
			shared_ptr<PickupItem> tempItem = (*it);
			_items.erase(it);
			return tempItem;
		}
	}

	return NULL;
}

void Inventory::storeItem(shared_ptr<PickupItem> item)
{
	_items.push_back(item);
}
