#ifndef __CLI_MENU__CONTROL_CPP__
#define __CLI_MENU__CONTROL_CPP__

#include "control.h"

namespace cli_menu {

  const std::string Control::NAMES[6][2] = {
    { ":c", ".cancel" },
    { ":s", ".select" },
    { ":p", ".previous" },
    { ":n", ".next" },
    { ":b", ".back" },
    { ":e", ".enter" }
  };

  int Control::test(std::string str) {

    int spaceChange = 0;
    mt_uti::StrTools::changeStringToLowercase(str);

    for (char &ch : str) {
      if (ch != ' ' && ch != '\t') {
        if (spaceChange == 0) spaceChange = 1;
        else if (spaceChange == 2) return 0;
      }
      else if (
        (ch == ' ' || ch == '\t') &&
        spaceChange == 1
      ) { spaceChange = 2; }
    }

    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 6; j++) {
        if (str.find(NAMES[j][i]) != std::string::npos) {
          return j + 1;
        }
      }
    }

    return 0;
  }

  bool Control::cancelTest(std::string &str) {
    return test(str) == CANCEL + 1;
  }

  bool Control::selectTest(std::string &str) {
    return test(str) == SELECT + 1;
  }

  bool Control::previousTest(std::string &str) {
    return test(str) == PREVIOUS + 1;
  }

  bool Control::nextTest(std::string &str) {
    return test(str) == NEXT + 1;
  }

  bool Control::backTest(std::string &str) {
    return test(str) == BACK + 1;
  }

  bool Control::enterTest(std::string &str) {
    return test(str) == ENTER + 1;
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