#ifndef __CLI_MENU__UTIL_H__
#define __CLI_MENU__UTIL_H__

#include <vector>

namespace cli_menu {
  typedef std::vector<std::string> VEC_STR;
  typedef std::vector<bool> VEC_BOL;
  typedef std::vector<int> VEC_INT;

  class Util {
  public:
    static void stringsToLowercase(VEC_STR *vecStr);
    static void cutVector(VEC_STR *vecStr, int index);
    static void cleanDuplicateStrings(VEC_STR *vecStr);
    static void cleanDuplicatesToName(VEC_STR *vecStr, std::sdtring *name);
  };
}

#endif // __CLI_MENU__UTIL_H__