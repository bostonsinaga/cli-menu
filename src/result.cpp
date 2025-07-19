#ifndef __CLI_MENU__RESULT_CPP__
#define __CLI_MENU__RESULT_CPP__

#include "result.hpp"

namespace cli_menu {

  void Result::addWord(
    mt::CR_STR keyword,
    mt::CR_STR input
  ) {
    words[keyword].push_back(input);
  }

  void Result::addNumber(
    mt::CR_STR keyword,
    mt::CR_LD input
  ) {
    numbers[keyword].push_back(input);
  }

  void Result::addToggle(
    mt::CR_STR keyword,
    mt::CR_BOL input
  ) {
    toggles[keyword].push_back(input);
  }

  void Result::removeWord(mt::CR_STR keyword) {
    words.erase(keyword);
  }

  void Result::removeNumber(mt::CR_STR keyword) {
    numbers.erase(keyword);
  }

  void Result::removeToggle(mt::CR_STR keyword) {
    toggles.erase(keyword);
  }

  void Result::printInputs() {
    std::cout << std::endl;

    printType<std::string>(
      Language::getStringifiedType(LANGUAGE_WORD_STRINGIFIED_TYPE),
      words
    );

    printType<mt::LD>(
      Language::getStringifiedType(LANGUAGE_NUMBER_STRINGIFIED_TYPE),
      numbers
    );

    printType<bool>(
      Language::getStringifiedType(LANGUAGE_TOGGLE_STRINGIFIED_TYPE),
      toggles
    );
  }
}

#endif // __CLI_MENU__RESULT_CPP__