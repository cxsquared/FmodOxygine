#include "MainMenuState.h"
#include "Command.h"
#include "ScreenActor.h"
#include "Login.h"

void MainMenuState::enter(ScreenActor & screen)
{
	ScreenState::enter(screen);

	knowCommands["mail"] = Command::mail;
	knowCommands["Mail"] = Command::mail;
    knowCommands["Game"] = Command::mail;
    knowCommands["game"] = Command::mail;
    knowCommands["start"] = Command::mail;
    knowCommands["Start"] = Command::mail;
    knowCommands["Logout"] = Command::logout;
    knowCommands["logout"] = Command::logout;
    
    Login* login = dynamic_cast<Login*>(Command::login);
    screen.addText("Welcome " + login->currentUser);
    screen.addText("Type *game* to start playing");
}
