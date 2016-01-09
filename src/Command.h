#pragma once
#include <string>

using namespace std;
class ScreenState;
class InvalidCommand;
class Mail;

class Command {
public:
	virtual ~Command() {};
	virtual string parseNoun(string nouns, ScreenState state) = 0;

	// List all commands as static variables
	static Command* invalid;
	static Command* mail;
};