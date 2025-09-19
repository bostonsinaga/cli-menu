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

  bool Result::hasBooleans(Command *node) {
    return mt::UNORMAP_FOUND<Command*, mt::VEC_BOL>(booleans, node);
  }

  bool Result::hasOutputs(Command *node) {
    return mt::UNORMAP_FOUND<Command*, mt::VEC_STR>(outputs, node);
  }

  void Result::pushWord(Command *node, mt::CR_STR input) {
    words[node].push_back(input);
  }

  void Result::pushNumber(Command *node, mt::CR_LD input) {
    numbers[node].push_back(input);
  }

  void Result::pushBoolean(Command *node, mt::CR_BOL input) {
    booleans[node].push_back(input);
  }

  void Result::pushOutput(Command *node, mt::CR_STR input) {
    outputs[node].push_back(input);
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

  void Result::popBoolean(Command *node) {
    if (hasBooleans(node)) {
      booleans[node].pop_back();
    }
  }

  void Result::popOutput(Command *node) {
    if (hasOutputs(node)) {
      outputs[node].pop_back();
    }
  }

  void Result::eraseWords(Command *node) {
    words.erase(node);
  }

  void Result::eraseNumbers(Command *node) {
    numbers.erase(node);
  }

  void Result::eraseBooleans(Command *node) {
    booleans.erase(node);
  }

  void Result::eraseOutputs(Command *node) {
    outputs.erase(node);
  }

  void Result::clearWords() {
    words.clear();
  }

  void Result::clearNumbers() {
    numbers.clear();
  }

  void Result::clearBooleans() {
    booleans.clear();
  }

  void Result::clearOutputs() {
    outputs.clear();
  }

  void Result::clearAll() {
    words.clear();
    numbers.clear();
    booleans.clear();
    outputs.clear();
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

  size_t Result::numberOfBooleans(Command *node) {
    if (hasBooleans(node)) {
      return booleans[node].size();
    }
    return 0;
  }

  size_t Result::numberOfOutputs(Command *node) {
    if (hasOutputs(node)) {
      return outputs[node].size();
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

  bool Result::getBooleanAt(Command *node, mt::CR_SZ index) {
    if (hasBooleans(node) &&
      index < booleans[node].size()
    ) {
      return booleans[node][index];
    }
    return false;
  }

  std::string Result::getOutputAt(Command *node, mt::CR_SZ index) {
    if (hasOutputs(node) &&
      index < outputs[node].size()
    ) {
      return outputs[node][index];
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

  bool Result::getFirstBoolean(Command *node) {
    if (hasBooleans(node)) {
      if (booleans[node].empty()) return false;
      return booleans[node].front();
    }
    return false;
  }

  std::string Result::getFirstOutput(Command *node) {
    if (hasOutputs(node)) {
      if (outputs[node].empty()) return "";
      return outputs[node].front();
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

  bool Result::getLastBoolean(Command *node) {
    if (hasBooleans(node)) {
      if (booleans[node].empty()) return false;
      return booleans[node].back();
    }
    return false;
  }

  std::string Result::getLastOutput(Command *node) {
    if (hasOutputs(node)) {
      if (outputs[node].empty()) return "";
      return outputs[node].back();
    }
    return "";
  }

  std::string Result::concatOutputs(
    Command *node,
    mt::CR_STR separator
  ) {
    std::string text;

    if (hasOutputs(node)) {
      for (int i = 0; i < outputs[node].size() - 1; i++) {
        text += outputs[node][i] + separator;
      }

      if (!outputs[node].empty()) {
        text += outputs[node].back();
      }
    }

    return text;
  }
}

#endif // __CLI_MENU__RESULT_CPP__