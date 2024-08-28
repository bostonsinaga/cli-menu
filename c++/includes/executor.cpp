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

      for (int i = 0; i < argc; i++) {
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
    Command **lastCom,
    Command **ultimateHook
  ) {
    for (int i = 0; i < command->getNumberOfItems(); i++) {
      Command *com = command->getItem(i);

      if (com->match(inputs)) {

        if (com->isUltimate()) *ultimateHook = com;
        *lastCom = com;

        usedIndexes.push_back(i);
        match(com, lastCom, ultimateHook);
      }
    }
  }

  void Executor::execute() {
    if (program) {
      ParamData paramData;

      Command *lastCom = nullptr,
        *ultimate = nullptr;

      if (inputs.size() > 1) {
        match(program, &lastCom, &ultimate);
      }

      int tierEnd = usedIndexes.size();

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
      else if (lastCom) lastCom->run();
      else program->run();
    }
    else Message::printDevError("Executor::execute", "Program");
  }
}

#endif // __CLI_MENU__EXECUTOR_CPP__