#ifndef __CLI_MENU__BASE_H__
#define __CLI_MENU__BASE_H__

#include <memory>
#include "mini-tools.h"

namespace cli_menu {

  // inheritance flags
  enum {COMMAND, PROGRAM, PARAMETER, TOGGLE};

  // callback format
  typedef std::function<void(mt::CR_VEC_STR, mt::CR_VEC2_DBL, mt::CR_VEC_BOL)> CALLBACK;
  typedef std::function<void()> PLAIN_CALLBACK;
  typedef std::shared_ptr<CALLBACK> SP_CALLBACK;
  typedef std::shared_ptr<PLAIN_CALLBACK> SP_PLAIN_CALLBACK;
  typedef const SP_CALLBACK& CR_SP_CALLBACK;
  typedef const SP_PLAIN_CALLBACK& CR_SP_PLAIN_CALLBACK;

  struct ParamData {
    mt::VEC_STR texts;
    mt::VEC2_DBL numbers;
    mt::VEC_BOL conditions;
  };

  class DashTest {
  public:
    static bool isSingle(mt::CR_STR str);
    static bool isDouble(mt::CR_STR str);
    static bool cleanSingle(std::string &str);
    static bool cleanDouble(std::string &str);
  };
}

#endif // __CLI_MENU__BASE_H__