#ifndef __CLI_MENU__BASE_CPP__
#define __CLI_MENU__BASE_CPP__

namespace cli_menu {

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

  std::string DashTest::getString(Command *command) {
    if (command) {
      if (command->getInheritanceFlag() == PARAMETER) {
        return "-";
      }
      else if (command->getInheritanceFlag() == TOGGLE) {
        return "--";
      }
      else return "";
    }
    else return "";
  }
}

#endif // __CLI_MENU__BASE_CPP__