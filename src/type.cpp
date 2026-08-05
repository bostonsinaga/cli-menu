#ifndef __CLI_MENU__TYPE_CPP__
#define __CLI_MENU__TYPE_CPP__

#include "type.hpp"

namespace cli_menu {

  /** PARAMETER */

  void Parameter::clipboardInputPaste() {
    strargv(Clipboard::pasteText());
  }

  void Parameter::clipboardOutputCopy() {
    if (Data::isTextsEmpty(this)) {
      Langu::ageMessage::printResponse(SENTENCE_EMPTY_OUTPUT_THIS);
    }
    else Clipboard::copyText(&Data::getText(this));
  }

  void Parameter::printOutput() {
    if (Data::isTextsEmpty(this)) {
      Langu::ageMessage::printResponse(SENTENCE_EMPTY_OUTPUT_THIS);
    }
    else Data::printTexts(this, Data::ConsoleCodeSticked, IndentSticked());
  }

  void Parameter::printChildrenOutputs() {
    if (getChildren()) {
      getChildren()->traverse(
        [&](mt_ds::LinkedList *current)->bool {

          static_cast<Command*>(current)->printKeyword(CONSOLE_HINT_1, IndentSticked());

          Data::printTexts(
            static_cast<Command*>(current),
            Data::ConsoleCodeBranched,
            IndentBranched()
          );

          return true;
        }
      );
    }
  }

  void Parameter::resetOutput() {
    Data::resetTexts(this);
    Langu::ageMessage::printResponse(SENTENCE_RESET_OUTPUT_THIS);
  }

  void Parameter::resetDescendantOutputs() {

    if (getChildren()) {
      getChildren()->traverse(
        [&](mt_ds::LinkedList *current)->bool {
          Data::resetTexts(static_cast<Command*>(current));
          return true;
        }
      );
    }

    Langu::ageMessage::printResponse(SENTENCE_RESET_OUTPUT_DESCENDANTS);
  }

  void Parameter::replaceExistingChildByKeyword(Command *newChild) {
    Command *willDestroyed = nullptr;

    // find existing keyword
    if (getChildren()) {
      willDestroyed = static_cast<Command*>(getChildren())
      ->findEach([&](Command *current)->bool {
        return newChild->getKeyword() == current->getKeyword();
      });
    }

    if (willDestroyed) willDestroyed->destroy();
    addChild(newChild);
  }

  Word *Parameter::addWord(
    mt::CR_STR keyw,
    mt::CR_STR desc,
    mt::CR<CODE_CALLBACK> calb,
    mt::CR_BOL applyPresetHelpList
  ) {
    if (!sterilized) {
      Word *word = new Word(keyw, desc, calb);
      replaceExistingChildByKeyword(word);
      if (applyPresetHelpList) word->setPresetHelpList();
      return word;
    }
    return nullptr;
  }

  Number *Parameter::addNumber(
    mt::CR_STR keyw,
    mt::CR_STR desc,
    mt::CR<CODE_CALLBACK> calb,
    mt::CR_BOL applyPresetHelpList
  ) {
    if (!sterilized) {
      Number *number = new Number(keyw, desc, calb);
      replaceExistingChildByKeyword(number);
      if (applyPresetHelpList) number->setPresetHelpList();
      return number;
    }
    return nullptr;
  }

  Boolean *Parameter::addBoolean(
    mt::CR_STR keyw,
    mt::CR_STR desc,
    mt::CR<CODE_CALLBACK> calb,
    mt::CR_BOL applyPresetHelpList
  ) {
    if (!sterilized) {
      Boolean *boolean = new Boolean(keyw, desc, calb);
      replaceExistingChildByKeyword(boolean);
      if (applyPresetHelpList) boolean->setPresetHelpList();
      return boolean;
    }
    return nullptr;
  }

  /** WORD */

  Word::Word(
    mt::CR_STR keyw,
    mt::CR_STR desc,
    mt::CR<CODE_CALLBACK> calb
  ) : Parameter(keyw, desc, calb) {
    hyphens = "-";
    stringifiedTypeIndex = STRINGIFIED_TYPE_INPUT_WORD;
    Data::registerWords(this);
  }

  void Word::destroy() {
    Data::unregisterWords(this);
    Command::destroy();
  }

  void Word::printInput() {
    printInput_temp<WordMaps>();
  }

  void Word::printChildrenInputs() {
    printChildrenInputs_temp<WordMaps>();
  }

  void Word::resetInput() {
    resetInput_temp<WordMaps>();
  }

  void Word::resetDescendantInputs() {
    resetDescendantInputs_temp<WordMaps>();
  }

  void Word::strargv(mt::CR_STR raw) {
    required.first = false;
    Data::xpushWord(this, raw);
  }

  /** NUMBER */

  Number::Number(
    mt::CR_STR keyw,
    mt::CR_STR desc,
    mt::CR<CODE_CALLBACK> calb
  ) : Parameter(keyw, desc, calb) {
    hyphens = "-";
    stringifiedTypeIndex = STRINGIFIED_TYPE_INPUT_NUMBER;
    Data::registerNumbers(this);
  }

  void Number::destroy() {
    Data::unregisterNumbers(this);
    Command::destroy();
  }

  void Number::printInput() {
    printInput_temp<NumberMaps>();
  }

  void Number::printChildrenInputs() {
    printChildrenInputs_temp<NumberMaps>();
  }

  void Number::resetInput() {
    resetInput_temp<NumberMaps>();
  }

  void Number::resetDescendantInputs() {
    resetDescendantInputs_temp<NumberMaps>();
  }

  void Number::strargv(mt::CR_STR raw) {
    required.first = false;
    Data::addNumbers(this, mt_uti::Scanner::parseNumbers<double>(raw));
  }

  /** BOOLEAN */

  Boolean::Boolean(
    mt::CR_STR keyw,
    mt::CR_STR desc,
    mt::CR<CODE_CALLBACK> calb
  ) : Parameter(keyw, desc, calb) {
    hyphens = "--";
    stringifiedTypeIndex = STRINGIFIED_TYPE_INPUT_BOOLEAN;
    Data::registerBooleans(this);
  }

  void Boolean::clipboardInputPaste() {
    required.first = false;

    bool pushed = false;
    mt::VEC_BOL conditions;
    mt::VEC_STR textVector {""};
    std::string textPasted = Clipboard::pasteText();

    // truncated by spaces
    for (mt::CR_CH ch : textPasted) {

      if (mt_uti::StrTool::isWhitespace(ch)) {
        if (!pushed) {
          textVector.push_back("");
          pushed = true;
        }
      }
      else {
        textVector.back() += ch;
        pushed = false;
      }
    }

    // parse booleans
    for (mt::CR_STR str : textVector) {
      conditions.push_back(Langu::ageBooleanizer::test(str));
    }

    Data::addBooleans(this, conditions);
  }

  void Boolean::destroy() {
    Data::unregisterBooleans(this);
    Command::destroy();
  }

  void Boolean::printInput() {
    printInput_temp<BooleanMaps>();
  }

  void Boolean::printChildrenInputs() {
    printChildrenInputs_temp<BooleanMaps>();
  }

  void Boolean::resetInput() {
    resetInput_temp<BooleanMaps>();
  }

  void Boolean::resetDescendantInputs() {
    resetDescendantInputs_temp<BooleanMaps>();
  }

  void Boolean::strargv(mt::CR_STR raw) {
    required.first = false;
    Data::xpushBoolean(this, Langu::ageBooleanizer::test(raw));
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
      // yes
      if (Control::childrenEnterTest(rawstr) ||
        Control::childrenExecuteTest(rawstr) ||
        Control::neighborNextTest(rawstr)
      ) {
        return BOOLEAN_INSTANT_QUESTION_YES;
      }
      // no
      else if (Control::neighborPreviousTest(rawstr)) {
        break;
      }
      // cancel
      else if (Control::parentBackTest(rawstr) ||
        Control::rootBackTest(rawstr) ||
        Control::programQuitTest(rawstr)
      ) {
        return BOOLEAN_INSTANT_QUESTION_CANCELED;
      }
      // help
      else if (Control::commandHelpTest(rawstr) ||
        Control::controllerListTest(rawstr)
      ) {
        Control::printBooleanAvailableValues(true, IndentBranched());
      }
      // list
      else if (Control::childrenListTest(rawstr)) {
        Control::printBooleanAvailableValues(false, IndentSticked());
      }
      else if ( // forbidden
        Control::clearScreenTest(rawstr) ||
        Control::switchModifyTest(rawstr) ||
        Control::switchSelectTest(rawstr) ||
        Control::viewInputThisTest(rawstr) ||
        Control::viewInputChildrenTest(rawstr) ||
        Control::viewOutputThisTest(rawstr) ||
        Control::viewOutputChildrenTest(rawstr) ||
        Control::resetInputThisTest(rawstr) ||
        Control::resetInputDescendantsTest(rawstr) ||
        Control::resetOutputThisTest(rawstr) ||
        Control::resetOutputDescendantsTest(rawstr) ||
        Control::resetDataThisTest(rawstr) ||
        Control::resetDataDescendantsTest(rawstr) ||
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