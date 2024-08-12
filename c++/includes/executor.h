#ifndef __CLI_MENU__EXECUTOR_H__
#define __CLI_MENU__EXECUTOR_H__

#include "message.h"

namespace cli_menu {

  class Executor {
  private:
    VEC_COM commands;
    std::string programName;

    bool testDashSign(std::string &input, int dashCount);
    bool isParameter(std::string &input);
    bool isToggle(std::string &input);

  public:
    Executor(mt::CR_STR programName_in, const VEC_COM &commands_in);
    void iterate(int lineCount, char* lineArgs[]);
  };
}

#endif // __CLI_MENU__EXECUTOR_H__