#ifndef __CLI_MENU__CONTROL_CPP__
#define __CLI_MENU__CONTROL_CPP__

#include "control.h"

namespace cli_menu {
  /**
   * All tests argument are expected to be lowercase.
   */
  const std::string Control::NAMES[Control::count][2] = {
    { ".back",     ":b" },
    { ".cancel",   ":x" },
    { ".copy",     ":c" },
    { ".enter",    ":e" },
    { ".help",     ":h" }, 
    { ".list",     ":l" },
    { ".modify",   ":m" },
    { ".next",     ":n" },
    { ".previous", ":p" },
    { ".select",   ":s" }
  };

  mt::USI Control::sharedFlag = -1;

  int Control::test(mt::CR_STR str) {
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
      for (int j = 0; j < Control::count; j++) {

        if (input == NAMES[j][i]) {
          sharedFlag = j;
          return j;
        }
      }
    }

    return -1;
  }

  bool Control::backTest(mt::CR_STR str) {
    return Control::test(str) == BACK;
  }

  bool Control::cancelTest(mt::CR_STR str) {
    return Control::test(str) == CANCEL;
  }

  bool Control::copyTest(mt::CR_STR str) {
    return Control::test(str) == COPY;
  }

  bool Control::enterTest(mt::CR_STR str) {
    return Control::test(str) == ENTER;
  }

  bool Control::helpTest(mt::CR_STR str) {
    return Control::test(str) == HELP;
  }

  bool Control::listTest(mt::CR_STR str) {
    return Control::test(str) == LIST;
  }

  bool Control::modifyTest(mt::CR_STR str) {
    return Control::test(str) == MODIFY;
  }

  bool Control::nextTest(mt::CR_STR str) {
    return Control::test(str) == NEXT;
  }

  bool Control::previousTest(mt::CR_STR str) {
    return Control::test(str) == PREVIOUS;
  }

  bool Control::selectTest(mt::CR_STR str) {
    return Control::test(str) == SELECT;
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

      for (int i = 0; i < Control::count; i++) {
        even = !(i % 2);

        if (Control::NAMES[i][0].length() > maxLengths[even]) {
          maxLengths[even] = Control::NAMES[i][0].length();
        }
      }

      for (int i = 0; i < Control::count; i++) {
        even = !(i % 2);

        text += (even ? "  " : " ") + Control::NAMES[i][0] + std::string(
          maxLengths[even] - Control::NAMES[i][0].length(), ' '
        ) + " = " + Control::NAMES[i][1] + (even ? "," : "\n");
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