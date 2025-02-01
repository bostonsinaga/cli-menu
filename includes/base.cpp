#ifndef __CLI_MENU__BASE_CPP__
#define __CLI_MENU__BASE_CPP__

#include "base.h"

namespace cli_menu {

  bool DashTest::isDigitLetter(mt::CR_STR str, mt::CR_INT index) {

    if (mt_uti::StrTools::isDigit(str[index]) ||
      mt_uti::StrTools::isLetter(str[index])
    ) { return true; }

    return false;
  }

  bool DashTest::isSingle(mt::CR_STR str) {

    if (str.length() > 1 && str[0] == '-' &&
      isDigitLetter(str, 1)
    ) { return true; }

    return false;
  }

  bool DashTest::isDouble(mt::CR_STR str) {

    if (str.length() > 2 &&
      str[0] == '-' && str[1] == '-' &&
      isDigitLetter(str, 2)
    ) { return true; }

    return false;
  }

  std::string DashTest::cleanSingle(mt::CR_STR str) {
    if (isSingle(str)) {
      return str.substr(1);
    }
    return "";
  }

  std::string DashTest::cleanDouble(mt::CR_STR str) {
    if (isDouble(str)) {
      return str.substr(2);
    }
    return "";
  }
}

#endif // __CLI_MENU__BASE_CPP__