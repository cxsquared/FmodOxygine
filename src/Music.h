#pragma once
#include "Command.h"
#include "SoundManager.h"

class Music : public Command {
public:
	// Inherited via Command
	virtual string parseNoun(vector<string> options, ScreenState * state) override;
};