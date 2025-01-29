#ifndef __CLI_MENU__CONTROL_CPP__
#define __CLI_MENU__CONTROL_CPP__

#include "control.h"

namespace cli_menu {
  /**
   * The test argument is expected to be lowercase.
   */
  std::string Control::NAMES[TOTAL][2] = {
    { "back",      "b" },
    { "clipboard", "c" },
    { "enter",     "e" },
    { "help",      "h" },
    { "list",      "l" },
    { "modify",    "m" },
    { "next",      "n" },
    { "previous",  "p" },
    { "quit",      "q" },
    { "reset",     "r" },
    { "select",    "s" },
    { "view",      "v" }
  };

  std::string Control::modeSymbol = ":";
  mt::USI Control::sharedFlag = -1;

  void Control::rename(
    CONTROL_FLAG flag,
    mt::CR_STR name,
    mt::CR_STR abbreviation
  ) {
    NAMES[flag][0] = name;
    NAMES[flag][1] = abbreviation;
  }

  mt::SI Control::whitespacesCheck(mt::CR_STR str) {
    bool prevSpaced = false;
    std::string input;

    // force return when 'abc123 space abc123' is detected
    for (mt::CR_CH ch : str) {
      if (!mt_uti::StrTools::isWhitespace(ch)) {
        if (prevSpaced && !input.empty()) return -1;
        input += ch;
      }
      else prevSpaced = true;
    }

    // find a match with pattern ' abc123 \t'
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < TOTAL; j++) {

        if (input == modeSymbol + NAMES[j][i]) {
          sharedFlag = j;
          return j;
        }
      }
    }

    return -1;
  }

  bool Control::backTest(mt::CR_STR str) {
    return whitespacesCheck(str) == BACK;
  }

  bool Control::clipboardTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CLIPBOARD;
  }

  bool Control::enterTest(mt::CR_STR str) {
    return whitespacesCheck(str) == ENTER;
  }

  bool Control::helpTest(mt::CR_STR str) {
    return whitespacesCheck(str) == HELP;
  }

  bool Control::listTest(mt::CR_STR str) {
    return whitespacesCheck(str) == LIST;
  }

  bool Control::modifyTest(mt::CR_STR str) {
    return whitespacesCheck(str) == MODIFY;
  }

  bool Control::nextTest(mt::CR_STR str) {
    return whitespacesCheck(str) == NEXT;
  }

  bool Control::previousTest(mt::CR_STR str) {
    return whitespacesCheck(str) == PREVIOUS;
  }

  bool Control::quitTest(mt::CR_STR str) {
    return whitespacesCheck(str) == QUIT;
  }

  bool Control::resetTest(mt::CR_STR str) {
    return whitespacesCheck(str) == RESET;
  }

  bool Control::selectTest(mt::CR_STR str) {
    return whitespacesCheck(str) == SELECT;
  }

  bool Control::viewTest(mt::CR_STR str) {
    return whitespacesCheck(str) == VIEW;
  }

  int Control::booleanTest(mt::CR_STR str) {
    if (str == "y" || str == "yes" ||
      str == "1" || str == "true"
    ) {
      return 2;
    }
    else if (str == "n" || str == "no" ||
      str == "0" || str == "false"
    ) {
      return 1;
    }
    return 0;
  }

  bool Control::revealBoolean(mt::CR_INT testedFlag) {
    if (testedFlag > 1) return true;
    return false;
  }

  void Control::printParameterHelp() {

    static int maxLengths[] = {0, 0};
    static std::string text = "";

    if (!(maxLengths[0] || maxLengths[1])) {
      bool even;

      for (int i = 0; i < TOTAL - 1; i++) {
        even = !(i % 2);

        if (NAMES[i][0].length() > maxLengths[even]) {
          maxLengths[even] = NAMES[i][0].length();
        }
      }

      for (int i = 0; i < TOTAL - 1; i++) {
        even = !(i % 2);
        text += even ? "  " : " ";

        text += modeSymbol + NAMES[i][0] + std::string(
          maxLengths[even] - NAMES[i][0].length(), ' '
        );

        text += " = " + modeSymbol + NAMES[i][1];

        if (i < TOTAL - 2) {
          text += even ? "," : ",\n";
        }
        else text += "\n";
      }
    }

    Message::printItalicString(text);
  }

  void Control::printToggleHelp() {
    Message::printItalicString(
      "  yes = y, no = n, or boolean\n"
    );
  }
}

#endif // __CLI_MENU__CONTROL_CPP__