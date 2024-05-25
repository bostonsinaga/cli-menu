#ifndef __CLI_MENU__EXECUTOR_H__
#define __CLI_MENU__EXECUTOR_H__

#include <vector>
#include "util.h"
#include "command.h"

namespace cli_menu {

  class Executor {
  private:
    Command *singleCommand = nullptr;
    std::vector<Command*> *commands = nullptr;
    std::string programName;

  public:
    Executor(std::string programName_in, Command *command_in) {
      programName = programName_in;
      singleCommand = command_in;
    }

    Executor(std::string programName_in, std::vector<Command*> commands_in) {
      programName = programName_in;
      commands = &commands_in;
    }

    void iterate(int lineCount, char *argv[] lineArgs) {

      // useless 'lineCount'
      if (lineCount <= 0) {
        std::cerr << 
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
        for (const auto &comm : commands) {
          for (int i = 0; i < lineCount; i++) {

            
          }
        }
      }
      else if (singleCommand) {

      }
    }
  };
}

#endif // __CLI_MENU__EXECUTOR_H__