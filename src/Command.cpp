#include "Command.h"
#include "InvalidCommand.h"
#include "Mail.h"

Command* Command::invalid = new InvalidCommand();
Command* Command::mail = new Mail();
