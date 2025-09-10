#ifndef __CLI_MENU__TYPE_CPP__
#define __CLI_MENU__TYPE_CPP__

#include "type.hpp"

namespace cli_menu {

  Creator::Creator(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in,
    COMMAND_CALLBACK callback_in
  ) : Command(
    keyword_in, description_in, callback_in
  ) {}

  void Creator::replaceExistingKeyword(Command *newCommand) {
    if (!sterilized) {
      Creator *willDestroyed = nullptr;

      if (getChildren()) getChildren()->iterate(
        mt_ds::LinkedList::RIGHT,
        [&](mt_ds::LinkedList *node)->bool {

          // find keyword
          if (static_cast<Creator*>(node)->keyword
            == static_cast<Creator*>(newCommand)->keyword
          ) {
            willDestroyed = static_cast<Creator*>(node);
            return false;
          }

          return true;
        }
      );

      if (willDestroyed) willDestroyed->destroy();
      addChild(newCommand);
    }
  }

  Word *Creator::createWord(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in,
    COMMAND_CALLBACK callback_in
  ) {
    Word *word = new Word(
      keyword_in,
      description_in,
      callback_in
    );

    replaceExistingKeyword(word);
    word->setPresetHelpList();
    return word;
  }

  Number *Creator::createNumber(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in,
    COMMAND_CALLBACK callback_in
  ) {
    Number *number = new Number(
      keyword_in,
      description_in,
      callback_in
    );

    replaceExistingKeyword(number);
    number->setPresetHelpList();
    return number;
  }

  Toggle *Creator::createToggle(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in,
    COMMAND_CALLBACK callback_in
  ) {
    Toggle *toggle = new Toggle(
      keyword_in,
      description_in,
      callback_in
    );

    replaceExistingKeyword(toggle);
    toggle->setPresetHelpList();
    return toggle;
  }

  /** WORD */

  Word::Word(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in,
    COMMAND_CALLBACK callback_in
  ) : Creator (
    keyword_in, description_in, callback_in
  ) {
    hyphens = "-";
    stringifiedTypeIndex = STRINGIFIED_TYPE_WORD;
  }

  void Word::clipboardPaste() {
    Result::words[this].push_back(Clipboard::pasteText());
  }

  void Word::pushUnormap(mt::CR_STR input) {
    Result::words[this].push_back(input);
  }

  void Word::resetUnormap() {
    Result::words.clear();
  }

  bool Word::printInput() {
    if (Result::numberOfWords(this)) {
      Result::printInput<std::string>(Result::words[this], false);
      return true;
    }
    return false;
  }

  /** NUMBER */

  Number::Number(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in,
    COMMAND_CALLBACK callback_in
  ) : Creator (
    keyword_in, description_in, callback_in
  ) {
    hyphens = "-";
    stringifiedTypeIndex = STRINGIFIED_TYPE_NUMBER;
  }

  void Number::clipboardPaste() {
    mt_uti::VecTools<mt::LD>::concatCopy(
      Result::numbers[this],
      Clipboard::pasteNumbers()
    );
  }

  void Number::pushUnormap(mt::CR_STR input) {
    mt_uti::VecTools<mt::LD>::concatCopy(
      Result::numbers[this],
      mt_uti::Scanner::parseNumbers<mt::LD>(input)
    );
  }

  void Number::resetUnormap() {
    Result::numbers.clear();
  }

  bool Number::printInput() {
    if (Result::numberOfNumbers(this)) {
      Result::printInput<mt::LD>(Result::numbers[this], false);
      return true;
    }
    return false;
  }

  /** TOGGLE */

  Toggle::Toggle(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in,
    COMMAND_CALLBACK callback_in
  ) : Creator (
    keyword_in, description_in, callback_in
  ) {
    hyphens = "--";
    stringifiedTypeIndex = STRINGIFIED_TYPE_TOGGLE;
  }

  void Toggle::clipboardPaste() {
    mt_uti::VecTools<bool>::concatCopy(
      Result::toggles[this],
      Clipboard::pasteConditions()
    );
  }

  void Toggle::pushUnormap(mt::CR_STR input) {
    Result::toggles[this].push_back(
      Langu::ageBooleanizer::test(input)
    );
  }

  void Toggle::resetUnormap() {
    Result::toggles.clear();
  }

  bool Toggle::printInput() {
    if (Result::numberOfToggles(this)) {
      Result::printInput<bool>(Result::toggles[this], false);
      return true;
    }
    return false;
  }
}

#endif // __CLI_MENU__TYPE_CPP__