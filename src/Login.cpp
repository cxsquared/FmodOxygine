#include "Login.h"
#include "ScreenState.h"

string Login::parseNoun(vector<string> options, ScreenState * state)
{
	// TODO: Store login data
	if (state->isWaiting()) {
		if (options.size() > 0) {
			if (_username != "") {
				// user has given a name
				if (checkLogin(options[0])) {
					// password checks out. User is logged in
					state->clearLastCommand();
					string tempUser = _username;
					_username = "";
                    state->switchStates(ScreenState::mainMenu);
                    currentUser = tempUser;
					return "Welcome " + tempUser;
				}
				else {
					// password is wrong
					state->clearLastCommand();
					_username = "";
					return "Password is wrong. Please try to login again.";
				}
			}
			else if (isUser(options[0])) {
				// user exists
				_username = options[0];
				_newUser = false;
				return "Please input your password...";
			}
			else {
				// user doesn't exist
				_username = options[0];
				_newUser = true;
				return "Welcome new user " + options[0] + ". Please enter your new password...";
			}
		}

		state->clearLastCommand();
		_username = "";
		return "Error.. please try to login again.";
    }
    
    if (options.size() > 0) {
        if (isUser(options[0])) {
            _username = options[0];
            _newUser = false;
            state->setLastCommand(this);
            return "Please input your password...";
        } else {
            // user doesn't exist
            _username = options[0];
            _newUser = true;
            state->setLastCommand(this);
            return "Welcome new user " + options[0] + ". Please enter your new password...";
        }
    }

	// First time asking to login in
	state->setLastCommand(this);
	return "Input your user name...";
}

bool Login::checkLogin(string password)
{
	if (_username != "") {
		if (_newUser) {
			_loginData[_username] = password;
			return true;
		}
		auto user = _loginData.find(_username);
		if (user != _loginData.end()) {
			// user is reall
			if (password == user->second) {
				// password checks out
				return true;
			}
		}
	}

	return false;
}

bool Login::isUser(string username)
{
	auto user = _loginData.find(username);
	if (user == _loginData.end()) {
		// user doesn't exist
		return false;
	}

	// user exists
	return true;
}
