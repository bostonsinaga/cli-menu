#ifndef __CLI_MENU__CONTROL_CPP__
#define __CLI_MENU__CONTROL_CPP__

#include "control.hpp"

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
  CONTROL_ENUM Control::sharedEnum = CONTROL_BACK;

  void Control::rename(
    const CONTROL_ENUM& index,
    mt::CR_STR name,
    mt::CR_STR abbreviation
  ) {
    NAMES[index][0] = name;
    NAMES[index][1] = abbreviation;
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
          sharedEnum = static_cast<CONTROL_ENUM>(j);
          return j;
        }
      }
    }

    return -1;
  }

  bool Control::backTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_BACK;
  }

  bool Control::clipboardTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_CLIPBOARD;
  }

  bool Control::enterTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_ENTER;
  }

  bool Control::helpTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_HELP;
  }

  bool Control::listTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_LIST;
  }

  bool Control::modifyTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_MODIFY;
  }

  bool Control::nextTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_NEXT;
  }

  bool Control::previousTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_PREVIOUS;
  }

  bool Control::quitTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_QUIT;
  }

  bool Control::resetTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_RESET;
  }

  bool Control::selectTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_SELECT;
  }

  bool Control::viewTest(mt::CR_STR str) {
    return whitespacesCheck(str) == CONTROL_VIEW;
  }

  void Control::printParameterHelp() {

    static int maxLengths[] = {0, 0};
    static std::string text = "";

    if (!(maxLengths[0] || maxLengths[1])) {
      bool even;

      for (int i = 0; i < TOTAL; i++) {
        even = !(i % 2);

        if (NAMES[i][0].length() > maxLengths[even]) {
          maxLengths[even] = NAMES[i][0].length();
        }
      }

      for (int i = 0; i < TOTAL; i++) {
        even = !(i % 2);
        text += even ? "  " : " ";

        text += modeSymbol + NAMES[i][0] + std::string(
          maxLengths[even] - NAMES[i][0].length(), ' '
        );

        text += " = " + modeSymbol + NAMES[i][1];

        if (i < TOTAL - 1) {
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