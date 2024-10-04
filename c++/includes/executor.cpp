#ifndef __CLI_MENU__EXECUTOR_CPP__
#define __CLI_MENU__EXECUTOR_CPP__

#include "executor.h"

namespace cli_menu {

  Program *Executor::program = nullptr;
  mt::VEC_STR Executor::inputs = {};
  mt::VEC_UI Executor::usedIndexes = {};
  bool Executor::dialogComplete = true;

  bool Executor::resolveFunction(Command *command) {
    if (command) {
      if (dialogComplete) {
        mt::USI flag = command->dialog();

        if (flag == Command::DIALOG::COMPLETE) {
          return pullDataRun(command);
        }
        else { // CANCELED
          Message::print(
            Message::STATUS::CANCELED,
            "Program terminated.",
            program->getName()
          );
          return true;
        }
      }
    }
    return false;
  }

  bool Executor::pullDataRun(Command *command) {
    ParamData paramData;
    program->pullData(paramData, usedIndexes);

    if (command->run(paramData)) {
      Message::print(
        Message::STATUS::SUCCEED,
        "Output file written to 'foo.kml'.",
        program->getName()
      );
      return true;
    }
    else {
      Message::print(
        Message::STATUS::ERROR,
        "Problem with input.",
        program->getName()
      );
      return false;
    }
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
      //______|
      // Init |
      //______|

      program = program_in;
      dialogComplete = completingDialog;

      Command *lastCom = nullptr,
        *ultimate = nullptr;

      for (int i = 0; i < argc; i++) {
        inputs.push_back(argv[i]);
      }

      if (Command::usingLowercaseName) {
        program->changeTreeNamesToLowercase();
      }
      else if (Command::usingUppercaseName) {
        program->changeTreeNamesToUppercase();
      }

      //_________|
      // Process |
      //_________|

      if (inputs.size() > 1) {
        match(program, &lastCom, &ultimate);
      }

      int levelEnd = usedIndexes.size();

      if (ultimate) {
        if (ultimate->getLevel() <= levelEnd) {
          pullDataRun(ultimate);
        }
        // 'lastCom' is a group
        else if (!resolveFunction(lastCom)) {
          lastCom->printError();
        }
      }
      else if (!resolveFunction(lastCom)) {
        if (lastCom) {
          lastCom->printError();
        }
        else if (!resolveFunction(program)) {
          program->printError();
        }
      }

      // runtime end
      program->remove();
    }
    else Message::print(
      Message::STATUS::ERROR,
      "Unassigned 'Program'.",
      "cli_menu::Executor::run",
      false
    );
  }
}

#endif // __CLI_MENU__EXECUTOR_CPP__