#ifndef __CLI_MENU__PARAMETER_CPP__
#define __CLI_MENU__PARAMETER_CPP__

#include "parameter.h"

namespace cli_menu {

  Parameter::Parameter(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    PARAM_TYPE argumentType_in,
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
    setInputTypeString();
  }

  Parameter::Parameter(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    PARAM_TYPE argumentType_in,
    mt::CR_BOL accumulating_in
  ) : Command::Command(
    name_in, description_in,
    required_in, parent_in,
    accumulating_in
  ) {
    argumentType = argumentType_in;
    setInputTypeString();
  }

  void Parameter::setInputTypeString() {

    if (argumentType == PARAM_NUMBER) {
      inputTypeString = "numeric argument";
    }
    else inputTypeString = "text argument";

    if (accumulating) {
      inputTypeString += "s";
    }
    else inputTypeString = "a " + inputTypeString;
  }

  void Parameter::initDefaultData() {
    if (!used) {

      if (argumentType == PARAM_TEXT) {
        ResultInputs::addTexts(name, {defaultText});
      }
      else ResultInputs::addNumbers(name, {defaultNumber});

      useResultInputsIndex();
    }
  }

  void Parameter::setNumbers(mt::CR_VEC_LD numbers) {
    if (!numbers.empty()) {
      overwrite();

      if (!used) {
        ResultInputs::addNumbers(name, numbers);
        useResultInputsIndex();
      }
      // accumulated
      else ResultInputs::pushNumbers(
        resultInputsIndex, numbers
      );
    }
    else Message::printNeatDialog(
      MESSAGE_ERROR,
      std::string(Command::matching ? getSubjectString(this) : "")
      + "only accepts numeric values"
    );
  }

  void Parameter::setData(mt::CR_STR input) {
    bool isEmpty = true;

    // ignore empty input
    for (mt::CR_CH ch : input) {
      if (!Util::isWhitespace(ch)) {
        isEmpty = false;
        break;
      }
    }

    // only spaces detected
    if (isEmpty) return;

    if (argumentType == PARAM_TEXT) {
      overwrite();

      if (!used) {
        ResultInputs::addTexts(name, {input});
        useResultInputsIndex();
      }
      // accumulated
      else ResultInputs::pushText(
        resultInputsIndex, input
      );
    }
    // just for numbers
    else {
      // whitespace is separator
      mt::VEC_LD numbers;
      mt_uti::Scanner<mt::LD>::parseNumbers(input, numbers);
      setNumbers(numbers);
    }
  }

  void Parameter::resetData(RESET_ENUM resetEnum) {
    if (used) {

      // backup registered vector
      if (resetEnum == RESET_BACKUP) {
        if (argumentType == PARAM_TEXT) {
          textsBackup = ResultInputs::getTexts(resultInputsIndex);
        }
        else numbersBackup = ResultInputs::getNumbers(resultInputsIndex);
      }
      // clean the backup
      else if (resetEnum == RESET_DISCARD) {
        resetBackupData();
      }

      // pop registered vector
      Command::resetData(resetEnum);

      // clean registered vector
      if (resetEnum != RESET_DISCARD && !accumulating) {

        if (argumentType == PARAM_TEXT) {
          ResultInputs::clearText(resultInputsIndex);
        }
        else ResultInputs::clearNumber(resultInputsIndex);
      }
    }
  }

  void Parameter::resetBackupData() {
    textsBackup = {};
    numbersBackup = {};
  }

  std::string Parameter::getStringifiedArgumentType() {
    if (argumentType == PARAM_TEXT) return "text";
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
    std::string nameStr, typeStr, text;

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
      typeStr = "<" + getStringifiedArgumentType() + ">";
    }

    // final assignment
    text += nameStr + Color::getString(
      typeStr,
      Command::usingDashesBoundaryLine ?
        Color::AZURE : Color::ROYAL_BLUE
    );

    // add level name
    if (useLevelName) {
      text += getLevelLabel();
    }

    return text;
  }

  COMMAND_ENUM Parameter::match() {

    if (!directInputs.empty()) {
      std::string copyName, copyInput;

      // copy to secure original strings
      copyMatchStrings(
        copyName, copyInput, directInputs.back()
      );

      // e.g. '-name argument'
      if (copyName == DashTest::cleanSingle(copyInput)) {
        const COMMAND_ENUM enumeration = popBackSet();

        // redirected to first child or unused neighbor
        if (enumeration == COMMAND_PASSED) {
          return middleMatch(true);
        }
        // 'directInputs' may now be empty (dialog may already called)
        else if (enumeration != COMMAND_CONTINUE) {
          return enumeration;
        }

        // implicit argument
        if (!required) {
          initDefaultData();
          return COMMAND_COMPLETED;
        }
        // has no argument
        else if (Command::dialogued) {
          Command::matching = false;

          Message::printNeatDialog(
            MESSAGE_ERROR, getNeedsString(true)
          );

          return question();
        }

        // no dialog
        return COMMAND_FAILED;
      }

      // point to neighbor if input not matched
      return askNeighbor();
    }
    // 'directInputs' completion
    else if (doesMatchNeedDialog()) {
      return dialogTo(
        static_cast<Cm*>(parent)
      );
    }
    // invoke callback
    else if (ultimate && getRequiredCount() == 0) {
      Command::lastCom = ultimate;
      return COMMAND_COMPLETED;
    }
    // print error of incompleteness
    return COMMAND_FAILED;
  }

  // match continues after question in the middle
  COMMAND_ENUM Parameter::middleMatch(mt::CR_BOL needUnusedNeighbor) {
    Command::matching = true;

    return matchTo(static_cast<Cm*>(
      getContinuation(needUnusedNeighbor)
    ));
  }

  COMMAND_ENUM Parameter::channelTheParent() {

    // question in the middle, back to match
    if (!directInputs.empty()) {
      return middleMatch(false);
    }

    return Command::channelTheParent();
  }

  // argument input
  COMMAND_ENUM Parameter::answerSpecial(mt::CR_STR bufferStr) {
    setData(bufferStr);
    return downTheChannel(COMMAND_PASSED);
  }

  COMMAND_ENUM Parameter::questionEnterTest() {

    // question in the middle, back to match
    if (!directInputs.empty()) {
      Command::selecting = false;
      return middleMatch(false);
    }

    return COMMAND_PASSED;
  }

  void Parameter::clipboardAction() {

    if (argumentType == PARAM_TEXT) {
      std::string textRef;
      Clipboard::pasteText(textRef);
      setData(textRef);
    }
    else {
      mt::VEC_LD numbersRef;
      Clipboard::pasteNumbers(numbersRef);
      setNumbers(numbersRef);
    }
  }

  void Parameter::viewAction() {
    ResultInputs::printAt(
      argumentType == PARAM_TEXT ?
      ResultInputs::RESULT_TEXT : ResultInputs::RESULT_NUMBER,
      resultInputsIndex
    );
  }

  /**
   * Might call 'question' for arguments
   * before selection in 'dialog'.
   */
  COMMAND_ENUM Parameter::dialog() {

    const bool noArguments = (
      (argumentType == PARAM_TEXT && textsBackup.empty()) ||
      (argumentType == PARAM_NUMBER && numbersBackup.empty())
    );

    const bool needQuestion = (
      (!used && !Command::selecting && isParent()) ||
      isToddler()
    );

    if (noArguments) {
      if (needQuestion) return question();
    }
    // remember the past
    else {
      if (argumentType == PARAM_TEXT) {
        ResultInputs::addTexts(name, textsBackup);
      }
      else ResultInputs::addNumbers(name, numbersBackup);

      useResultInputsIndex();
    }

    // no need to set argument exclusively
    return Command::dialog();
  }

  void Parameter::setAccumulating(mt::CR_BOL cond) {
    accumulating = cond;

    if (accumulating) {
      inputTypeString += "s";
    }
    else inputTypeString = "a " + inputTypeString;
  }
}

#endif // __CLI_MENU__PARAMETER_CPP__