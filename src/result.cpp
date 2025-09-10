#ifndef __CLI_MENU__RESULT_CPP__
#define __CLI_MENU__RESULT_CPP__

#include "result.hpp"

namespace cli_menu {

  bool Result::hasWords(Command *node) {
    return mt::UNORMAP_FOUND<Command*, mt::VEC_STR>(words, node);
  }

  bool Result::hasNumbers(Command *node) {
    return mt::UNORMAP_FOUND<Command*, mt::VEC_LD>(numbers, node);
  }

  bool Result::hasToggles(Command *node) {
    return mt::UNORMAP_FOUND<Command*, mt::VEC_BOL>(toggles, node);
  }

  bool Result::hasUltimates(Command *node) {
    return mt::UNORMAP_FOUND<Command*, mt::VEC_STR>(ultimates, node);
  }

  void Result::pushWord(Command *node, mt::CR_STR input) {
    if (hasWords(node)) {
      words[node].push_back(input);
    }
  }

  void Result::pushNumber(Command *node, mt::CR_LD input) {
    if (hasNumbers(node)) {
      numbers[node].push_back(input);
    }
  }

  void Result::pushToggle(Command *node, mt::CR_BOL input) {
    if (hasToggles(node)) {
      toggles[node].push_back(input);
    }
  }

  void Result::pushUltimate(Command *node, mt::CR_STR input) {
    if (hasUltimates(node)) {
      ultimates[node].push_back(input);
    }
  }

  void Result::popWord(Command *node) {
    if (hasWords(node)) {
      words[node].pop_back();
    }
  }

  void Result::popNumber(Command *node) {
    if (hasNumbers(node)) {
      numbers[node].pop_back();
    }
  }

  void Result::popToggle(Command *node) {
    if (hasToggles(node)) {
      toggles[node].pop_back();
    }
  }

  void Result::popUltimate(Command *node) {
    if (hasUltimates(node)) {
      ultimates[node].pop_back();
    }
  }

  void Result::eraseWords(Command *node) {
    if (hasWords(node)) {
      words.erase(node);
    }
  }

  void Result::eraseNumbers(Command *node) {
    if (hasNumbers(node)) {
      numbers.erase(node);
    }
  }

  void Result::eraseToggles(Command *node) {
    if (hasToggles(node)) {
      toggles.erase(node);
    }
  }

  void Result::eraseUltimates(Command *node) {
    if (hasUltimates(node)) {
      ultimates.erase(node);
    }
  }

  void Result::clearWords() {
    words.clear();
  }

  void Result::clearNumbers() {
    numbers.clear();
  }

  void Result::clearToggles() {
    toggles.clear();
  }

  void Result::clearUltimates() {
    ultimates.clear();
  }

  void Result::clearAll() {
    words.clear();
    numbers.clear();
    toggles.clear();
    ultimates.clear();
  }

  size_t Result::numberOfWords(Command *node) {
    if (hasWords(node)) {
      return words[node].size();
    }
    return 0;
  }

  size_t Result::numberOfNumbers(Command *node) {
    if (hasNumbers(node)) {
      return numbers[node].size();
    }
    return 0;
  }

  size_t Result::numberOfToggles(Command *node) {
    if (hasToggles(node)) {
      return toggles[node].size();
    }
    return 0;
  }

  size_t Result::numberOfUltimates(Command *node) {
    if (hasUltimates(node)) {
      return ultimates[node].size();
    }
    return 0;
  }

  std::string Result::getWordAt(Command *node, mt::CR_SZ index) {
    if (hasWords(node) &&
      index < words[node].size()
    ) {
      return words[node][index];
    }
    return "";
  }

  mt::LD Result::getNumberAt(Command *node, mt::CR_SZ index) {
    if (hasNumbers(node) &&
      index < numbers[node].size()
    ) {
      return numbers[node][index];
    }
    return 0;
  }

  bool Result::getToggleAt(Command *node, mt::CR_SZ index) {
    if (hasToggles(node) &&
      index < toggles[node].size()
    ) {
      return toggles[node][index];
    }
    return false;
  }

  std::string Result::getUltimateAt(Command *node, mt::CR_SZ index) {
    if (hasUltimates(node) &&
      index < ultimates[node].size()
    ) {
      return ultimates[node][index];
    }
    return "";
  }

  std::string Result::getFirstWord(Command *node) {
    if (hasWords(node)) {
      if (words[node].empty()) return "";
      return words[node].front();
    }
    return "";
  }

  mt::LD Result::getFirstNumber(Command *node) {
    if (hasNumbers(node)) {
      if (numbers[node].empty()) return 0;
      return numbers[node].front();
    }
    return 0;
  }

  bool Result::getFirstToggle(Command *node) {
    if (hasToggles(node)) {
      if (toggles[node].empty()) return false;
      return toggles[node].front();
    }
    return false;
  }

  std::string Result::getFirstUltimate(Command *node) {
    if (hasUltimates(node)) {
      if (ultimates[node].empty()) return "";
      return ultimates[node].front();
    }
    return "";
  }

  std::string Result::getLastWord(Command *node) {
    if (hasWords(node)) {
      if (words[node].empty()) return "";
      return words[node].back();
    }
    return "";
  }

  mt::LD Result::getLastNumber(Command *node) {
    if (hasNumbers(node)) {
      if (numbers[node].empty()) return 0;
      return numbers[node].back();
    }
    return 0;
  }

  bool Result::getLastToggle(Command *node) {
    if (hasToggles(node)) {
      if (toggles[node].empty()) return false;
      return toggles[node].back();
    }
    return false;
  }

  std::string Result::getLastUltimate(Command *node) {
    if (hasUltimates(node)) {
      if (ultimates[node].empty()) return "";
      return ultimates[node].back();
    }
    return "";
  }

  std::string Result::concatUltimates(
    Command *node,
    mt::CR_STR separator
  ) {
    std::string text;

    for (mt::CR_STR str : ultimates[node]) {
      text += str + separator;
    }

    return text;
  }
}

#endif // __CLI_MENU__RESULT_CPP__