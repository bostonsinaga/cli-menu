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

  bool Result::hasUltimates(mt::CR_STR keyword) {
    return mt::STRUNORMAP_FOUND<mt::VEC_STR>(ultimates, keyword);
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

  mt::VEC_STR &Result::useWords(mt::CR_STR keyword) {
    if (hasWords(keyword)) {
      return words[keyword];
    }
    return wordsGarbage;
  }

  mt::VEC_LD &Result::useNumbers(mt::CR_STR keyword) {
    if (hasNumbers(keyword)) {
      return numbers[keyword];
    }
    return numbersGarbage;
  }

  mt::VEC_BOL &Result::useToggles(mt::CR_STR keyword) {
    if (hasToggles(keyword)) {
      return toggles[keyword];
    }
    return togglesGarbage;
  }

  mt::VEC_STR &Result::useUltimates(mt::CR_STR keyword) {
    if (hasUltimates(keyword)) {
      return ultimates[keyword];
    }
    return ultimatesGarbage;
  }

  std::string Result::getFirstWord(mt::CR_STR keyword) {
    if (hasWords(keyword)) {
      if (words[keyword].empty()) return "";
      return words[keyword].front();
    }
    return "";
  }

  mt::LD Result::getFirstNumber(mt::CR_STR keyword) {
    if (hasNumbers(keyword)) {
      if (numbers[keyword].empty()) return 0;
      return numbers[keyword].front();
    }
    return 0;
  }

  bool Result::getFirstToggle(mt::CR_STR keyword) {
    if (hasToggles(keyword)) {
      if (toggles[keyword].empty()) return "";
      return toggles[keyword].front();
    }
    return "";
  }

  std::string Result::getFirstUltimate(mt::CR_STR keyword) {
    if (hasUltimates(keyword)) {
      if (ultimates[keyword].empty()) return "";
      return ultimates[keyword].front();
    }
    return "";
  }

  std::string Result::getLastWord(mt::CR_STR keyword) {
    if (hasWords(keyword)) {
      if (words[keyword].empty()) return "";
      return words[keyword].back();
    }
    return "";
  }

  mt::LD Result::getLastNumber(mt::CR_STR keyword) {
    if (hasNumbers(keyword)) {
      if (numbers[keyword].empty()) return 0;
      return numbers[keyword].back();
    }
    return 0;
  }

  bool Result::getLastToggle(mt::CR_STR keyword) {
    if (hasToggles(keyword)) {
      if (toggles[keyword].empty()) return "";
      return toggles[keyword].back();
    }
    return "";
  }

  std::string Result::getLastUltimate(mt::CR_STR keyword) {
    if (hasUltimates(keyword)) {
      if (ultimates[keyword].empty()) return "";
      return ultimates[keyword].back();
    }
    return "";
  }

  std::string Result::concatUltimates(
    mt::CR_STR keyword,
    mt::CR_STR separator
  ) {
    std::string data;

    for (mt::CR_STR str : Result::useUltimates(keyword)) {
      data += str + separator;
    }

    return data;
  }

  void Result::printInputs() {
    std::cout << std::endl;

    printType<std::string>(
      Langu::ageCommand::getStringifiedType(STRINGIFIED_TYPE_WORD),
      words
    );

    printType<mt::LD>(
      Langu::ageCommand::getStringifiedType(STRINGIFIED_TYPE_NUMBER),
      numbers
    );

    printType<bool>(
      Langu::ageCommand::getStringifiedType(STRINGIFIED_TYPE_TOGGLE),
      toggles
    );
  }
}

#endif // __CLI_MENU__RESULT_CPP__