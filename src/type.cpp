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
    Parameter::destroy();
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

  void Word::strargv(mt::CR_STR rawstr) {
    required.first = false;
    Data::xpushWord(this, rawstr);
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
    Parameter::destroy();
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

  void Number::strargv(mt::CR_STR rawstr) {
    required.first = false;
    Data::addNumbers(this, mt_uti::Scanner::parseNumbers<double>(rawstr));
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
    mt::VEC_BOL conditions;
    mt_uti::BOOLEANIZER_CODE code;
    mt::VEC_STR vec = mt_uti::StrTool::whitespaceSlice(Clipboard::pasteText());

    // parse only booleans
    for (mt::CR_STR str : vec) {
      code = Boolean::avoidStringTest(str);

      if (code != mt_uti::BOOLEANIZER_OTHER) {
        conditions.push_back(code);
      }
    }

    Data::addBooleans(this, conditions);
  }

  void Boolean::destroy() {
    Data::unregisterBooleans(this);
    Parameter::destroy();
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

  void Boolean::strargv(mt::CR_STR rawstr) {
    required.first = false;
    mt_uti::BOOLEANIZER_CODE code = Boolean::avoidStringTest(rawstr);

    if (code != mt_uti::BOOLEANIZER_OTHER) {
      Data::xpushBoolean(this, code);
    }
  }

  mt_uti::BOOLEANIZER_CODE Boolean::avoidStringTest(mt::CR_STR rawstr) {

    // booleanizer test
    mt_uti::BOOLEANIZER_CODE code = Langu::ageBooleanizer::test(rawstr);

    if (code == mt_uti::BOOLEANIZER_OTHER) {
      // controllers as yes
      if (Control::childrenEnterTest(rawstr) ||
        Control::childrenExecuteTest(rawstr) ||
        Control::neighborNextTest(rawstr)
      ) {
        code = mt_uti::BOOLEANIZER_TRUE;
      }
      // controller as no
      else if (Control::neighborPreviousTest(rawstr)) {
        code = mt_uti::BOOLEANIZER_FALSE;
      }
      // forbidden controllers
      else if (Control::stringIsController(rawstr)) {
        Langu::ageMessage::printResponse(
          SENTENCE_BOOLEAN_FORBIDDEN_CONTROLLER
        );
      }
      // unknown value
      else Langu::ageMessage::printTemplateResponse(
        SENTENCE_UNKNOWN_VALUE, Console::LimitedText::trim(rawstr)
      );
    }

    return code;
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
      // cancel
      if (Control::parentBackTest(rawstr) ||
        Control::rootBackTest(rawstr) ||
        Control::programQuitTest(rawstr)
      ) {
        break;
      }
      // show help
      else if (Control::commandHelpTest(rawstr) ||
        Control::controllerListTest(rawstr)
      ) {
        Control::printBooleanAvailableValues(true, IndentBranched());
      }
      // show list
      else if (Control::childrenListTest(rawstr)) {
        Control::printBooleanAvailableValues(false, IndentSticked());
      }
      else {
        mt_uti::BOOLEANIZER_CODE code = Boolean::avoidStringTest(rawstr);

        // no
        if (code == mt_uti::BOOLEANIZER_FALSE) {
          return BOOLEAN_INSTANT_QUESTION_NO;
        }
        // yes
        else if (code == mt_uti::BOOLEANIZER_TRUE) {
          return BOOLEAN_INSTANT_QUESTION_YES;
        }
      }
    }

    return BOOLEAN_INSTANT_QUESTION_CANCELED;
  }
}

#endif // __CLI_MENU__TYPE_CPP__