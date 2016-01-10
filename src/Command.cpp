#include "Command.h"
#include "InvalidCommand.h"
#include "Mail.h"
#include "Music.h"
#include "Look.h"
#include "Go.h"
#include "Clear.h"
#include "Quit.h"

Command* Command::invalid = new InvalidCommand();
Command* Command::mail = new Mail();
Command* Command::music = new Music();
Command* Command::look = new Look();
Command* Command::go = new Go();
Command* Command::clear = new Clear();
Command* Command::quit = new Quit();