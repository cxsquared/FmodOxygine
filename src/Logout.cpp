#include "Logout.h"
#include "ScreenState.h"
#include "Login.h"

string Logout::parseNoun(vector<string> options, ScreenState * state)
{
    state->switchStates(ScreenState::loginState);
    
    Login* login = dynamic_cast<Login*>(Command::login);
    login->currentUser = "";
    
    return "Loging out";
}

