#ifndef __CLI_MENU__RESULT_INPUTS_TPP__
#define __CLI_MENU__RESULT_INPUTS_TPP__

#include "result-inputs.hpp"

namespace cli_menu {

  template <Console::LIMITED_TYPE T>
  std::string Result::stringifiedVectorMember(T &vecmem) {
    std::string stringified;

    if constexpr (std::is_same_v<T, bool>) {
      if (vecmem) stringified = "true";
      else stringified = "false";
    }
    else if constexpr (std::is_same_v<T, mt::LD>) {
      stringified = std::to_string(vecmem);
    }
    else stringified = vecmem;

    return stringified;
  }

  template <Console::LIMITED_TYPE T>
  void Result::printType(
    mt::CR_STR stringifiedType,
    mt::STRUNORMAP<mt::VEC<T>> &unormap
  ) {
    // type title
    Console::logResponse(Console::HINT, stringifiedType + ":\n");

    for (mt::CR_PAIR2<std::string, mt::VEC<T>> keyvec : unormap) {
      int i = 0;

      // keyword with size of vector
      std::cout << "  " << keyvec.first << '(' << keyvec.second.size() << "):\n";

      // members of vector
      for (; i < keyvec.second.size() - 1; i++) {
        std::cout << "    " << stringifiedVectorMember<T>(keyvec.second[i]) << ",\n";
      }

      // the last member
      std::cout << "    " << stringifiedVectorMember<T>(keyvec.second[i]) << std::endl;
    }
  }
}

#endif // __CLI_MENU__RESULT_INPUTS_TPP__