#ifndef __CLI_MENU__BASE_H__
#define __CLI_MENU__BASE_H__

#include "mini-tools.h"
#define assertm(exp, msg) assert(((void)msg, exp))

namespace cli_menu {

  // inheritance flags
  enum {COMMAND, PROGRAM, PARAMETER, TOGGLE};

  // callback format
  typedef std::function<void(mt::CR_VEC_STR, mt::CR_VEC2_DBL, mt::CR_VEC_BOL)> CALLBACK;

  class DashTest {
  public:
    static bool isSingle(mt::CR_STR str);
    static bool isDouble(mt::CR_STR str);
    static bool cleanSingle(std::string &str);
    static bool cleanDouble(std::string &str);
  };
}

#endif // __CLI_MENU__BASE_H__