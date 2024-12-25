#ifndef __CLI_MENU__DASH_TEST_H__
#define __CLI_MENU__DASH_TEST_H__

#include "mini-tools.h"

namespace cli_menu {
namespace dash_test {

  bool isSingle(mt::CR_STR str) {
    if (str.length() > 1 && str[0] == '-') {
      return true;
    }
    return false;
  }

  bool isDouble(mt::CR_STR str) {
    if (str.length() > 2 &&
      str[0] == '-' && str[1] == '-'
    ) { return true; }
    return false;
  }

  std::string cleanSingle(mt::CR_STR str) {
    if (isSingle(str)) {
      return str.substr(1);
    }
    return "";
  }

  std::string cleanDouble(mt::CR_STR str) {
    if (isDouble(str)) {
      return str.substr(2);
    }
    return "";
  }
}}

#endif // __CLI_MENU__DASH_TEST_H__