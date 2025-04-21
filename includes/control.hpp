#ifndef __CLI_MENU__CONTROL_H__
#define __CLI_MENU__CONTROL_H__

#include "message.h"

namespace cli_menu {

  // obtained after testing the input string
  enum CONTROL_ENUM {
    CONTROL_BACK, CONTROL_CLIPBOARD, CONTROL_ENTER, CONTROL_HELP,
    CONTROL_LIST, CONTROL_MODIFY, CONTROL_NEXT, CONTROL_PREVIOUS,
    CONTROL_QUIT, CONTROL_RESET, CONTROL_SELECT, CONTROL_VIEW
  };

  class Control {
  private:
    static CONTROL_ENUM sharedEnum;
    static const int TOTAL = 12;

    static std::string
      NAMES[TOTAL][2], modeSymbol;

    static mt::SI whitespacesCheck(mt::CR_STR str);

  public:
    static void rename(
      const CONTROL_ENUM& index,
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

    static const CONTROL_ENUM getSharedEnum() {
      return sharedEnum;
    }
  };
}

#endif // __CLI_MENU__CONTROL_H__