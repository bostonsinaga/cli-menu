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

  Creator::Creator(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in
  ) : Command(
    keyword_in, description_in
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

  Word *Creator::createWord(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in
  ) {
    Word *word = new Word(
      keyword_in,
      description_in
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

  Number *Creator::createNumber(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in
  ) {
    Number *number = new Number(
      keyword_in,
      description_in
    );

    replaceExistingKeyword(number);
    number->setPresetHelpList();
    return number;
  }

  Boolean *Creator::createBoolean(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in,
    COMMAND_CALLBACK callback_in
  ) {
    Boolean *boolean = new Boolean(
      keyword_in,
      description_in,
      callback_in
    );

    replaceExistingKeyword(boolean);
    boolean->setPresetHelpList();
    return boolean;
  }

  Boolean *Creator::createBoolean(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in
  ) {
    Boolean *boolean = new Boolean(
      keyword_in,
      description_in
    );

    replaceExistingKeyword(boolean);
    boolean->setPresetHelpList();
    return boolean;
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

  Word::Word(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in
  ) : Creator (
    keyword_in, description_in
  ) {
    hyphens = "-";
    stringifiedTypeIndex = STRINGIFIED_TYPE_WORD;
  }

  void Word::clipboardPaste() {
    required.first = false;
    Result::words[this].push_back(Clipboard::pasteText());
  }

  void Word::pushUnormap(mt::CR_STR input) {
    required.first = false;
    Result::words[this].push_back(input);
  }

  void Word::resetUnormap() {
    if (required.second) required.first = true;
    Result::words.clear();
  }

  bool Word::printInput() {
    if (Result::numberOfWords(this)) {
      Result::printInput<std::string>(this, false);
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

  Number::Number(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in
  ) : Creator (
    keyword_in, description_in
  ) {
    hyphens = "-";
    stringifiedTypeIndex = STRINGIFIED_TYPE_NUMBER;
  }

  void Number::clipboardPaste() {
    required.first = false;

    mt_uti::VecTools<mt::LD>::concatCopy(
      Result::numbers[this],
      Clipboard::pasteNumbers()
    );
  }

  void Number::pushUnormap(mt::CR_STR input) {
    required.first = false;

    mt_uti::VecTools<mt::LD>::concatCopy(
      Result::numbers[this],
      mt_uti::Scanner::parseNumbers<mt::LD>(input)
    );
  }

  void Number::resetUnormap() {
    if (required.second) required.first = true;
    Result::numbers.clear();
  }

  bool Number::printInput() {
    if (Result::numberOfNumbers(this)) {
      Result::printInput<mt::LD>(this, false);
      return true;
    }
    return false;
  }

  /** BOOLEAN */

  Boolean::Boolean(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in,
    COMMAND_CALLBACK callback_in
  ) : Creator (
    keyword_in, description_in, callback_in
  ) {
    hyphens = "--";
    stringifiedTypeIndex = STRINGIFIED_TYPE_BOOLEAN;
  }

  Boolean::Boolean(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in
  ) : Creator (
    keyword_in, description_in
  ) {
    hyphens = "--";
    stringifiedTypeIndex = STRINGIFIED_TYPE_BOOLEAN;
  }

  void Boolean::clipboardPaste() {
    required.first = false;

    mt_uti::VecTools<bool>::concatCopy(
      Result::booleans[this],
      Clipboard::pasteConditions()
    );
  }

  void Boolean::pushUnormap(mt::CR_STR input) {
    required.first = false;

    Result::booleans[this].push_back(
      Langu::ageBooleanizer::test(input)
    );
  }

  void Boolean::resetUnormap() {
    if (required.second) required.first = true;
    Result::booleans.clear();
  }

  bool Boolean::printInput() {
    if (Result::numberOfBooleans(this)) {
      Result::printInput<bool>(this, false);
      return true;
    }
    return false;
  }

  bool Boolean::instantQuestion(
    const SENTENCE_CODE &responseCode,
    mt::CR_STR replacementText
  ) {
    std::string input;

    Langu::ageMessage::printTemplateResponse(
      responseCode, replacementText, true
    );

    return Control::cinDialogInput(input, true) &&
      Langu::ageBooleanizer::test(input);
  }
}

#endif // __CLI_MENU__TYPE_CPP__