#ifndef __CLI_MENU__EXECUTOR_CPP__
#define __CLI_MENU__EXECUTOR_CPP__

#include "executor.h"

namespace cli_menu {

  Executor::Executor(std::string programName_in, Command *command_in) {
    programName = programName_in;
    singleCommand = command_in;
  }

  Executor::Executor(std::string programName_in, std::vector<Command*> commands_in) {
    programName = programName_in;
    commands = &commands_in;
  }

  void Executor::iterate(int lineCount, char* lineArgs[]) {

    // useless 'lineCount'
    if (lineCount <= 0) {
      Message::printError("the 'lineCount' is not specified");
      return;
    }

    // the 'lineCount' is wrong because it exceeds the actual number of 'lineArgs'
    try { lineArgs[lineCount - 1]; }
    catch (...) { return; }

    /**
      * If 'lineCount' is less than 'lineArgs' real count,
      * the rest of 'lineArgs' will be ignored.
      */

    std::vector<std::string> lineStrings;

    for (int i = 0; i < lineCount; i++) {
      lineStrings.push_back(lineArgs[i]);
    }

    // ...

    if (commands) {
      for (auto &comm : *commands) {
        for (int i = 0; i < lineCount; i++) {

          
        }
      }
    }
    else if (singleCommand) {

    }
  }
}

#endif // __CLI_MENU__EXECUTOR_CPP__