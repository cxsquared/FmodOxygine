#pragma once
#include "Command.h"

class Mail : public Command {
	// Inherited via Command
	virtual string parseNoun(string noun, ScreenState state) override;
};