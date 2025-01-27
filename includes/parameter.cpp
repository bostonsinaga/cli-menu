#ifndef __CLI_MENU__PARAMETER_CPP__
#define __CLI_MENU__PARAMETER_CPP__

#include "parameter.h"

namespace cli_menu {

  const std::string Parameter::needsArgStr = " needs an argument";

  Parameter::Parameter(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    mt::CR_BOL argumentType_in,
    mt::CR_BOL accumulating_in,
    RESULT_CALLBACK callback_in,
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
    mt::CR_BOL accumulating_in,
    PLAIN_CALLBACK callback_in,
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
      INPUTS.result.addTexts(name, {defaultText});
    }
    else INPUTS.result.addNumbers(name, {defaultNumber});

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
        INPUTS.result.addTexts(name, {argument});
      }
      // whitespace is separator
      else INPUTS.result.addNumbers(
        name, mt_uti::Scanner<mt::LD>::parseNumbers(argument)
      );

      useResultInputsIndex();
    }
    // accumulated
    else {
      if (argumentType == TEXT) {
        INPUTS.result.pushText(INPUTS.index, argument);
      }
      // whitespace is separator
      else INPUTS.result.pushNumbers(
        INPUTS.index,
        mt_uti::Scanner<mt::LD>::parseNumbers(argument)
      );
    }
  }

  void Parameter::resetData(mt::CR_BOL discarded) {
    if (used) {

      // backup vector member
      if (argumentType == TEXT) {
        texts = INPUTS.result.getTexts(INPUTS.index);
      }
      else numbers = INPUTS.result.getNumbers(INPUTS.index);

      // pop vector
      Command::resetData(discarded);

      // clean vector member
      if (!(discarded || accumulating)) {

        if (argumentType == TEXT) {
          INPUTS.result.clearText(INPUTS.index);
        }
        else INPUTS.result.clearNumber(INPUTS.index);
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

    // only capture the last reversed 'INPUTS.direct'
    if (INPUTS.direct.size() > 0) {
      bool found = false;

      std::string copyInput;
      LINKED_LIST *continuation;

      if (isToddler()) continuation = nullptr;
      else continuation = getContinuation();

      if (continuation) {
        copyMatchInput(copyInput, INPUTS.direct.back());

        continuation->iterate<mt::CR_STR, bool&>(
          Parameter::checkArgument, copyInput, found
        );

        // question in the middle
        if (found && required) {
          matching = false;

          Message::printNeatDialog(
            Message::ERROR_FLAG,
            "the '" + name + "' " + getLevelName() + needsArgStr, 1
          );

          return question();
        }
      }

      resetData(false);
      setData(INPUTS.direct.back());
      INPUTS.direct.pop_back();

      if (isToddler()) return COMPLETED_FLAG;
      return PASSED_FLAG;
    }

    return ERROR_FLAG;
  }

  mt::USI Parameter::notPopBackSet() {

    // has no argument
    if (Command::dialogued) {
      matching = false;

      Message::printNeatDialog(
        Message::ERROR_FLAG,
        "the last " + getLevelName() + needsArgStr, 1
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
        Color::AZURE : Color(0, 95, 223)
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

    if (INPUTS.direct.size()) {
      std::string copyName, copyInput;

      // copy to secure original strings
      copyMatchStrings(
        copyName, copyInput, INPUTS.direct.back()
      );

      if (copyName == DashTest::cleanSingle(copyInput)) {
        INPUTS.direct.pop_back();
        Command::lastCom = this;

        const mt::USI flag = popBackSet();

        // 'INPUTS.direct' may be empty
        if (flag == PASSED_FLAG) {

          // redirected to first child or unused neighbor
          return middleMatch(true);
        }
        else if (flag != ERROR_FLAG) {
          return flag;
        }

        // 'INPUTS.direct' is empty, could invoke 'question'
        return notPopBackSet();
      }

      // point to neighbor if input not matched
      return askNeighbor();
    }
    // 'INPUTS.direct' completion
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

  mt::USI Parameter::answerControl(mt::CR_STR controlStr) {

    if (Control::backTest(controlStr)) {
      const mt::USI flag = tryToGoBack();

      if (flag != ERROR_FLAG) {
        return flag;
      }
    }
    else if (Control::clipboardTest(controlStr)) {
      setData(clipboard.paste());
    }
    else if (Control::enterTest(controlStr)) {
      // need argument
      if (!used && required) {
        Message::printNeatDialog(
          Message::ERROR_FLAG,
          "this " + getLevelName() + needsArgStr
        );
      }
      // question in the middle, back to match
      else if (!INPUTS.direct.empty()) {
        return middleMatch();
      }
      // pointing to first child
      else if (isParent()) {
        return dialogTo(
          static_cast<Cm*>(children.front())
        );
      }
      // directly completed
      else if (doesUltimateAllowEnter()) {
        Command::lastCom = chooseLastCommand();
        return COMPLETED_FLAG;
      }
    }
    else if (Control::helpTest(controlStr)) {
      printHelp();
    }
    else if (Control::listTest(controlStr)) {
      printList();
    }
    else if (
      Control::nextTest(controlStr) ||
      Control::previousTest(controlStr)
    ) {
      const mt::USI tryToSkipFlag = tryToSkip(
        Control::getSharedFlag() == Control::NEXT
      );

      if (tryToSkipFlag != ERROR_FLAG) {
        return tryToSkipFlag;
      }
    }
    else if (Control::quitTest(controlStr)) {
      Command::stopThreadsLoop();
    }
    else if (Control::selectTest(controlStr)) {

      const mt::USI tryToSelectFlag = tryToSelect(
        "arguments are given"
      );

      if (tryToSelectFlag != ERROR_FLAG) {
        return tryToSelectFlag;
      }
    }
    else Control::printError();

    return PASSED_FLAG;
  }

  // argument input
  mt::USI Parameter::answerSpecial(mt::CR_STR cinStr) {
    setData(cinStr);
    return PASSED_FLAG;
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
        INPUTS.result.addTexts(name, texts);
      }
      else INPUTS.result.addNumbers(name, numbers);

      useResultInputsIndex();
    }

    // inverted in base method
    selecting = false;

    // no need to set argument exclusively
    return Command::dialog();
  }
}

#endif // __CLI_MENU__PARAMETER_CPP__