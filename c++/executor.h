#ifndef __CLI_MENU__EXECUTOR_H__
#define __CLI_MENU__EXECUTOR_H__

#include <vector>
#include "message.h"

namespace cli_menu {

  class Executor {
  private:
    Command *singleCommand = nullptr;
    VEC_COM *commands = nullptr;
    std::string programName;

  public:
    Executor(std::string programName_in, Command *command_in);
    Executor(std::string programName_in, std::vector<Command*> commands_in);
    void iterate(int lineCount, char* lineArgs[]);
  };
}

#endif // __CLI_MENU__EXECUTOR_H__