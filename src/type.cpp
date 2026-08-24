#ifndef __CLI_MENU__TYPE_CPP__
#define __CLI_MENU__TYPE_CPP__

#include "type.hpp"

namespace cli_menu {

  /** PARAMETER */

  void Parameter::clipboardInputPaste() {
    strargv({Clipboard::pasteText()});
  }

  void Parameter::clipboardOutputCopy() {
    if (Data::isTextsEmpty(this)) {
      Langu::ageMessage::printResponse(SENTENCE_EMPTY_OUTPUT_THIS);
    }
    else Clipboard::copyText(&Data::getText(this));
  }

  void Parameter::selectOutputDown() {
    if (Data::selectText(this, 1)) {
      Langu::ageMessage::printTemplateResponse(
        SENTENCE_SELECT_OUTPUT_DOWN,
        {std::to_string(Data::getTextIndex(this))}
      );
    }
    else Langu::ageMessage::printResponse(SENTENCE_EMPTY_OUTPUT_THIS);
  }

  void Parameter::selectOutputUp() {
    if (Data::selectText(this, -1)) {
      Langu::ageMessage::printTemplateResponse(
        SENTENCE_SELECT_OUTPUT_UP,
        {std::to_string(Data::getTextIndex(this))}
      );
    }
    else Langu::ageMessage::printResponse(SENTENCE_EMPTY_OUTPUT_THIS);
  }

  void Parameter::printOutput(mt::CR_BOL withDesignedSticked) {
    if (withDesignedSticked) {
      if (Data::isTextsEmpty(this)) {
        Langu::ageMessage::printResponse(SENTENCE_EMPTY_OUTPUT_THIS);
      }
      else Data::printTexts(this, Console::IndentSticked(), Data::ConlorHighlightSticked);
    }
    else Data::printTexts(this, Console::IndentBranched(), Data::ConlorHighlightBranched);
  }

  void Parameter::resetOutput(mt::CR_BOL withMessage) {
    Data::resetTexts(this);
    if (withMessage) Langu::ageMessage::printResponse(SENTENCE_RESET_OUTPUT_THIS);
  }

  void Parameter::displayChildrenData(mt::CR_BOL inputOrOutput) {
    if (hasChildren()) {
      getChildren()->head()->forEach(
        [&](mt_ds::LinkedList *current)->bool {

          // only for ortho node
          if (!static_cast<Command*>(current)->isPseudo()) {
            std::cout << '\r';

            static_cast<Command*>(current)->printKeyword(
              CONLOR_TITLE, Console::IndentSticked()
            );

            // input or output selection
            if (inputOrOutput) static_cast<Parameter*>(current)->printInput(false);
            else static_cast<Parameter*>(current)->printOutput(false);
          }

          return true;
        }
      );

      // remove indentation
      std::cout << '\r';
    }
    else Langu::ageMessage::printResponse(SENTENCE_PARAMETER_AT_LEAF);
  }

  void Parameter::cleanDescendantData(mt::CR_BOL inputOrOutput) {

    // reset children down to the leaves
    if (getChildren()) {
      getChildren()->traverse(
        [&](mt_ds::LinkedList *current)->bool {
          if (inputOrOutput) static_cast<Parameter*>(current)->resetInput(false);
          else static_cast<Parameter*>(current)->resetOutput(false);
          return true;
        }
      );
    }

    Langu::ageMessage::printResponse(
      inputOrOutput ? SENTENCE_RESET_INPUT_DESCENDANTS : SENTENCE_RESET_OUTPUT_DESCENDANTS
    );
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

  void Word::selectInputDown() {
    if (Data::selectWord(this, 1)) {
      Langu::ageMessage::printTemplateResponse(
        SENTENCE_SELECT_INPUT_DOWN,
        {std::to_string(Data::getWordIndex(this))}
      );
    }
    else Langu::ageMessage::printResponse(SENTENCE_EMPTY_INPUT_THIS);
  }

  void Word::selectInputUp() {
    if (Data::selectWord(this, -1)) {
      Langu::ageMessage::printTemplateResponse(
        SENTENCE_SELECT_INPUT_UP,
        {std::to_string(Data::getWordIndex(this))}
      );
    }
    else Langu::ageMessage::printResponse(SENTENCE_EMPTY_INPUT_THIS);
  }

  void Word::printInput(mt::CR_BOL withDesignedSticked) {
    if (withDesignedSticked) {
      if (Data::isWordsEmpty(this)) {
        Langu::ageMessage::printResponse(SENTENCE_EMPTY_INPUT_THIS);
      }
      else Data::printWords(this, Console::IndentSticked(), Data::ConlorHighlightSticked);
    }
    else Data::printWords(this, Console::IndentBranched(), Data::ConlorHighlightBranched);
  }

  void Word::resetInput(mt::CR_BOL withMessage) {
    Data::resetWords(this);
    if (withMessage) Langu::ageMessage::printResponse(SENTENCE_RESET_INPUT_THIS);
  }

  void Word::strargv(mt::CR_VEC_STR rawstrs) {
    mt::VEC_STR vecstr;

    // strings slicing in vector
    for (mt::CR_STR str : rawstrs) {
      mt_uti::VecTool<std::string>::concatCopy(
        vecstr, mt_uti::StrTool::whitespaceSliceExceptQuotes(str)
      );
    }

    // save new arguments
    required.first = false;
    Data::addWords(this, vecstr);

    // print number of new arguments
    if (!vecstr.empty()) {
      Langu::ageMessage::printTemplateResponse(
        SENTENCE_ARGUMENT_ADDED, { std::to_string(vecstr.size()), keyword }
      );
    }
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

  void Number::selectInputDown() {
    if (Data::selectNumber(this, 1)) {
      Langu::ageMessage::printTemplateResponse(
        SENTENCE_SELECT_INPUT_DOWN,
        {std::to_string(Data::getNumberIndex(this))}
      );
    }
    else Langu::ageMessage::printResponse(SENTENCE_EMPTY_INPUT_THIS);
  }

  void Number::selectInputUp() {
    if (Data::selectNumber(this, -1)) {
      Langu::ageMessage::printTemplateResponse(
        SENTENCE_SELECT_INPUT_UP,
        {std::to_string(Data::getNumberIndex(this))}
      );
    }
    else Langu::ageMessage::printResponse(SENTENCE_EMPTY_INPUT_THIS);
  }

  void Number::printInput(mt::CR_BOL withDesignedSticked) {
    if (withDesignedSticked) {
      if (Data::isNumbersEmpty(this)) {
        Langu::ageMessage::printResponse(SENTENCE_EMPTY_INPUT_THIS);
      }
      else Data::printNumbers(this, Console::IndentSticked(), Data::ConlorHighlightSticked);
    }
    else Data::printNumbers(this, Console::IndentBranched(), Data::ConlorHighlightBranched);
  }

  void Number::resetInput(mt::CR_BOL withMessage) {
    Data::resetNumbers(this);
    if (withMessage) Langu::ageMessage::printResponse(SENTENCE_RESET_INPUT_THIS);
  }

  void Number::strargv(mt::CR_VEC_STR rawstrs) {
    mt::VEC_DBL numbers[2];
    mt::VEC_STR vecstr;

    // strings slicing in vector
    for (mt::CR_STR str : rawstrs) {
      mt_uti::VecTool<std::string>::concatCopy(
        vecstr, mt_uti::StrTool::whitespaceSlice(str)
      );
    }

    // parse only numbers
    for (mt::CR_STR str : vecstr) {
      numbers[0] = mt_uti::Scanner::parseNumbers<double>(str);

      if (numbers[0].empty()) {
        // forbidden controllers
        if (Control::stringIsController(str)) {
          Langu::ageMessage::printTemplateResponse(
            SENTENCE_FORBIDDEN_CONTROLLER, {str}
          );
        }
        // unknown value
        else Langu::ageMessage::printTemplateResponse(
          SENTENCE_UNKNOWN_VALUE, {Console::LimitedText::trim(str)}
        );
      }
      else mt_uti::VecTool<double>::concatCut(numbers[1], numbers[0]);
    }

    // save new arguments
    required.first = false;
    Data::addNumbers(this, numbers[1]);

    // print number of new arguments
    if (!numbers[1].empty()) {
      Langu::ageMessage::printTemplateResponse(
        SENTENCE_ARGUMENT_ADDED, { std::to_string(numbers[1].size()), keyword }
      );
    }
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

  void Boolean::destroy() {
    Data::unregisterBooleans(this);
    Parameter::destroy();
  }

  void Boolean::selectInputDown() {
    if (Data::selectBoolean(this, 1)) {
      Langu::ageMessage::printTemplateResponse(
        SENTENCE_SELECT_INPUT_DOWN,
        {std::to_string(Data::getBooleanIndex(this))}
      );
    }
    else Langu::ageMessage::printResponse(SENTENCE_EMPTY_INPUT_THIS);
  }

  void Boolean::selectInputUp() {
    if (Data::selectBoolean(this, -1)) {
      Langu::ageMessage::printTemplateResponse(
        SENTENCE_SELECT_INPUT_UP,
        {std::to_string(Data::getBooleanIndex(this))}
      );
    }
    else Langu::ageMessage::printResponse(SENTENCE_EMPTY_INPUT_THIS);
  }

  void Boolean::printInput(mt::CR_BOL withDesignedSticked) {
    if (withDesignedSticked) {
      if (Data::isBooleansEmpty(this)) {
        Langu::ageMessage::printResponse(SENTENCE_EMPTY_INPUT_THIS);
      }
      else Data::printBooleans(this, Console::IndentSticked(), Data::ConlorHighlightSticked);
    }
    else Data::printBooleans(this, Console::IndentBranched(), Data::ConlorHighlightBranched);
  }

  void Boolean::resetInput(mt::CR_BOL withMessage) {
    Data::resetBooleans(this);
    if (withMessage) Langu::ageMessage::printResponse(SENTENCE_RESET_INPUT_THIS);
  }

  void Boolean::strargv(mt::CR_VEC_STR rawstrs) {
    mt::VEC_BOL conditions;
    mt_uti::BOOLEANIZER_CODE bolcode;
    mt::VEC_STR vecstr;

    // strings slicing in vector
    for (mt::CR_STR str : rawstrs) {
      mt_uti::VecTool<std::string>::concatCopy(
        vecstr, mt_uti::StrTool::whitespaceSlice(str)
      );
    }

    // parse only booleans
    for (mt::CR_STR str : vecstr) {
      bolcode = Boolean::controllerTest(str);

      if (bolcode != mt_uti::BOOLEANIZER_OTHER) {
        conditions.push_back(bolcode);
      }
    }

    // save new arguments
    required.first = false;
    Data::addBooleans(this, conditions);

    // print number of new arguments
    if (!conditions.empty()) {
      Langu::ageMessage::printTemplateResponse(
        SENTENCE_ARGUMENT_ADDED, { std::to_string(conditions.size()), keyword }
      );
    }
  }

  mt_uti::BOOLEANIZER_CODE Boolean::controllerTest(mt::CR_STR rawstr) {

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
        Langu::ageMessage::printTemplateResponse(
          SENTENCE_FORBIDDEN_CONTROLLER, {rawstr}
        );
      }
      // unknown value
      else Langu::ageMessage::printTemplateResponse(
        SENTENCE_UNKNOWN_VALUE, {Console::LimitedText::trim(rawstr)}
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
      responseCode, {replacementText}, true
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
        Control::printBooleanAvailableValues(true, Console::IndentBranched());
      }
      // show list
      else if (Control::childrenListTest(rawstr)) {
        Control::printBooleanAvailableValues(false, Console::IndentSticked());
      }
      else {
        mt_uti::BOOLEANIZER_CODE code = Boolean::controllerTest(rawstr);

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