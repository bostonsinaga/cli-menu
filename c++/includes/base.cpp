#ifndef __CLI_MENU__BASE_CPP__
#define __CLI_MENU__BASE_CPP__

#include "base.h"

namespace cli_menu {

  mt::VEC_STR DIALOG() {
    static bool isInit = true;

    mt::VEC_STR textVec;
    std::string buffer;

    if (isInit) {
      std::cout << "\n\nINPUT: (cancel = :q, next = :w)\n";
    }
    else std::cout << "\nINPUT:\n";

    while (std::getline(std::cin, buffer)) {
      if (buffer == ":q") return {};
      else if (buffer == ":w") break;
      textVec.push_back(buffer);
    }

    isInit = false;
    return textVec;
  }

  bool DashTest::isSingle(mt::CR_STR str) {
    if (str.length() > 1 && str[0] == '-') {
      return true;
    }
    return false;
  }

  bool DashTest::isDouble(mt::CR_STR str) {
    if (str.length() > 2 &&
      str[0] == '-' && str[1] == '-'
    ) { return true; }
    return false;
  }

  bool DashTest::cleanSingle(std::string &str) {
    if (isSingle(str)) {
      str = str.substr(1);
      return true;
    }
    return false;
  }

  bool DashTest::cleanDouble(std::string &str) {
    if (isDouble(str)) {
      str = str.substr(2);
      return true;
    }
    return false;
  }
}

#endif // __CLI_MENU__BASE_CPP__