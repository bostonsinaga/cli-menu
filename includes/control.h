#ifndef __CLI_MENU__CONTROL_H__
#define __CLI_MENU__CONTROL_H__

#include "message.h"

namespace cli_menu {

  class Control {
  private:
    static mt::USI sharedFlag;
    static const int TOTAL = 10;

    static std::string
      NAMES[TOTAL][2], modeSymbol;

    static mt::SI whitespacesCheck(mt::CR_STR str);

  public:
    enum CONTROL_FLAG {
      BACK = 0, CLIPBOARD = 1,
      ENTER = 2, HELP = 3,
      LIST = 4, MODIFY = 5,
      NEXT = 6, PREVIOUS = 7,
      QUIT = 8, SELECT = 9
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
    static bool selectTest(mt::CR_STR str);

    // returns 0 (other), 1 (false), 2 (true)
    static int booleanTest(mt::CR_STR str);
    static bool revealBoolean(mt::CR_INT testedFlag);

    static void printParameterHelp();
    static void printToggleHelp();

    static const mt::USI getSharedFlag() { return sharedFlag; }
  };
}

#endif // __CLI_MENU__CONTROL_H__