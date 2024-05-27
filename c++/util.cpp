#ifndef __CLI_MENU__UTIL_CPP__
#define __CLI_MENU__UTIL_CPP__

#include "util.h"

namespace cli_menu {
  void Util::stringsToLowercase(VEC_STR *vecStr) {
    for (int i = 0; i < vecStr->size(); i++) {
      vecStr->at(i) = std::toLower(vecStr->at(i));
    }
  }

  void Util::cutVector(VEC_STR *vecStr, int index) {
    VEC_STR tail = VEC_STR(vecStr->begin() + index + 1, vecStr->end());
    *vecStr = VEC_STR(vecStr->begin(), vecStr->begin() + index);
    vecStr->insert(vecStr->end(), tail.begin(), tail.end());
  }

  void Util::cleanDuplicateStrings(VEC_STR *vecStr) {
    for (int i = 0; i < vecStr->size(); i++) {
      for (int j = 0; j < vecStr->size(); j++) {
        if (i != j && vecStr->at(i) == vecStr->at(j)) {
          cutVector(vecStr, j);
          j--;
        }
      }
    }
  }

  void Util::cleanDuplicatesToName(VEC_STR *vecStr, std::string name) {
    bool first = false;
    for (int i = 0; i < vecStr->size(); i++) {
      if (vecStr->at(i) == name) {
        if (first) cutVector(vecStr, i);
        else first = true;
      }
    }
  }
}

#endif // __CLI_MENU__UTIL_CPP__