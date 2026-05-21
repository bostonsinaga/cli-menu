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

      if (getChildren()) getChildren()->forEach(
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

  /** WORD */

  Word::Word(
    mt::CR_STR keyword_in,
    mt::CR_STR description_in,
    COMMAND_CALLBACK callback_in
  ) : Creator (
    keyword_in, description_in, callback_in
  ) {
    hyphens = "-";
    stringifiedTypeIndex = STRINGIFIED_TYPE_INPUT_WORD;
  }

  void Word::clipboardInputPaste() {
    required.first = false;
    Data::Input::words[this].push_back(Clipboard::pasteText());
  }

  void Word::pushInputUnormap(mt::CR_STR input) {
    required.first = false;
    Data::Input::words[this].push_back(input);
  }

  bool Word::resetInputUnormap() {
    if (required.second) required.first = true;

    if (Data::Input::hasWords(this)) {
      Data::Input::words.erase(this);
      return true;
    }

    return false;
  }

  bool Word::printInput() {
    if (Data::Input::numberOfWords(this)) {
      Data::Input::printVector<std::string>(this, false);
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
    stringifiedTypeIndex = STRINGIFIED_TYPE_INPUT_NUMBER;
  }

  void Number::clipboardInputPaste() {
    required.first = false;

    mt_uti::VecTool<mt::LD>::concatCopy(
      Data::Input::numbers[this],
      Clipboard::pasteNumbers()
    );
  }

  void Number::pushInputUnormap(mt::CR_STR input) {
    required.first = false;

    mt_uti::VecTool<mt::LD>::concatCopy(
      Data::Input::numbers[this],
      mt_uti::Scanner::parseNumbers<mt::LD>(input)
    );
  }

  bool Number::resetInputUnormap() {
    if (required.second) required.first = true;

    if (Data::Input::hasNumbers(this)) {
      Data::Input::numbers.erase(this);
      return true;
    }

    return false;
  }

  bool Number::printInput() {
    if (Data::Input::numberOfNumbers(this)) {
      Data::Input::printVector<mt::LD>(this, false);
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
    stringifiedTypeIndex = STRINGIFIED_TYPE_INPUT_BOOLEAN;
  }

  void Boolean::clipboardInputPaste() {
    required.first = false;

    mt_uti::VecTool<bool>::concatCopy(
      Data::Input::booleans[this],
      Clipboard::pasteConditions()
    );
  }

  void Boolean::pushInputUnormap(mt::CR_STR input) {
    required.first = false;

    Data::Input::booleans[this].push_back(
      Langu::ageBooleanizer::test(input)
    );
  }

  bool Boolean::resetInputUnormap() {
    if (required.second) required.first = true;

    if (Data::Input::hasBooleans(this)) {
      Data::Input::booleans.erase(this);
      return true;
    }
  
    return false;
  }

  bool Boolean::printInput() {
    if (Data::Input::numberOfBooleans(this)) {
      Data::Input::printVector<bool>(this, false);
      return true;
    }
    return false;
  }

  BOOLEAN_INSTANT_QUESTION_CODE Boolean::instantQuestion(
    const SENTENCE_CODE &responseCode,
    mt::CR_STR replacementText
  ) {
    std::string rawstr;

    Langu::ageMessage::printTemplateResponse(
      responseCode, replacementText, true
    );

    while (Control::cinDialogInput(rawstr, true)) {
      // no
      if (Control::neighborPreviousTest(rawstr)) {
        break;
      }
      else if ( // yes
        Control::childrenEnterTest(rawstr) ||
        Control::neighborNextTest(rawstr)
      ) {
        return BOOLEAN_INSTANT_QUESTION_YES;
      }
      else if ( // cancel
        Control::parentBackTest(rawstr) ||        
        Control::programQuitTest(rawstr)
      ) {
        return BOOLEAN_INSTANT_QUESTION_CANCELED;
      }
      // help
      else if (
        Control::commandHelpTest(rawstr) ||
        Control::controllerListTest(rawstr)
      ) {
        Control::printBooleanAvailableValues(true, 2);
      }
      // list
      else if (Control::childrenListTest(rawstr)) {
        Control::printBooleanAvailableValues(false, 0);
      }
      else if ( // forbidden
        Control::switchModifyTest(rawstr) ||
        Control::switchSelectTest(rawstr) ||
        Control::viewInputThisTest(rawstr) ||
        Control::viewInputAllTest(rawstr) ||
        Control::viewOutputThisTest(rawstr) ||
        Control::viewOutputAllTest(rawstr) ||
        Control::resetInputThisTest(rawstr) ||
        Control::resetInputAllTest(rawstr) ||
        Control::resetOutputThisTest(rawstr) ||
        Control::resetOutputAllTest(rawstr) ||
        Control::copyOutputTest(rawstr) ||
        Control::pasteInputTest(rawstr)
      ) {
        Langu::ageMessage::printResponse(
          SENTENCE_BOOLEAN_INSTANT_QUESTION_FORBIDDEN_CONTROLLER
        );
      }
      // yes
      else if (Langu::ageBooleanizer::test(rawstr)) {
        return BOOLEAN_INSTANT_QUESTION_YES;
      }
      else break; // no
    }

    return BOOLEAN_INSTANT_QUESTION_NO;
  }
}

#endif // __CLI_MENU__TYPE_CPP__