#ifndef __CLI_MENU__CONTROL_CPP__
#define __CLI_MENU__CONTROL_CPP__

#include "control.h"

namespace cli_menu {
  /**
   * All tests argument are expected to be lowercase.
   */
  const std::string Control::NAMES[count][2] = {
    { ".back", ":b" },
    { ".cancel", ":c" },
    { ".enter", ":e" },
    { ".help", ":h" }, 
    { ".list", ":l" },
    { ".next", ":n" },
    { ".previous", ":p" },
    { ".select", ":s" }
  };

  int Control::test(mt::CR_STR str) {
    int spaceChange = 0;

    static std::function<bool(mt::CR_CH)>
      isSpace = [](mt::CR_CH ch)->bool {
        return ch == ' ' || ch == '\t';
      },
      isNotSpace = [](mt::CR_CH ch)->bool {
        return ch != ' ' && ch != '\t';
      };

    // force return when 'abc123 space abc123' is detected
    for (mt::CR_CH ch : str) {
      if (isNotSpace(ch)) {
        if (spaceChange == 0) spaceChange = 1;
        else if (spaceChange == 2) return 0;
      }
      else if (spaceChange == 1 && isSpace(ch)) {
        spaceChange = 2;
      }
    }

    // find a match with pattern ' abc123 \t'
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < count; j++) {

        if (str.length() >= NAMES[j][i].length()) {
          bool found = false;

          for (int k = 0;
            k <= str.length() - NAMES[j][i].length();
            k++
          ) {
            bool kWillBreak = false;

            for (int l = 0; l < NAMES[j][i].length(); l++) {
              if (str[l+k] != NAMES[j][i][l]) {
                if (isNotSpace(str[l+k])) {
                  kWillBreak = true;
                  found = false;
                }
                else if (found) {
                  kWillBreak = true;
                }
                break;
              }
              else found = true;
            }

            if (kWillBreak) break;
          }

          if (found) return j+1;
        }
      }
    }

    return 0;
  }

  bool Control::cancelTest(mt::CR_STR str) {
    return test(str) == CANCEL + 1;
  }

  bool Control::selectTest(mt::CR_STR str) {
    return test(str) == SELECT + 1;
  }

  bool Control::previousTest(mt::CR_STR str) {
    return test(str) == PREVIOUS + 1;
  }

  bool Control::nextTest(mt::CR_STR str) {
    return test(str) == NEXT + 1;
  }

  bool Control::backTest(mt::CR_STR str) {
    return test(str) == BACK + 1;
  }

  bool Control::enterTest(mt::CR_STR str) {
    return test(str) == ENTER + 1;
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

  void Control::printHelp(mt::CR_BOL isSupporter) {
    static int groupPhase = isSupporter;
    if (groupPhase == 3) return;

    switch (groupPhase) {
      case 0: {
        Message::printItalicString(
          std::string("  .back = :b, .cancel   = :c,\n") +
          std::string("  .help = :h, .list     = :l,\n") +
          std::string("  .next = :n, .previous = :p\n")
        );
        groupPhase = 2;
      break;}
      case 1: {
        Message::printItalicString(
          std::string("  .back     = :b, .cancel = :c,\n") +
          std::string("  .enter    = :e, .help   = :h,\n") +
          std::string("  .list     = :l, .next   = :n,\n") +
          std::string("  .previous = :p, .select = :s\n")
        );
        groupPhase = 3;
      break;}
      case 2: {
        Message::printItalicString("  .enter = :e, .select = :s\n");
        groupPhase = 3;
      break;}
    }
  }
}

#endif // __CLI_MENU__CONTROL_CPP__