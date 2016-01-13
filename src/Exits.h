#pragma once
#include "Command.h"

class Exits : public Command {
	// Inherited via Command
	virtual string parseNoun(vector<string> options, ScreenState * state) override;
};