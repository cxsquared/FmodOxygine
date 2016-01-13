#include "Command.h"
#include "InvalidCommand.h"
#include "Mail.h"
#include "Music.h"
#include "Look.h"
#include "Go.h"
#include "Clear.h"
#include "Quit.h"
#include "MainMenuCommand.h"
#include "ResetGame.h"
#include "South.h"
#include "North.h"
#include "East.h"
#include "West.h"
#include "Up.h"
#include "Down.h"
#include "Exits.h"

Command* Command::invalid = new InvalidCommand();
Command* Command::mail = new Mail();
Command* Command::music = new Music();
Command* Command::look = new Look();
Command* Command::go = new Go();
Command* Command::clear = new Clear();
Command* Command::quit = new Quit();
Command* Command::mainMenu = new MainMenuCommand();
Command* Command::reset = new ResetGame();
Command* Command::south = new South();
Command* Command::north = new North();
Command* Command::east = new East();
Command* Command::west = new West();
Command* Command::up = new Up();
Command* Command::down = new Down();
Command* Command::exits = new Exits();