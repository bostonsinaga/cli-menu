#ifndef __CLI_MENU__CONTROL_CPP__
#define __CLI_MENU__CONTROL_CPP__

#include "control.hpp"

namespace cli_menu {

  /** Controllers Test */

  CONTROL_CODE Control::whitespacesCheck(mt::CR_STR str) {
    bool prevSpaced = false;
    std::string input;

    // force to return 'UNKNOWN' when pattern 'abc123 \t abc123' is detected
    for (mt::CR_CH ch : str) {
      if (!mt_uti::StrTools::isWhitespace(ch)) {
        if (prevSpaced && !input.empty()) return CONTROL_UNKNOWN;
        input += ch;
      }
      else prevSpaced = true;
    }

    // find a match with pattern ' abc123 \t'
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < Control::totalSymbols; j++) {

        if (input == symbols[j][i]) {
          sharedEnum = static_cast<CONTROL_CODE>(j+1);
          return sharedEnum;
        }
      }
    }

    return CONTROL_UNKNOWN;
  }

  bool Control::helpTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_HELP;
  }

  bool Control::listTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_LIST;
  }

  bool Control::enterTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_ENTER;
  }

  bool Control::backTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_BACK;
  }

  bool Control::nextTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_NEXT;
  }

  bool Control::previousTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_PREVIOUS;
  }

  bool Control::modifyTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_MODIFY;
  }

  bool Control::selectTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_SELECT;
  }

  bool Control::resetThisTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_RESET_THIS;
  }

  bool Control::viewThisTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_VIEW_THIS;
  }

  bool Control::resetAllTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_RESET_ALL;
  }

  bool Control::viewAllTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_VIEW_ALL;
  }

  bool Control::copyTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_COPY;
  }

  bool Control::pasteTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_PASTE;
  }

  bool Control::quitTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_QUIT;
  }

  void Control::printAbbreviations() {

    Console::logItalicString(
      Langu::ageControl::getAbbreviationsTitle() + ":\n",
      Console::messageColors[CONSOLE_HINT_1]
    );

    size_t leastMaxTermLength = 0;

    // find the maximum term length at even indices
    for (int i = 0; i < Control::totalSymbols; i++) {

      size_t curlen = Langu::ageControl::getTerm(static_cast<CONTROL_CODE>(i)).length();

      if (i % 2 == 0 && curlen > leastMaxTermLength) {
        leastMaxTermLength = curlen;
      }
    }

    // display terms with 2 columns
    for (int i = 0; i < Control::totalSymbols; i++) {
      std::string curterm = Langu::ageControl::getTerm(static_cast<CONTROL_CODE>(i));

      // even is followed by spaces, uneven is followed by newline
      Console::logString(
        "  " + symbols[i][0] + " = " + curterm
        + (i % 2 ? "\n" : std::string(leastMaxTermLength - curterm.length(), ' ')),
        Console::messageColors[CONSOLE_HINT_2]
      );
    }

    std::cout << "\n\n";
  }

  void Control::printBooleanAvailableValues() {

    Console::logItalicString(
      Langu::ageControl::getBooleanAvailableValuesTitle() + ":\n  ",
      Console::messageColors[CONSOLE_HINT_1]
    );

    mt::CR_PAIR<mt::VEC_STR> boolTerms = Langu::ageBooleanizer::getTerms();

    // true terms
    for (int i = 0; i < boolTerms.first.size(); i++) {
      Console::logString(
        boolTerms.first[i] + ", ",
        Console::messageColors[CONSOLE_HINT_2]
      );
    }

    // number is not zero
    Console::logString(
      "n != 0\n  ",
      Console::messageColors[CONSOLE_HINT_2]
    );

    // false terms
    for (int i = 0; i < boolTerms.second.size(); i++) {
      Console::logString(
        boolTerms.second[i] + ", ",
        Console::messageColors[CONSOLE_HINT_2]
      );
    }

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