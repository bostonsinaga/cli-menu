#ifndef __CLI_MENU__CONTROL_H__
#define __CLI_MENU__CONTROL_H__

#include "message.h"

namespace cli_menu {

  class Control {
  private:
    static mt::USI sharedFlag;
    static bool modeOn;
    static const int COUNT = 11;
    static const std::string NAMES[COUNT][2];

    static mt::SI whitespacesCheck(mt::CR_STR str);
    static bool checkOut(mt::CR_STR str, mt::CR_SI flag);

  public:
    enum {
      _CONTROL_MODE = 0,
      BACK = 1, CANCEL = 2,
      CLIPBOARD = 3, ENTER = 4,
      HELP = 5, LIST = 6,
      MODIFY = 7, NEXT = 8,
      PREVIOUS = 9, SELECT = 10
    };

    static bool intoMode(mt::CR_STR str);
    static bool backTest(mt::CR_STR str);
    static bool cancelTest(mt::CR_STR str);
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

    // listen to shortcuts
    static void handleCtrlC(int signal);
    static void handleKeypress();

    static void printParameterHelp();
    static void printToggleHelp();

    static const mt::USI getSharedFlag() { return sharedFlag; }
    static const bool onMode() { return modeOn; }
    static void printError(mt::CR_STR listPointStyle = ">");
  };
}

#endif // __CLI_MENU__CONTROL_H__