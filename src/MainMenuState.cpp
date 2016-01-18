#include "MainMenuState.h"
#include "Command.h"
#include "ScreenActor.h"

void MainMenuState::enter(ScreenActor & screen)
{
	ScreenState::enter(screen);

	knowCommands["mail"] = Command::mail;
	knowCommands["Mail"] = Command::mail;
    knowCommands["Game"] = Command::mail;
    knowCommands["game"] = Command::mail;
    knowCommands["start"] = Command::mail;
    knowCommands["Start"] = Command::mail;
    
    // TODO : replace user with actual name
    screen.addText("Welcome user");
    screen.addText("Type game to start playing");
}
