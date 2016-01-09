#pragma once
#include <vector>
#include <string>
#include <sstream>

using namespace std;
class ScreenState;
class InvalidCommand;
class Mail;
class Music;

class Command {
public:
	virtual ~Command() {};
	virtual string parseNoun(vector<string> options, ScreenState* state) = 0;

	// List all commands as static variables
	static Command* invalid;
	static Command* mail;
	static Command* music;
};