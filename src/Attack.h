#pragma once
#include "Command.h"

class Attack : public Command {
	// Inherited via Command
	virtual string parseNoun(vector<string> options, ScreenState * state) override;
};
