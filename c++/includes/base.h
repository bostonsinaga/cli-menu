#ifndef __CLI_MENU__BASE_H__
#define __CLI_MENU__BASE_H__

#include "mini-tools.h"

namespace cli_menu {

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

#endif // __CLI_MENU__BASE_H__