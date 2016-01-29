#pragma once
#include "Item.h"

class PickupItem : public Item {
public:
	PickupItem();

	virtual PickupItem pickUp();

	// Inherited via Item
	virtual string Interact(Level & level) override;

};