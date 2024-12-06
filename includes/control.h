#ifndef __CLI_MENU__CONTROL_H__
#define __CLI_MENU__CONTROL_H__

#include "message.h"

namespace cli_menu {

  class Control {
  private:
    enum {
      BACK = 0, CANCEL = 1,
      COPY = 2, ENTER = 3,
      HELP = 4, LIST = 5,
      MODIFY = 6, NEXT = 7,
      PREVIOUS = 8, SELECT = 9
    };

    static const int count = 10;
    static const std::string NAMES[count][2];
    static int test(mt::CR_STR str);

  public:
    static bool backTest(mt::CR_STR str);
    static bool cancelTest(mt::CR_STR str);
    static bool copyTest(mt::CR_STR str);
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
  };
}

#endif // __CLI_MENU__CONTROL_H__