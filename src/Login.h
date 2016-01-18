#pragma once
#include "Command.h"
#include <map>
#include <string>

using namespace std;

class Login : public Command {
public:
	// Inherited via Command
	virtual string parseNoun(vector<string> options, ScreenState * state) override;
    string currentUser = "";
	
private:
	string _username = "";
	bool _newUser = false;
	bool checkLogin(string password);
	bool isUser(string username);
	map<string, string> _loginData;
};