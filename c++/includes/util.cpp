#ifndef __CLI_MENU__UTIL_CPP__
#define __CLI_MENU__UTIL_CPP__

#include "util.h"

namespace cli_menu {

  void Util::changeStringCase(std::string &str, bool isUpper) {
    std::transform(
      str.begin(),
      str.end(),
      str.begin(),
      [&](unsigned char c) {
        if (isUpper) return std::toupper(c);
        else return std::tolower(c);
      }
    );
  }

  void Util::changeStringsCase(std::vector<std::string> *vecStr, bool isUpper) {
    for (int i = 0; i < vecStr->size(); i++) {
      changeStringCase(vecStr->at(i), isUpper);
    }
  }

  std::vector<LLUI> Util::getDifferencesToSize(
    std::vector<LLUI> sizes,
    LLUI targetSize
  ) {
    LLUI max = targetSize;

    for (auto &sz : sizes) {
      if (targetSize > 0) {
        if (sz > max && sz <= targetSize) max = sz;
      }
      else if (sz > max) max = sz;
    }

    std::vector<LLUI> differences;
    for (auto &sz : sizes) { differences.push_back(max - sz); }
    
    return differences;
  }

  void Util::changeStringToLowercase(std::string &str) {
    changeStringCase(str, false);
  }

  void Util::changeStringToUppercase(std::string &str) {
    changeStringCase(str, true);
  }

  void Util::getStringToLowercase(std::string str) {
    changeStringCase(str, false);
    return str;
  }

  void Util::getStringToUppercase(std::string str) {
    changeStringCase(str, true);
    return str;
  }

  void Util::stringsToLowercase(VEC_STR *vecStr) {
    changeStringsCase(vecStr, false);
  }

  void Util::stringsToUppercase(VEC_STR *vecStr) {
    changeStringsCase(vecStr, true);
  }
}

#endif // __CLI_MENU__UTIL_CPP__