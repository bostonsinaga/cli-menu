#ifndef __CLI_MENU__BASE_H__
#define __CLI_MENU__BASE_H__

#include "mini-tools.h"

namespace cli_menu {

  // inheritance flags
  enum { COMMAND, PROGRAM, PARAMETER, TOGGLE };

  class DashTest {
  public:
    static bool isSingle(mt::CR_STR str) {
      if (str.length() > 1 && str[0] == '-') {
        return true;
      }
      return false;
    }

    static bool isDouble(mt::CR_STR str) {
      if (str.length() > 2 &&
        str[0] == '-' && str[1] == '-'
      ) { return true; }
      return false;
    }

    static std::string cleanSingle(mt::CR_STR str) {
      if (isSingle(str)) {
        return str.substr(1);
      }
      return "";
    }

    static std::string cleanDouble(mt::CR_STR str) {
      if (isDouble(str)) {
        return str.substr(2);
      }
      return "";
    }
  };
}

#endif // __CLI_MENU__BASE_H__