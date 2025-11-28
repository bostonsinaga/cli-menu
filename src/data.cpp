#ifndef __CLI_MENU__DATA_CPP__
#define __CLI_MENU__DATA_CPP__

#include "data.hpp"

namespace cli_menu {

  bool Data::clearAll() {
    return Input::clearAll() || Output::clearAll();
  }

  //_______|
  // INPUT |
  //_______|

  bool Data::Input::hasWords(Command *node) {
    return mt::UNORMAP_FOUND<Command*, mt::VEC_STR>(words, node);
  }

  bool Data::Input::hasNumbers(Command *node) {
    return mt::UNORMAP_FOUND<Command*, mt::VEC_LD>(numbers, node);
  }

  bool Data::Input::hasBooleans(Command *node) {
    return mt::UNORMAP_FOUND<Command*, mt::VEC_BOL>(booleans, node);
  }

  void Data::Input::pushWord(Command *node, mt::CR_STR input) {
    words[node].push_back(input);
  }

  void Data::Input::pushNumber(Command *node, mt::CR_LD input) {
    numbers[node].push_back(input);
  }

  void Data::Input::pushBoolean(Command *node, mt::CR_BOL input) {
    booleans[node].push_back(input);
  }

  void Data::Input::popWord(Command *node) {
    if (hasWords(node)) {
      words[node].pop_back();
    }
  }

  void Data::Input::popNumber(Command *node) {
    if (hasNumbers(node)) {
      numbers[node].pop_back();
    }
  }

  void Data::Input::popBoolean(Command *node) {
    if (hasBooleans(node)) {
      booleans[node].pop_back();
    }
  }

  void Data::Input::eraseWords(Command *node) {
    words.erase(node);
  }

  void Data::Input::eraseNumbers(Command *node) {
    numbers.erase(node);
  }

  void Data::Input::eraseBooleans(Command *node) {
    booleans.erase(node);
  }

  bool Data::Input::clearWords() {
    if (words.size()) {
      words.clear();
      return true;
    }
    return false;
  }

  bool Data::Input::clearNumbers() {
    if (numbers.size()) {
      numbers.clear();
      return true;
    }
    return false;
  }

  bool Data::Input::clearBooleans() {
    if (booleans.size()) {
      booleans.clear();
      return true;
    }
    return false;
  }

  bool Data::Input::clearAll() {
    return clearWords() || clearNumbers() || clearBooleans();
  }

  size_t Data::Input::numberOfWords(Command *node) {
    if (hasWords(node)) {
      return words[node].size();
    }
    return 0;
  }

  size_t Data::Input::numberOfNumbers(Command *node) {
    if (hasNumbers(node)) {
      return numbers[node].size();
    }
    return 0;
  }

  size_t Data::Input::numberOfBooleans(Command *node) {
    if (hasBooleans(node)) {
      return booleans[node].size();
    }
    return 0;
  }

  std::string Data::Input::getWordAt(Command *node, mt::CR_SZ index) {
    if (hasWords(node) &&
      index < words[node].size()
    ) {
      return words[node][index];
    }
    return "";
  }

  mt::LD Data::Input::getNumberAt(Command *node, mt::CR_SZ index) {
    if (hasNumbers(node) &&
      index < numbers[node].size()
    ) {
      return numbers[node][index];
    }
    return 0;
  }

  bool Data::Input::getBooleanAt(Command *node, mt::CR_SZ index) {
    if (hasBooleans(node) &&
      index < booleans[node].size()
    ) {
      return booleans[node][index];
    }
    return false;
  }

  std::string Data::Input::getFirstWord(Command *node) {
    if (hasWords(node)) {
      if (words[node].empty()) return "";
      return words[node].front();
    }
    return "";
  }

  mt::LD Data::Input::getFirstNumber(Command *node) {
    if (hasNumbers(node)) {
      if (numbers[node].empty()) return 0;
      return numbers[node].front();
    }
    return 0;
  }

  bool Data::Input::getFirstBoolean(Command *node) {
    if (hasBooleans(node)) {
      if (booleans[node].empty()) return false;
      return booleans[node].front();
    }
    return false;
  }

  std::string Data::Input::getLastWord(Command *node) {
    if (hasWords(node)) {
      if (words[node].empty()) return "";
      return words[node].back();
    }
    return "";
  }

  mt::LD Data::Input::getLastNumber(Command *node) {
    if (hasNumbers(node)) {
      if (numbers[node].empty()) return 0;
      return numbers[node].back();
    }
    return 0;
  }

  bool Data::Input::getLastBoolean(Command *node) {
    if (hasBooleans(node)) {
      if (booleans[node].empty()) return false;
      return booleans[node].back();
    }
    return false;
  }

  //________|
  // OUTPUT |
  //________|

  bool Data::Output::has(Command *node) {
    return mt::UNORMAP_FOUND<Command*, mt::VEC_STR>(texts, node);
  }

  void Data::Output::push(Command *node, mt::CR_STR input) {
    texts[node].push_back(input);
  }

  void Data::Output::pop(Command *node) {
    if (has(node)) {
      texts[node].pop_back();
    }
  }

  void Data::Output::erase(Command *node) {
    texts.erase(node);
  }

  bool Data::Output::clearAll() {
    if (texts.size()) {
      texts.clear();
      return true;
    }
    return false;
  }

  size_t Data::Output::numberOf(Command *node) {
    if (has(node)) {
      return texts[node].size();
    }
    return 0;
  }

  std::string Data::Output::getAt(Command *node, mt::CR_SZ index) {
    if (has(node) &&
      index < texts[node].size()
    ) {
      return texts[node][index];
    }
    return "";
  }

  std::string Data::Output::getFirst(Command *node) {
    if (has(node)) {
      if (texts[node].empty()) return "";
      return texts[node].front();
    }
    return "";
  }

  std::string Data::Output::getLast(Command *node) {
    if (has(node)) {
      if (texts[node].empty()) return "";
      return texts[node].back();
    }
    return "";
  }

  std::string Data::Output::concat(
    Command *node,
    mt::CR_STR separator
  ) {
    std::string retstr;

    if (has(node)) {
      for (int i = 0; i < texts[node].size() - 1; i++) {
        retstr += texts[node][i] + separator;
      }

      if (!texts[node].empty()) {
        retstr += texts[node].back();
      }
    }

    return retstr;
  }
}

#endif // __CLI_MENU__DATA_CPP__