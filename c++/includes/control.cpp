#ifndef __CLI_MENU__CONTROL_CPP__
#define __CLI_MENU__CONTROL_CPP__

#include "command.h"

namespace cli_menu {

  const std::string Command::Control::NAMES[6][2] = {
    { ":c", ".cancel" },
    { ":s", ".select" },
    { ":p", ".previous" },
    { ":n", ".next" },
    { ":b", ".back" },
    { ":e", ".enter" }
  };

  int Command::Control::test(std::string str) {

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

  bool Command::Control::cancelTest(std::string &str) {
    return test(str) == CANCEL + 1;
  }

  bool Command::Control::selectTest(std::string &str) {
    return test(str) == SELECT + 1;
  }

  bool Command::Control::previousTest(std::string &str) {
    return test(str) == PREVIOUS + 1;
  }

  bool Command::Control::nextTest(std::string &str) {
    return test(str) == NEXT + 1;
  }

  bool Command::Control::backTest(std::string &str) {
    return test(str) == BACK + 1;
  }

  bool Command::Control::enterTest(std::string &str) {
    return test(str) == ENTER + 1;
  }

  void Command::Control::printHelp() {
    std::cout << Color::getItalicString(
      std::string("  .cancel   = :c, .select = :s,\n") +
      std::string("  .previous = :p, .next   = :n,\n") +
      std::string("  .back     = :b, .enter  = :e \n")
    );
  }
}

#endif // __CLI_MENU__CONTROL_CPP__