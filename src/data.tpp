#ifndef __CLI_MENU__DATA_TPP__
#define __CLI_MENU__DATA_TPP__

namespace cli_menu {

  template <PRIMITIVE_TYPE T>
  std::string Data::stringifiedVectorMember(mt::CR<T> vecmem) {
    std::string stringified;

    if constexpr (WORD_TYPE<T>) {
      stringified = vecmem;
    }
    else if constexpr (NUMBER_TYPE<T>) {
      stringified = std::to_string(vecmem);
    }
    else { // boolean
      if (vecmem) stringified = "true";
      else stringified = "false";
    }

    return stringified;
  }

  template <PRIMITIVE_TYPE T>
  void Data::printVector(mt::CR_VEC<T> vec, mt::CR_BOL withIndent) {

    for (int i = 0; i < vec.size(); i++) {
      Console::logString(
        (withIndent ? "  " : "") + stringifiedVectorMember<T>(vec[i])
        + (i == vec.size() - 1 ? "\n" : ",\n"),
        Console::messageColors[withIndent ? CONSOLE_HINT_3 : CONSOLE_HINT_2]
      );
    }
  }

  template <PRIMITIVE_TYPE T>
  void Data::printType(
    mt::CR_STR stringifiedType,
    COMUNORMAP<T> &unormap
  ) {
    // type title
    Console::logString(
      stringifiedType + ":\n",
      Console::messageColors[CONSOLE_HINT_1]
    );

    for (mt::CR_PAIR2<Command*, mt::VEC<T>> keyvec : unormap) {

      // keyword with size of vector
      Console::logItalicString(
        keyvec.first->getKeyword() + " (" + std::to_string(keyvec.second.size()) + "):\n",
        Console::messageColors[CONSOLE_HINT_2]
      );

      // members of vector
      printVector<T>(keyvec.second, true);
    }

    // display emptiness
    if (unormap.empty()) {
      Console::logString("...\n", Console::messageColors[CONSOLE_HINT_2]);
    }

    std::cout << std::endl;
  }

  template <PRIMITIVE_TYPE T>
  void Data::Input::printVector(Command *node, mt::CR_BOL withIndent) {

    if constexpr (WORD_TYPE<T>) {
      Data::printVector<std::string>(words[node], withIndent);
    }
    else if constexpr (NUMBER_TYPE<T>) {
      Data::printVector<mt::LD>(numbers[node], withIndent);
    }
    else { // boolean
      Data::printVector<bool>(booleans[node], withIndent);
    }
  }
}

#endif // __CLI_MENU__DATA_TPP__