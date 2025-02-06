#ifndef __CLI_MENU__CONTROL_H__
#define __CLI_MENU__CONTROL_H__

#include "message.h"

namespace cli_menu {

  class Control {
  private:
    static mt::USI sharedFlag;
    static const int TOTAL = 12;

    static std::string
      NAMES[TOTAL][2], modeSymbol;

    static mt::SI whitespacesCheck(mt::CR_STR str);

  public:
    enum CONTROL_FLAG {
      BACK, CLIPBOARD, ENTER, HELP,
      LIST, MODIFY, NEXT, PREVIOUS,
      QUIT, RESET, SELECT, VIEW
    };

    static void rename(
      CONTROL_FLAG flag,
      mt::CR_STR name,
      mt::CR_STR abbreviation
    );

    static bool backTest(mt::CR_STR str);
    static bool quitTest(mt::CR_STR str);
    static bool clipboardTest(mt::CR_STR str);
    static bool enterTest(mt::CR_STR str);
    static bool helpTest(mt::CR_STR str);
    static bool listTest(mt::CR_STR str);
    static bool modifyTest(mt::CR_STR str);
    static bool nextTest(mt::CR_STR str);
    static bool previousTest(mt::CR_STR str);
    static bool resetTest(mt::CR_STR str);
    static bool selectTest(mt::CR_STR str);
    static bool viewTest(mt::CR_STR str);

    static void printParameterHelp();
    static void printToggleHelp();

    static const mt::USI getSharedFlag() { return sharedFlag; }
  };
}

#endif // __CLI_MENU__CONTROL_H__