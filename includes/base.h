#ifndef __CLI_MENU__BASE_H__
#define __CLI_MENU__BASE_H__

#include <memory>
#include "mini-tools.h"

namespace cli_menu {

  class DashTest {
  public:
    static bool isSingle(mt::CR_STR str);
    static bool isDouble(mt::CR_STR str);
    static std::string cleanSingle(mt::CR_STR str);
    static std::string cleanDouble(mt::CR_STR str);
  };
}

#endif // __CLI_MENU__BASE_H__