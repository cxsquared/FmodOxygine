#include "Command.h"
#include "InvalidCommand.h"
#include "Mail.h"
#include "Music.h"

Command* Command::invalid = new InvalidCommand();
Command* Command::mail = new Mail();
Command* Command::music = new Music();
