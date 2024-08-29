#ifndef __CLI_MENU__EXECUTOR_CPP__
#define __CLI_MENU__EXECUTOR_CPP__

#include "executor.h"

namespace cli_menu {

  Program *Executor::program = nullptr;
  mt::VEC_STR Executor::inputs = {};
  mt::VEC_UI Executor::usedIndexes = {};
  bool Executor::dialogComplete = true;

  bool Executor::selectFunction(Command *command) {
    if (command) {
      if (dialogComplete) {
        DIALOG();
        return true;
      }
      else if (command->hasCallback()) {
        command->run();
        return true;
      }
    }
    return false;
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

  void Executor::run(
    Program *program_in,
    mt::CR_INT argc,
    char *argv[],
    mt::CR_BOL completingDialog
  ) {
    if (program_in) {
      /** Init */

      ParamData paramData;
      program = program_in;
      dialogComplete = completingDialog;

      Command *lastCom = nullptr,
        *ultimate = nullptr;

      for (int i = 0; i < argc; i++) {
        inputs.push_back(argv[i]);
      }

      /** Process */

      if (inputs.size() > 1) {
        match(program, &lastCom, &ultimate);
      }

      int tierEnd = usedIndexes.size();

      if (ultimate) {
        if (ultimate->getTier() <= tierEnd) {
          program->pullData(paramData, usedIndexes);
          ultimate->run(paramData);
        }
        // 'lastCom' is definitely exist
        else if (!selectFunction(lastCom)) {
          Message::printCommandError(lastCom);
        }
      }
      else if (!selectFunction(lastCom)) {
        if (lastCom) {
          Message::printCommandError(lastCom);
        }
        else if (!selectFunction(program)) {
          Message::printProgramError(program, inputs.size() <= 1);
        }
      }

      // runtime end
      program->remove();
    }
    else Message::printDevError("Executor::create", "Program");
  }
}

#endif // __CLI_MENU__EXECUTOR_CPP__