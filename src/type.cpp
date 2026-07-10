#ifndef __CLI_MENU__TYPE_CPP__
#define __CLI_MENU__TYPE_CPP__

#include "type.hpp"

namespace cli_menu {

  /** CREATOR */

  void Creator::replaceExistingChildByKeyword(Command *newChild) {
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

  Word *Creator::addWord(
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

  Number *Creator::addNumber(
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

  Boolean *Creator::addBoolean(
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
  ) : Command(keyw, desc, calb),
    ParameterWord(keyw, desc, calb),
    Creator(keyw, desc, calb)
  {
    this->hyphens = "-";
    this->stringifiedTypeIndex = STRINGIFIED_TYPE_INPUT_WORD;
  }

  void Word::clipboardInputPaste() {
    strargv(Clipboard::pasteText());
  }

  void Word::strargv(mt::CR_STR raw) {
    this->required.first = false;
    this->input.values.push_back(raw);
  }

  /** NUMBER */

  Number::Number(
    mt::CR_STR keyw,
    mt::CR_STR desc,
    mt::CR<CODE_CALLBACK> calb
  ) : Command(keyw, desc, calb),
    ParameterNumber(keyw, desc, calb),
    Creator(keyw, desc, calb)
  {
    this->hyphens = "-";
    this->stringifiedTypeIndex = STRINGIFIED_TYPE_INPUT_NUMBER;
  }

  void Number::clipboardInputPaste() {
    this->required.first = false;
    std::string textPasted = Clipboard::pasteText();

    mt_uti::VecTool<double>::concatCopy(
      this->input.values, mt_uti::Scanner::parseNumbers<double>(textPasted)
    );
  }

  void Number::strargv(mt::CR_STR raw) {
    this->required.first = false;

    mt_uti::VecTool<double>::concatCopy(
      this->input.values, mt_uti::Scanner::parseNumbers<double>(raw)
    );
  }

  /** BOOLEAN */

  Boolean::Boolean(
    mt::CR_STR keyw,
    mt::CR_STR desc,
    mt::CR<CODE_CALLBACK> calb
  ) : Command(keyw, desc, calb),
    ParameterBoolean(keyw, desc, calb),
    Creator(keyw, desc, calb)
  {
    this->hyphens = "--";
    this->stringifiedTypeIndex = STRINGIFIED_TYPE_INPUT_BOOLEAN;
  }

  void Boolean::clipboardInputPaste() {
    this->required.first = false;

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

    mt_uti::VecTool<bool>::concatCopy(this->input.values, conditions);
  }

  void Boolean::strargv(mt::CR_STR raw) {
    this->required.first = false;
    this->input.values.push_back(Langu::ageBooleanizer::test(raw));
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
        Control::printBooleanAvailableValues(true, 2);
      }
      // list
      else if (Control::childrenListTest(rawstr)) {
        Control::printBooleanAvailableValues(false, 0);
      }
      else if ( // forbidden
        Control::switchModifyTest(rawstr) ||
        Control::switchSelectTest(rawstr) ||
        Control::clearScreenTest(rawstr) ||
        Control::viewInputThisTest(rawstr) ||
        Control::viewInputDescendantsTest(rawstr) ||
        Control::viewOutputThisTest(rawstr) ||
        Control::viewOutputDescendantsTest(rawstr) ||
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