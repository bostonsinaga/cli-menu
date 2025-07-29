#ifndef __CLI_MENU__RESULT_CPP__
#define __CLI_MENU__RESULT_CPP__

#include "result.hpp"

namespace cli_menu {

  bool Result::hasWords(mt::CR_STR keyword) {
    return mt::STRUNORMAP_FOUND<mt::VEC_STR>(words, keyword);
  }

  bool Result::hasNumbers(mt::CR_STR keyword) {
    return mt::STRUNORMAP_FOUND<mt::VEC_LD>(numbers, keyword);
  }

  bool Result::hasToggles(mt::CR_STR keyword) {
    return mt::STRUNORMAP_FOUND<mt::VEC_BOL>(toggles, keyword);
  }

  void Result::addWords(
    mt::CR_STR keyword, mt::CR_STR input
  ) {
    if (hasWords(keyword)) {
      words[keyword].push_back(input);
    }
  }

  void Result::addNumbers(
    mt::CR_STR keyword, mt::CR_LD input
  ) {
    if (hasNumbers(keyword)) {
      numbers[keyword].push_back(input);
    }
  }

  void Result::addToggles(
    mt::CR_STR keyword, mt::CR_BOL input
  ) {
    if (hasToggles(keyword)) {
      toggles[keyword].push_back(input);
    }
  }

  void Result::removeWords(mt::CR_STR keyword) {
    if (hasWords(keyword)) {
      words.erase(keyword);
    }
  }

  void Result::removeNumbers(mt::CR_STR keyword) {
    if (hasNumbers(keyword)) {
      numbers.erase(keyword);
    }
  }

  void Result::removeToggles(mt::CR_STR keyword) {
    if (hasToggles(keyword)) {
      toggles.erase(keyword);
    }
  }

  mt::VEC_STR &Result::getWords(mt::CR_STR keyword) {
    if (hasWords(keyword)) {
      return words[keyword];
    }
    return wordsGarbage;
  }

  mt::VEC_LD &Result::getNumbers(mt::CR_STR keyword) {
    if (hasNumbers(keyword)) {
      return numbers[keyword];
    }
    return numbersGarbage;
  }

  mt::VEC_BOL &Result::getToggles(mt::CR_STR keyword) {
    if (hasToggles(keyword)) {
      return toggles[keyword];
    }
    return togglesGarbage;
  }

  std::string &Result::getUltimate(mt::CR_STR keyword) {
    if (mt::STRUNORMAP_STR_FOUND(ultimate, keyword)) {
      return ultimate[keyword];
    }
    return ultimateGarbage;
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