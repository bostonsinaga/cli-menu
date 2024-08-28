#ifndef __CLI_MENU__EXECUTOR_H__
#define __CLI_MENU__EXECUTOR_H__

#include "message.h"

namespace cli_menu {

  class Executor {
  private:
    static Program *program;
    static mt::VEC_STR inputs;
    static mt::VEC_UI usedIndexes;

    static void match(
      Command *command,
      Command **ultimateHook
    );
  
  public:
    static void create(
      Program *program_in,
      mt::CR_INT argc,
      char *argv[]
    );

    static void execute();
    static void end();
  };
}

#endif // __CLI_MENU__EXECUTOR_H__