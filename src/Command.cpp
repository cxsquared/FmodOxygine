#include "Command.h"
#include "InvalidCommand.h"
#include "Mail.h"
#include "Music.h"
#include "Look.h"
#include "Go.h"

Command* Command::invalid = new InvalidCommand();
Command* Command::mail = new Mail();
Command* Command::music = new Music();
Command* Command::look = new Look();
Command* Command::go = new Go();