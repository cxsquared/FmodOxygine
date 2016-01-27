#pragma once
#include "Item.h"

class Sign : public Item {
public:
	Sign(string text);
	// Inherited via Item
	virtual string Interact(Level & level) override;

private:
	string _signText;
};