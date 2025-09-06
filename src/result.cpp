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

  void Result::addWord(
    mt::CR_STR keyword, mt::CR_STR input
  ) {
    if (hasWords(keyword)) {
      words[keyword].push_back(input);
    }
  }

  void Result::addNumber(
    mt::CR_STR keyword, mt::CR_LD input
  ) {
    if (hasNumbers(keyword)) {
      numbers[keyword].push_back(input);
    }
  }

  void Result::addToggle(
    mt::CR_STR keyword, mt::CR_BOL input
  ) {
    if (hasToggles(keyword)) {
      toggles[keyword].push_back(input);
    }
  }

  void Result::addUltimate(mt::CR_STR keyword, mt::CR_STR input) {
    if (hasUltimates(keyword)) {
      ultimates[keyword].push_back(input);
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

  void Result::removeUltimate(mt::CR_STR keyword) {
    if (hasUltimates(keyword)) {
      ultimates.erase(keyword);
    }
  }

  size_t Result::numberOfWords(mt::CR_STR keyword) {
    if (hasWords(keyword)) {
      return words[keyword].size();
    }
    return 0;
  }

  size_t Result::numberOfNumbers(mt::CR_STR keyword) {
    if (hasNumbers(keyword)) {
      return numbers[keyword].size();
    }
    return 0;
  }

  size_t Result::numberOfToggles(mt::CR_STR keyword) {
    if (hasToggles(keyword)) {
      return toggles[keyword].size();
    }
    return 0;
  }

  size_t Result::numberOfUltimates(mt::CR_STR keyword) {
    if (hasUltimates(keyword)) {
      return ultimates[keyword].size();
    }
    return 0;
  }

  std::string Result::getWordAt(mt::CR_STR keyword, mt::CR_SZ index) {
    if (hasWords(keyword) &&
      index < words[keyword].size()
    ) {
      return words[keyword][index];
    }
    return "";
  }

  mt::LD Result::getNumberAt(mt::CR_STR keyword, mt::CR_SZ index) {
    if (hasNumbers(keyword) &&
      index < numbers[keyword].size()
    ) {
      return numbers[keyword][index];
    }
    return 0;
  }

  bool Result::getToggleAt(mt::CR_STR keyword, mt::CR_SZ index) {
    if (hasToggles(keyword) &&
      index < toggles[keyword].size()
    ) {
      return toggles[keyword][index];
    }
    return false;
  }

  std::string Result::getUltimateAt(mt::CR_STR keyword, mt::CR_SZ index) {
    if (hasUltimates(keyword) &&
      index < ultimates[keyword].size()
    ) {
      return ultimates[keyword][index];
    }
    return "";
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
      if (toggles[keyword].empty()) return false;
      return toggles[keyword].front();
    }
    return false;
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
      if (toggles[keyword].empty()) return false;
      return toggles[keyword].back();
    }
    return false;
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
    std::string text;

    for (mt::CR_STR str : ultimates[keyword]) {
      text += str + separator;
    }

    return text;
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