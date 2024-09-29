#ifndef __CLI_MENU__CONTROL_CPP__
#define __CLI_MENU__CONTROL_CPP__

#include "command.h"

namespace cli_menu {
  const std::string
    Command::Control::CANCEL = ":c",
    Command::Control::ENTER = ":e",
    Command::Control::NEXT = ":n";

  int Command::Control::test(std::string &str) {
    mt_uti::StrTools::changeStringToLowercase(str);

    if (str == Command::Control::CANCEL) return 1;
    else if (str == Command::Control::ENTER) return 2;
    else if (str == Command::Control::NEXT) return 3;
    return 0;
  }

  bool Command::Control::cancelTest(std::string &str) {
    return test(str) == 1;
  }

  bool Command::Control::enterTest(std::string &str) {
    return test(str) == 2;
  }

  bool Command::Control::nextTest(std::string &str) {
    return test(str) == 3;
  }
}

#endif // __CLI_MENU__CONTROL_CPP__