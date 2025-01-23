#ifndef __CLI_MENU__CONTROL_H__
#define __CLI_MENU__CONTROL_H__

#include "message.h"

namespace cli_menu {

  class Control {
  private:
    static mt::USI sharedFlag;
    static bool modeOn;
    static const int TOTAL = 11;
    static std::string NAMES[TOTAL][2];

    static void colorize();
    static mt::SI whitespacesCheck(mt::CR_STR str);
    static bool checkOut(mt::CR_STR str, mt::CR_SI flag);

  public:
    enum _CONTROL_FLAG {
      _CONTROL_MODE = 0,
      BACK = 1, CLIPBOARD = 2,
      ENTER = 3, HELP = 4,
      LIST = 5, MODIFY = 6,
      NEXT = 7, PREVIOUS = 8,
      QUIT = 9, SELECT = 10
    };

    static void rename(
      _CONTROL_FLAG flag,
      mt::CR_STR name,
      mt::CR_STR abbreviation
    );

    static bool intoMode(mt::CR_STR str);
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