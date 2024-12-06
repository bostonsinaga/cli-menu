#ifndef __CLI_MENU__CONTROL_H__
#define __CLI_MENU__CONTROL_H__

#include "message.h"

namespace cli_menu {

  class Control {
  private:
    enum {
      BACK = 0, CANCEL = 1,
      ENTER = 2, HELP  = 3, 
      LIST = 4, NEXT = 5,
      PREVIOUS = 6, SELECT = 7
    };

    static const int count = 10;
    static const std::string NAMES[count][2];
    static int test(mt::CR_STR str);

  public:
    static bool cancelTest(mt::CR_STR str);
    static bool backTest(mt::CR_STR str);
    static bool enterTest(mt::CR_STR str);
    static bool nextTest(mt::CR_STR str);
    static bool previousTest(mt::CR_STR str);
    static bool selectTest(mt::CR_STR str);

    // returns 0 (other), 1 (false), 2 (true)
    static int booleanTest(mt::CR_STR str);
    static bool revealBoolean(mt::CR_INT testedFlag);

    static void printParameterHelp();
    static void printToggleHelp();
  };
}

#endif // __CLI_MENU__CONTROL_H__