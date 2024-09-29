#ifndef __CLI_MENU__CONTROL_CPP__
#define __CLI_MENU__CONTROL_CPP__

#include "command.h"

namespace cli_menu {

  const std::string
    Command::Control::CANCEL[2] = { ":c", ".cancel" },
    Command::Control::ENTER[2] = { ":e", ".enter" },
    Command::Control::NEXT[2] = { ":n", ".next" },
    Command::Control::SELECT[2] = { ":s", ".select" };

  int Command::Control::test(std::string &str) {
    mt_uti::StrTools::changeStringToLowercase(str);

    for (int i = 0; i < 2; i++) {
      if (str == Command::Control::CANCEL[i]) return CANCEL_FG;
      else if (str == Command::Control::ENTER[i]) return ENTER_FG;
      else if (str == Command::Control::NEXT[i]) return NEXT_FG;
      else if (str == Command::Control::SELECT[i]) return SELECT_FG;
    }

    return 0;
  }

  bool Command::Control::cancelTest(std::string &str) {
    return test(str) == CANCEL_FG;
  }

  bool Command::Control::enterTest(std::string &str) {
    return test(str) == ENTER_FG;
  }

  bool Command::Control::nextTest(std::string &str) {
    return test(str) == NEXT_FG;
  }

  bool Command::Control::selectTest(std::string &str) {
    return test(str) == SELECT_FG;
  }
}

#endif // __CLI_MENU__CONTROL_CPP__