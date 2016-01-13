#pragma once
#include "Command.h"

class North : public Command {
	// Inherited via Command
	virtual string parseNoun(vector<string> options, ScreenState * state) override;
};