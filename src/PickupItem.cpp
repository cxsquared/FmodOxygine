#include "PickupItem.h"

PickupItem::PickupItem()
{
	_canPickup = true;
	_name = "item";
}

PickupItem PickupItem::pickUp()
{
	return *this;
}

string PickupItem::Interact(Level & level)
{
	return "Looks like an item you can pick up";
}
