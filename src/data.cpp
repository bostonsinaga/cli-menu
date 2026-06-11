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

  bool Data::Input::hasWords(Command *command) {
    return mt::UNORMAP_FOUND<Command*, mt::VEC_STR>(words, command);
  }

  bool Data::Input::hasNumbers(Command *command) {
    return mt::UNORMAP_FOUND<Command*, mt::VEC_LD>(numbers, command);
  }

  bool Data::Input::hasBooleans(Command *command) {
    return mt::UNORMAP_FOUND<Command*, mt::VEC_BOL>(booleans, command);
  }

  void Data::Input::pushWord(Command *command, mt::CR_STR input) {
    words[command].push_back(input);
  }

  void Data::Input::pushNumber(Command *command, mt::CR_LD input) {
    numbers[command].push_back(input);
  }

  void Data::Input::pushBoolean(Command *command, mt::CR_BOL input) {
    booleans[command].push_back(input);
  }

  void Data::Input::popWord(Command *command) {
    if (hasWords(command)) {
      words[command].pop_back();
    }
  }

  void Data::Input::popNumber(Command *command) {
    if (hasNumbers(command)) {
      numbers[command].pop_back();
    }
  }

  void Data::Input::popBoolean(Command *command) {
    if (hasBooleans(command)) {
      booleans[command].pop_back();
    }
  }

  void Data::Input::eraseWords(Command *command) {
    words.erase(command);
  }

  void Data::Input::eraseNumbers(Command *command) {
    numbers.erase(command);
  }

  void Data::Input::eraseBooleans(Command *command) {
    booleans.erase(command);
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

  size_t Data::Input::numberOfWords(Command *command) {
    if (hasWords(command)) {
      return words[command].size();
    }
    return 0;
  }

  size_t Data::Input::numberOfNumbers(Command *command) {
    if (hasNumbers(command)) {
      return numbers[command].size();
    }
    return 0;
  }

  size_t Data::Input::numberOfBooleans(Command *command) {
    if (hasBooleans(command)) {
      return booleans[command].size();
    }
    return 0;
  }

  std::string Data::Input::getWordAt(Command *command, mt::CR_SZ index) {
    if (hasWords(command) &&
      index < words[command].size()
    ) {
      return words[command][index];
    }
    return "";
  }

  mt::LD Data::Input::getNumberAt(Command *command, mt::CR_SZ index) {
    if (hasNumbers(command) &&
      index < numbers[command].size()
    ) {
      return numbers[command][index];
    }
    return 0;
  }

  bool Data::Input::getBooleanAt(Command *command, mt::CR_SZ index) {
    if (hasBooleans(command) &&
      index < booleans[command].size()
    ) {
      return booleans[command][index];
    }
    return false;
  }

  std::string Data::Input::getFirstWord(Command *command) {
    if (hasWords(command)) {
      if (words[command].empty()) return "";
      return words[command].front();
    }
    return "";
  }

  mt::LD Data::Input::getFirstNumber(Command *command) {
    if (hasNumbers(command)) {
      if (numbers[command].empty()) return 0;
      return numbers[command].front();
    }
    return 0;
  }

  bool Data::Input::getFirstBoolean(Command *command) {
    if (hasBooleans(command)) {
      if (booleans[command].empty()) return false;
      return booleans[command].front();
    }
    return false;
  }

  std::string Data::Input::getLastWord(Command *command) {
    if (hasWords(command)) {
      if (words[command].empty()) return "";
      return words[command].back();
    }
    return "";
  }

  mt::LD Data::Input::getLastNumber(Command *command) {
    if (hasNumbers(command)) {
      if (numbers[command].empty()) return 0;
      return numbers[command].back();
    }
    return 0;
  }

  bool Data::Input::getLastBoolean(Command *command) {
    if (hasBooleans(command)) {
      if (booleans[command].empty()) return false;
      return booleans[command].back();
    }
    return false;
  }

  //________|
  // OUTPUT |
  //________|

  bool Data::Output::has(Command *command) {
    return mt::UNORMAP_FOUND<Command*, mt::VEC_STR>(texts, command);
  }

  void Data::Output::push(Command *command, mt::CR_STR input) {
    texts[command].push_back(input);
  }

  void Data::Output::pop(Command *command) {
    if (has(command)) {
      texts[command].pop_back();
    }
  }

  void Data::Output::erase(Command *command) {
    texts.erase(command);
  }

  bool Data::Output::clearAll() {
    if (texts.size()) {
      texts.clear();
      return true;
    }
    return false;
  }

  size_t Data::Output::numberOf(Command *command) {
    if (has(command)) {
      return texts[command].size();
    }
    return 0;
  }

  std::string Data::Output::getAt(Command *command, mt::CR_SZ index) {
    if (has(command) &&
      index < texts[command].size()
    ) {
      return texts[command][index];
    }
    return "";
  }

  std::string Data::Output::getFirst(Command *command) {
    if (has(command)) {
      if (texts[command].empty()) return "";
      return texts[command].front();
    }
    return "";
  }

  std::string Data::Output::getLast(Command *command) {
    if (has(command)) {
      if (texts[command].empty()) return "";
      return texts[command].back();
    }
    return "";
  }

  std::string Data::Output::concat(
    Command *command,
    mt::CR_STR separator
  ) {
    std::string retstr;

    if (has(command)) {
      for (int i = 0; i < texts[command].size() - 1; i++) {
        retstr += texts[command][i] + separator;
      }

      if (!texts[command].empty()) {
        retstr += texts[command].back();
      }
    }

    return retstr;
  }
}

#endif // __CLI_MENU__DATA_CPP__