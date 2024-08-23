#ifndef __CLI_MENU__EXECUTOR_CPP__
#define __CLI_MENU__EXECUTOR_CPP__

#include "executor.h"

namespace cli_menu {

  Executor::create(
    mt::CR_INT argc, char *argv[]
  ) {
    if (Command::program) {
      for (int i = 1; i < argc; i++) {
        inputs.push_back(argv[i]);
      }

      if (inputs <= 1) {
        Message::printProgramError(true);
      }
    }
    else abort("Executor::create");
  }

  Executor::end() {
    if (Command::program) {
      Command::program->remove();
      inputs = {};
    }
    else abort("Executor::end");
  }

  void Executor::abort(mt::CR_STR funName) {
    assertm(Command::program, "not specified [" + funName + "]");
  }

  void Executor::match(
    Command *command,
    Command *ultimateHook
  ) {
    for (int i = 0; i < command->items->size(); i++) {
      if (command->items[i]->match(inputs)) {

        if (command->items[i]->isUltimate()) {
          ultimateHook = command->items[i];
        }

        usedIndexes.push_back(i);
        match(command->items[i], inputs, usedIndexes);
      }
    }
  }

  void Executor::execute() {
    if (Command::program) {
      Command *ultimate;

      mt::CR_VEC_STR TEXTS;
      mt::CR_VEC_DBL NUMBERS;
      mt::CR_VEC_BOL CONDITIONS;

      match(Command::program, ultimate);
      int tierEnd = usedIndexes.size() - 1;

      if (ultimate) {
        if (ultimate->getTier() <= tierEnd) {

          Command::program->pullData(
            TEXTS, NUMBERS, CONDITIONS, usedIndexes
          );

          (*ultimate->callback)(TEXTS, NUMBERS, CONDITIONS);
        }
        else if (tierEnd >= 0) {
          // open dialog..
        }
        else Message::printProgramError(false);
      }
      else assertm(ultimate, "not specified");
    }
    else abort("Executor::execute");
  }
}

#endif // __CLI_MENU__EXECUTOR_CPP__