#ifndef __CLI_MENU__DASH_TEST_H__
#define __CLI_MENU__DASH_TEST_H__

#include "mini-tools.h"

namespace cli_menu {

  // callback format
  typedef std::function<void(mt::CR_VEC_STR, mt::CR_VEC2_DBL, mt::CR_VEC_BOL)> CALLBACK;

  // inheritance flags
  enum {COMMAND, PARAMETER, TOGGLE};

  class Command;
  typedef std::vector<Command*> VEC_COM;
  typedef const VEC_COM& CR_VEC_COM;

  class DashTest {
  public:
    static bool isSingle(mt::CR_STR str);
    static bool isDouble(mt::CR_STR str);
    static bool cleanSingle(std::string &str);
    static bool cleanDouble(std::string &str);
    static std::string getString(Command *com);
  };
}

#endif // __CLI_MENU__DASH_TEST_H__