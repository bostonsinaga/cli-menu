#ifndef __CLI_MENU__RESULT_TPP__
#define __CLI_MENU__RESULT_TPP__

namespace cli_menu {

  template <typename T>
  std::string Result::stringifiedVectorMember(mt::CR<T> vecmem) {
    std::string stringified;

    if constexpr (std::is_same_v<T, std::string>) {
      stringified = vecmem;
    }
    else if constexpr (std::is_same_v<T, mt::LD>) {
      stringified = std::to_string(vecmem);
    }
    else if constexpr (std::is_same_v<T, bool>) {
      if (vecmem) stringified = "true";
      else stringified = "false";
    }

    return stringified;
  }

  template <typename T>
  void Result::printType(
    mt::CR_STR stringifiedType,
    mt::STRUNORMAP<mt::VEC<T>> &unormap
  ) {
    if constexpr (
      std::is_same_v<T, std::string> ||
      std::is_same_v<T, mt::LD> ||
      std::is_same_v<T, bool>
    ) {
      // type title
      Console::logString(
        stringifiedType + ":\n",
        Console::messageColors[CONSOLE_HINT_1]
      );

      for (mt::CR_PAIR2<std::string, mt::VEC<T>> keyvec : unormap) {

        // keyword with size of vector
        Console::logItalicString(
          keyvec.first + " (" + std::to_string(keyvec.second.size()) + "):\n",
          Console::messageColors[CONSOLE_HINT_2]
        );

        // members of vector
        for (int i = 0; i < keyvec.second.size(); i++) {
          Console::logString(
            "  " + stringifiedVectorMember<T>(keyvec.second[i])
            + (i == keyvec.second.size() - 1 ? "\n" : ",\n"),
            Console::messageColors[CONSOLE_HINT_3]
          );
        }
      }

      // display emptiness
      if (unormap.empty()) {
        Console::logString("...\n", Console::messageColors[CONSOLE_HINT_2]);
      }

      std::cout << std::endl;
    }
  }
}

#endif // __CLI_MENU__RESULT_TPP__