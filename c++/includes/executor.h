#ifndef __CLI_MENU__EXECUTOR_H__
#define __CLI_MENU__EXECUTOR_H__

#include "message.h"

namespace cli_menu {

  class Executor {
  private:
    static Program *program;
    static mt::VEC_STR inputs;
    static mt::VEC_UI usedIndexes;
    static bool dialogComplete;

    static bool resolveFunction(Command *command);
    static bool pullDataRun(Command *command);

    static void match(
      Command *command,
      Command **lastCom,
      Command **ultimateHook
    );
  
  public:
    static void run(
      Program *program_in,
      mt::CR_INT argc,
      char *argv[],
      mt::CR_BOL completingDialog
    );
  };
}

#endif // __CLI_MENU__EXECUTOR_H__