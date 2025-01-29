#ifndef __CLI_MENU__PARAMETER_CPP__
#define __CLI_MENU__PARAMETER_CPP__

#include "parameter.h"

namespace cli_menu {

  Parameter::Parameter(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    mt::CR_BOL argumentType_in,
    mt::CR_BOL accumulating_in,
    CM_CALLBACK callback_in,
    mt::CR_BOL propagatingCallback_in
  ) : Command::Command(
    name_in, description_in,
    required_in, parent_in,
    accumulating_in, callback_in,
    propagatingCallback_in
  ) {
    argumentType = argumentType_in;
  }

  Parameter::Parameter(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    mt::CR_BOL argumentType_in,
    mt::CR_BOL accumulating_in
  ) : Command::Command(
    name_in, description_in,
    required_in, parent_in,
    accumulating_in
  ) {
    argumentType = argumentType_in;
  }

  void Parameter::initDefaultData() {

    if (argumentType == TEXT) {
      ResultInputs::addTexts(name, {defaultText});
    }
    else ResultInputs::addNumbers(name, {defaultNumber});

    useResultInputsIndex();
  }

  void Parameter::setData(mt::CR_STR argument) {
    bool isEmpty = true;

    // ignore empty argument
    for (mt::CR_CH ch : argument) {
      if (ch != ' ' && ch != '\t' && ch != '\n') {
        isEmpty = false;
        break;
      }
    }

    if (isEmpty) return;

    if (!used) {
      if (argumentType == TEXT) {
        ResultInputs::addTexts(name, {argument});
      }
      // whitespace is separator
      else ResultInputs::addNumbers(
        name, mt_uti::Scanner<mt::LD>::parseNumbers(argument)
      );

      useResultInputsIndex();
    }
    // accumulated
    else {
      if (argumentType == TEXT) {
        ResultInputs::pushText(resultInputsIndex, argument);
      }
      // whitespace is separator
      else ResultInputs::pushNumbers(
        resultInputsIndex,
        mt_uti::Scanner<mt::LD>::parseNumbers(argument)
      );
    }
  }

  void Parameter::resetData(mt::CR_BOL discarded) {
    if (used) {

      // backup vector member
      if (argumentType == TEXT) {
        texts = ResultInputs::getTexts(resultInputsIndex);
      }
      else numbers = ResultInputs::getNumbers(resultInputsIndex);

      // pop vector
      Command::resetData(discarded);

      // clean vector member
      if (!(discarded || accumulating)) {

        if (argumentType == TEXT) {
          ResultInputs::clearText(resultInputsIndex);
        }
        else ResultInputs::clearNumber(resultInputsIndex);
      }
    }
  }

  // check if 'copyInput' is a command keyword
  bool Parameter::checkArgument(
    LINKED_LIST *node,
    mt::CR_STR copyInput,
    bool &found
  ) {
    std::string copyName;
    static_cast<Cm*>(node)->copyMatchName(copyName);

    if (copyName == DashTest::cleanSingle(copyInput) ||
      copyName == DashTest::cleanDouble(copyInput)
    ) {
      found = true;
      return false;
    }

    return true;
  }

  Parameter::LINKED_LIST *Parameter::getContinuation(
    mt::CR_BOL needUnused
  ) {
    if (isParent()) return children.front();
    else if (needUnused) return getUnusedNeighbor(this);
    return next;
  }

  mt::USI Parameter::popBackSet() {

    // only capture the last reversed 'directInputs'
    if (directInputs.size() > 0) {
      bool found = false;

      std::string copyInput;
      LINKED_LIST *continuation;

      if (isToddler()) continuation = nullptr;
      else continuation = getContinuation();

      if (continuation) {
        copyMatchInput(copyInput, directInputs.back());

        continuation->iterate<mt::CR_STR, bool&>(
          Parameter::checkArgument, copyInput, found
        );

        // question in the middle
        if (found && required) {
          matching = false;

          Message::printNeatDialog(
            Message::ERROR_FLAG,
            "the '" + name + "' " + getLevelName() + " needs arguments", 1
          );

          return question();
        }
      }

      resetData(false);
      setData(directInputs.back());
      directInputs.pop_back();

      if (isToddler()) return COMPLETED_FLAG;
      return PASSED_FLAG;
    }

    return CONTINUE_FLAG;
  }

  mt::USI Parameter::notPopBackSet() {

    // has no argument
    if (Command::dialogued) {
      matching = false;

      Message::printNeatDialog(
        Message::ERROR_FLAG,
        "the last " + getLevelName() + " needs arguments", 1
      );

      return question();
    }
    // no dialog
    return FAILED_FLAG;
  }

  std::string Parameter::getStringifiedArgumentType() {
    if (argumentType == TEXT) return "text";
    return "number";
  }

  std::string Parameter::getDashedName() {
    return "-" + name;
  }

  std::string Parameter::getFullName(
    mt::CR_BOL useDash,
    mt::CR_BOL useInputType,
    mt::CR_BOL useLevelName,
    CR_CLR nameColor
  ) {
    std::string nameStr, inputTypeStr, text;

    // choose name
    if (useDash) {
      nameStr = getDashedName();
    }
    else nameStr = name;

    // set 'nameStr' color
    nameStr = Color::getString(
      nameStr, nameColor
    );

    // add input 'argumentType'
    if (useInputType) {
      inputTypeStr = "<" + getStringifiedArgumentType() + ">";
    }

    // final assignment
    text += nameStr + Color::getString(
      inputTypeStr,
      Command::usingDashesBoundaryLine ?
        Color::AZURE : Color::ROYAL_BLUE
    );

    // add level name
    if (useLevelName) {
      text += getLevelLabel();
    }

    return text;
  }

  std::string Parameter::getFillingStatusString(
    mt::CR_BOL usingAbbreviations
  ) {
    std::string usedStr;

    if (usingAbbreviations) {
      if (!used) return "emp";
      else if (accumulating) usedStr = "acc";
      else usedStr = "cor";
    }
    else {
      if (!used) return "empty";
      else if (accumulating) usedStr = "accumulation";
      else usedStr = "correction";
    }

    return Color::getString(usedStr, Color::MAGENTA);
  }

  mt::USI Parameter::match() {

    if (directInputs.size()) {
      std::string copyName, copyInput;

      // copy to secure original strings
      copyMatchStrings(
        copyName, copyInput, directInputs.back()
      );

      if (copyName == DashTest::cleanSingle(copyInput)) {
        directInputs.pop_back();
        Command::lastCom = this;

        const mt::USI flag = popBackSet();

        // 'directInputs' may be empty
        if (flag == PASSED_FLAG) {

          // redirected to first child or unused neighbor
          return middleMatch(true);
        }
        else if (flag != CONTINUE_FLAG) {
          return flag;
        }

        // 'directInputs' is empty, could invoke 'question'
        return notPopBackSet();
      }

      // point to neighbor if input not matched
      return askNeighbor();
    }
    // 'directInputs' completion
    else if (isMatchNeedDialog()) {
      return dialogTo(
        static_cast<Cm*>(parent)
      );
    }
    // invoke callback
    else if (ultimate && getRequiredCount() == 0) {
      Command::lastCom = ultimate;
      return COMPLETED_FLAG;
    }
    // print error of incompleteness
    return FAILED_FLAG;
  }

  // match continues after question in the middle
  mt::USI Parameter::middleMatch(mt::CR_BOL needUnused) {
    matching = true;

    return matchTo(
      static_cast<Cm*>(getContinuation(needUnused))
    );
  }

  // argument input
  mt::USI Parameter::answerSpecial(mt::CR_STR bufferStr) {
    setData(bufferStr);
    return PASSED_FLAG;
  }

  mt::USI Parameter::questionEnterTest() {

    // question in the middle, back to match
    if (!directInputs.empty()) {
      return middleMatch();
    }

    return PASSED_FLAG;
  }

  void Parameter::viewAction() {
    ResultInputs::printAt(
      argumentType == TEXT ?
      ResultInputs::RESULT_TEXT : ResultInputs::RESULT_NUMBER,
      resultInputsIndex
    );
  }

  mt::USI Parameter::dialog() {

    const bool noArguments = (argumentType == TEXT && texts.empty()) ||
      (argumentType == NUMBER && numbers.empty());

    const bool needQuestion = (!used && !selecting && isParent()) || isToddler();

    if (noArguments) {
      if (needQuestion) return question();
    }
    // remember the past
    else {
      if (argumentType == TEXT) {
        ResultInputs::addTexts(name, texts);
      }
      else ResultInputs::addNumbers(name, numbers);

      useResultInputsIndex();
    }

    // inverted in base method
    selecting = false;

    // no need to set argument exclusively
    return Command::dialog();
  }
}

#endif // __CLI_MENU__PARAMETER_CPP__