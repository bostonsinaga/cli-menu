#ifndef __CLI_MENU__BASE_H__
#define __CLI_MENU__BASE_H__

#include "mini-tools.h"

namespace cli_menu {

  // inheritance flags
  enum { COMMAND, PROGRAM, PARAMETER, TOGGLE };

  class Util {
  public:
    static bool isWhitespace(mt::CR_CH ch);

    // returns 0 (other), 1 (false), 2 (true)
    static int booleanTest(mt::CR_STR str);

    // use 'booleanTest' first before using this
    static bool revealBoolean(mt::CR_INT testedFlag);
  };

  class DashTest {
  private:
    static bool isLetter(mt::CR_STR str, mt::CR_INT index);

  public:
    static bool isSingle(mt::CR_STR str);
    static bool isDouble(mt::CR_STR str);
    static std::string cleanSingle(mt::CR_STR str);
    static std::string cleanDouble(mt::CR_STR str);
  };
}

#endif // __CLI_MENU__BASE_H__