#pragma once
#include "Command.h"

class InvalidCommand : public Command {
	// Inherited via Command
	virtual string parseNoun(string nouns, ScreenState* state) override;
};