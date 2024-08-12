#ifndef __CLI_MENU__EXECUTOR_CPP__
#define __CLI_MENU__EXECUTOR_CPP__

#include "executor.h"

namespace cli_menu {

  Executor::Executor(mt::CR_STR programName_in, const VEC_COM &commands_in) {
    programName = programName_in;
    commands = commands_in;
  }

  bool Executor::testDashSign(std::string &input, int dashCount) {
    bool isIt = false, anyNonSpace = false;

    if (input.length() > dashCount) {
      for (int i = 0; i < dashCount; i++) {

        if (!anyNonSpace) {
          if (input[i] == ' ') continue;
          else anyNonSpace = true;
        }

        if (input[i] == '-') isIt = true;
        else {
          isIt = false;
          break;
        }
      }
    }

    return isIt;
  }

  bool Executor::isParameter(std::string &input) {
    return testDashSign(input, 2);
  }

  bool Executor::isToggle(std::string &input) {
    return testDashSign(input, 1);
  }

  /**
   * If 'lineCount' is less than 'lineArgs' actual count,
   * the rest of 'lineArgs' will be ignored.
   */
  void Executor::iterate(int lineCount, char* lineArgs[]) {

    // useless 'lineCount'
    if (lineCount <= 1) {
      Message::print(Message::ERROR, "no argument");
      Message::print(Message::HINT, "type '--help' for available commands");
      return;
    }

    /** Conversion to 'std::string' */

    std::vector<std::string> lineStrings;

    // converting
    for (int i = 1; i < lineCount; i++) {
      lineStrings.push_back(lineArgs[i]);
    }

    lineCount--;

    /** Command selection */

    Command *selCom = nullptr;

    for (Command *com : commands) {
      if (com->getName() == lineStrings[0]) {
        selCom = com;
        break;
      }
    }

    // command not found
    if (!selCom) {

      Message::print(
        Message::ERROR,
        "the command is not recognized",
        programName
      );

      Message::print(
        Message::HINT,
        "please type '--help' for a list of available commands",
        programName
      );

      return;
    }

    /** Get input pattern */

    enum {par_fg, arg_fg, tog_fg};
    bool lineFlags[lineCount];

    for (int i = 0; i < lineCount; i++) {

      bool dashedFirst = lineStrings[i][0] == '-',
        dashedSecond = lineStrings[i][1] == '-';

      if (dashedFirst && dashedSecond) {
        lineFlags[i] = par_fg;
        lineStrings[i] = lineStrings[i].substr(2);
      }
      else if (dashedFirst) {
        lineFlags[i] = tog_fg;
        lineStrings[i] = lineStrings[i].substr(1);
      }
      else lineFlags[i] = arg_fg;
    }

    /** Fix input pattern */

    bool lineFlagsPrev = lineFlags[0];

    for (int i = 1; i < lineCount; i++) {
      // fix
      if (lineFlagsPrev == par_fg) {
        lineFlags[i] = arg_fg;
      }
      // error
      else if (
        lineFlagsPrev == tog_fg &&
        lineFlags[i] == arg_fg
      ) {
        Message::printCommandError(selCom);
        return;
      }

      lineFlagsPrev = lineFlags[i];
    }

    /** Values to be filled in */

    mt::VEC_STR arguments;
    mt::VEC_BOL conditions;
    mt::VEC_INT argsOrders, condsOrders;

    /** Iterate 'lineStrings' */

    Parameters *selPar = selCom->getParameters();
    Toggles *selTog = selCom->getToggles();

    mt::VEC_STR selParNames = selPar->getNames(),
      selTogNames = selTog->getNames();

    mt::VEC_BOL selParUsed = mt::VEC_BOL(selParNames.size(), false),
      selTogUsed = mt::VEC_BOL(selTogNames.size(), false);

    auto iterSelCom = [&](
      int lineCtr,
      mt::VEC_STR &selNames,
      mt::VEC_INT &orders,
      mt::VEC_BOL &selUsed
    ) {
      for (int j = 0; j < selNames.size(); j++) {
        if (lineStrings[lineCtr] == selNames.at(j)) {
          if (!selUsed[j]) {
            orders.push_back(j);
            selUsed[j] = true;
          }
          else Message::printCommandError(selCom);
          break;
        }
      }
    };

    for (int i = 0; i < lineCount; i++) {
      if (lineFlags[i] == par_fg) {
        iterSelCom(i, selParNames, argsOrders, selParUsed);
      }
      else if (lineFlags[i] == arg_fg) {
        arguments.push_back(lineStrings[i]);
      }
      else iterSelCom(i, selTogNames, condsOrders, selTogUsed);
    }

    conditions = selTogUsed;

    /** Improve results */

    // for (int i = 0; i < argsOrders.size(); i++) {
    //   for (int j = i+1; j < argsOrders.size(); j++) {
    //     if (argsOrders[i] > argsOrders[j]) {
    //       std::swap(arguments[i], arguments[j]);
    //       std::swap(argsOrders[i], argsOrders[j]);
    //     }
    //   }
    // }

    // for (int i = 0; i < condsOrders.size(); i++) {
    //   for (int j = i+1; j < condsOrders.size(); j++) {
    //     if (condsOrders[i] > condsOrders[j]) {
    //       std::swap(conditions[i], conditions[j]);
    //       std::swap(condsOrders[i], condsOrders[j]);
    //     }
    //   }
    // }

    // TEST DISPLAY //

    for (int i = 0; i < selParNames.size(); i++) {
      std::cout << selParNames[i] << ": "
        << arguments[i] << std::endl;
    }

    std::cout << std::endl;

    for (int i = 0; i < selTogNames.size(); i++) {
      std::cout << selTogNames[i] << ": "
        << conditions[i] << std::endl;
    }

    // release memories
    for (Command *com : commands) com->remove();
  }
}

#endif // __CLI_MENU__EXECUTOR_CPP__