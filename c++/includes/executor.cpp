#ifndef __CLI_MENU__EXECUTOR_CPP__
#define __CLI_MENU__EXECUTOR_CPP__

#include "executor.h"

namespace cli_menu {

  Program *Executor::program = nullptr;
  mt::VEC_STR Executor::inputs = {};
  mt::VEC_UI Executor::usedIndexes = {};

  void Executor::create(
    Program *program_in,
    mt::CR_INT argc,
    char *argv[]
  ) {
    if (program_in) {
      program = program_in;

      for (int i = 1; i < argc; i++) {
        inputs.push_back(argv[i]);
      }

      if (inputs.size() <= 1) {
        Message::printProgramError(program, true);
      }
    }
    else Message::printDevError("Executor::create", "Program");
  }

  void Executor::end() {
    if (program) {
      program->remove();
      inputs = {};
    }
    else Message::printDevError("Executor::end", "Program");
  }

  void Executor::match(
    Command *command,
    Command *ultimateHook
  ) {
    for (int i = 0; i < command->getNumberOfItems(); i++) {
      if (command->getItem(i)->match(inputs)) {

        if (command->getItem(i)->isUltimate()) {
          ultimateHook = command->getItem(i);
        }

        usedIndexes.push_back(i);
        match(command->getItem(i), ultimateHook);
      }
    }
  }

  void Executor::execute() {
    if (program) {
      Command *ultimate;
      ParamData paramData;

      match(program, ultimate);
      int tierEnd = usedIndexes.size() - 1;

      if (ultimate) {
        if (ultimate->getTier() <= tierEnd) {
          program->pullData(paramData, usedIndexes);
          ultimate->run(paramData);
        }
        else if (tierEnd >= 0) {
          // open dialog..
        }
        else Message::printProgramError(program, false);
      }
      else Message::printDevError("Executor::execute", "Command::ultimate");
    }
    else Message::printDevError("Executor::execute", "Program");
  }
}

#endif // __CLI_MENU__EXECUTOR_CPP__