#ifndef __CLI_MENU__BASE_HPP__
#define __CLI_MENU__BASE_HPP__

#include "mini-tools.h"

namespace cli_menu {

  /**
   * As coefficients in a conditional block
   * that can provide options for polymorphism.
   */
  enum INHERITANCE_ENUM {
    INHERITANCE_COMMAND,
    INHERITANCE_PROGRAM,
    INHERITANCE_PARAMETER,
    INHERITANCE_TOGGLE
  };

  class Util {
  public:
    enum BOOL_ENUM {
      BOOL_OTHER, BOOL_FALSE, BOOL_TRUE
    };

    static BOOL_ENUM booleanTest(mt::CR_STR str);

    // use 'booleanTest' first before using this
    static bool revealBoolean(BOOL_ENUM testedEnum) {
      return testedEnum == BOOL_TRUE;
    }
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

#endif // __CLI_MENU__BASE_HPP__