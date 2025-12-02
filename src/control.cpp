#ifndef __CLI_MENU__CONTROL_CPP__
#define __CLI_MENU__CONTROL_CPP__

#include "control.hpp"

namespace cli_menu {

  /** Controllers Test */

  CONTROL_CODE Control::whitespacesCheck(mt::CR_STR rawstr) {
    bool prevSpaced = false;
    std::string str;

    // force to return 'UNKNOWN' when pattern 'abc123 \t abc123' is detected
    for (mt::CR_CH ch : rawstr) {
      if (!mt_uti::StrTools::isWhitespace(ch)) {

        if (prevSpaced && !str.empty()) {
          return CONTROL_UNKNOWN;
        }

        str += ch;
        prevSpaced = false;
      }
      else prevSpaced = true;
    }

    // find a match with pattern ' abc123 \t'
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < Control::totalSymbols; j++) {

        if (str == symbols[j][i].first) {
          sharedEnum = static_cast<CONTROL_CODE>(symbols[j][i].second);
          return sharedEnum;
        }
      }
    }

    return CONTROL_UNKNOWN;
  }

  bool Control::commandHelpTest(mt::CR_STR rawstr) {
    bool isEqual = whitespacesCheck(rawstr) == CONTROL_COMMAND_HELP;
    return isEqual;
  }

  bool Control::controllerListTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_CONTROLLER_LIST;
  }

  bool Control::childrenEnterTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_CHILDREN_ENTER;
  }

  bool Control::childrenListTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_CHILDREN_LIST;
  }

  bool Control::neighborNextTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_NEIGHBOR_NEXT;
  }

  bool Control::neighborPreviousTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_NEIGHBOR_PREVIOUS;
  }

  bool Control::switchModifyTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_SWITCH_MODIFY;
  }

  bool Control::switchSelectTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_SWITCH_SELECT;
  }

  bool Control::viewInputThisTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_VIEW_INPUT_THIS;
  }

  bool Control::viewOutputThisTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_VIEW_OUTPUT_THIS;
  }

  bool Control::viewInputAllTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_VIEW_INPUT_ALL;
  }

  bool Control::viewOutputAllTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_VIEW_OUTPUT_ALL;
  }

  bool Control::resetInputThisTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_RESET_INPUT_THIS;
  }

  bool Control::resetOutputThisTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_RESET_OUTPUT_THIS;
  }

  bool Control::resetInputAllTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_RESET_INPUT_ALL;
  }

  bool Control::resetOutputAllTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_RESET_OUTPUT_ALL;
  }

  bool Control::copyOutputTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_COPY_OUTPUT;
  }

  bool Control::pasteInputTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_PASTE_INPUT;
  }

  bool Control::parentBackTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_PARENT_BACK;
  }

  bool Control::programQuitTest(mt::CR_STR rawstr) {
    return whitespacesCheck(rawstr) == CONTROL_PROGRAM_QUIT;
  }

  void Control::printAbbreviations(
    mt::CR_BOL titleDisplayed,
    mt::CR_SZ numberOfIndents
  ) {
    if (titleDisplayed) {
      Console::logItalicString(
        Langu::ageControl::getAbbreviationsTitle() + ":\n",
        Console::messageColors[CONSOLE_HINT_1]
      );
    }

    // include lowercase and uppercase symbols that have different codes
    std::vector<const std::string*> onlySymbols;

    for (int i = 0; i < Control::totalSymbols; i++) {
      onlySymbols.push_back(&Control::symbols[i][0].first);

      if (Control::symbols[i][0].second != Control::symbols[i][1].second) {
        onlySymbols.push_back(&Control::symbols[i][1].first);
      }
    }

    // display terms in rows
    for (int i = 0; i < Control::totalSymbols; i++) {

      std::cout << std::string(numberOfIndents, ' ');
      std::string curterm = Langu::ageControl::getTerm(static_cast<CONTROL_CODE>(i));

      Console::logString(
        *onlySymbols[i] + " = " + curterm + '\n',
        Console::messageColors[CONSOLE_HINT_2]
      );
    }

    // additional newline
    std::cout << std::endl;
  }

  void Control::printBooleanAvailableValues(
    mt::CR_BOL titleDisplayed,
    mt::CR_SZ numberOfIndents
  ) {
    if (titleDisplayed) {
      Console::logItalicString(
        Langu::ageControl::getBooleanAvailableValuesTitle() + ":\n",
        Console::messageColors[CONSOLE_HINT_1]
      );
    }

    mt::CR_PAIR<mt::VEC_STR> boolTerms = Langu::ageBooleanizer::getTerms();

    /** True Terms */

    std::cout << std::string(numberOfIndents, ' ');

    for (int i = 0; i < boolTerms.first.size(); i++) {
      Console::logString(
        boolTerms.first[i] + ", ",
        Console::messageColors[CONSOLE_HINT_2]
      );
    }

    Console::logString(
      symbols[CONTROL_CHILDREN_ENTER][0].first + ", ",
      Console::messageColors[CONSOLE_HINT_2]
    );

    Console::logString(
      symbols[CONTROL_NEIGHBOR_NEXT][0].first + ", ",
      Console::messageColors[CONSOLE_HINT_2]
    );

    // number is not zero
    Console::logString(
      "n != 0\n",
      Console::messageColors[CONSOLE_HINT_2]
    );

    /** False Terms */

    std::cout << std::string(numberOfIndents, ' ');

    for (int i = 0; i < boolTerms.second.size(); i++) {
      Console::logString(
        boolTerms.second[i] + ", ",
        Console::messageColors[CONSOLE_HINT_2]
      );
    }

    Console::logString(
      symbols[CONTROL_NEIGHBOR_PREVIOUS][0].first + ", ",
      Console::messageColors[CONSOLE_HINT_2]
    );

    // number is zero
    Console::logString(
      "n == 0\n\n",
      Console::messageColors[CONSOLE_HINT_2]
    );
  }

  /** Interrupted 'Ctrl+C' Interactions */

  bool Control::cinDialogInput(
    std::string &buffer,
    mt::CR_BOL editing
  ) {
    // decoration string
    Console::logString(
      Console::listPointStyle + ' ',
      Console::chooseBoundaryColor(editing)
    );

    if (Control::INTERRUPTED_CTRL_C.load()) return false; // stop loop

    // user input
    std::getline(std::cin, buffer);

    if (Control::INTERRUPTED_CTRL_C.load()) return false; // stop loop

    // loop still running
    return true;
  }

  void Control::setInterruptedCtrlC(int) {
    Control::INTERRUPTED_CTRL_C.store(true);
  }
}

#endif // __CLI_MENU__CONTROL_CPP__