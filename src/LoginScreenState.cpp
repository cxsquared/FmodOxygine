//
//  LoginScreenState.cpp
//  FmodTest_macosx
//
//  Created by Cody Claborn on 1/18/16.
//  Copyright © 2016 oxygine. All rights reserved.
//

#include "LoginScreenState.h"
#include "ScreenActor.h"
#include "Command.h"

void LoginScreenState::enter(ScreenActor &screen) {
    ScreenState::enter(screen);
    
    knowCommands["login"] = Command::login;
    knowCommands["Login"] = Command::login;
    
    screen.addText("Welcome to Hightower Industries OS...");
    screen.addText("Your number one leader in future technlogy.");
    screen.addText("Please login...");
}
