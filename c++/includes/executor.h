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

    static void match(
      Command *command,
      Command **lastCom,
      Command **ultimateHook
    );
  
  public:
    static void create(
      Program *program_in,
      mt::CR_INT argc,
      char *argv[],
      mt::CR_BOL completingDialog = true
    );

    static void end();
    static void execute();

    static void completeWithDialog(mt::CR_BOL condition) {
      dialogComplete = condition;
    }
  };
}

#endif // __CLI_MENU__EXECUTOR_H__