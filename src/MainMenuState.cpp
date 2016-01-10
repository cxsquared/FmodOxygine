#include "MainMenuState.h"
#include "Command.h"
#include "ScreenActor.h"

void MainMenuState::enter(ScreenActor & screen)
{
	ScreenState::enter(screen);

	knowCommands["mail"] = Command::mail;
	knowCommands["Mail"] = Command::mail;

	screen.addText("Welcome to Hightower Industries OS...");
	screen.addText("Your number one leader in future technlogy.");
	screen.addText("Please login...");
}
