#ifndef __CLI_MENU__BASE_CPP__
#define __CLI_MENU__BASE_CPP__

#include "base.h"

namespace cli_menu {

  Util::BOOL_ENUM Util::booleanTest(mt::CR_STR str) {
    if (str == "y" || str == "yes" ||
      str == "1" || str == "true"
    ) {
      return BOOL_TRUE;
    }
    else if (str == "n" || str == "no" ||
      str == "0" || str == "false"
    ) {
      return BOOL_FALSE;
    }
    return BOOL_OTHER;
  }

  bool DashTest::isLetter(mt::CR_STR str, mt::CR_INT index) {
    if (mt_uti::StrTools::isLetter(str[index])) {
      return true;
    }
    return false;
  }

  bool DashTest::isSingle(mt::CR_STR str) {

    if (str.length() > 1 && str[0] == '-' &&
      isLetter(str, 1)
    ) { return true; }

    return false;
  }

  bool DashTest::isDouble(mt::CR_STR str) {

    if (str.length() > 2 &&
      str[0] == '-' && str[1] == '-' &&
      isLetter(str, 2)
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