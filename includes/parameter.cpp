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
    if (!used) {

      if (argumentType == TEXT) {
        ResultInputs::addTexts(name, {defaultText});
      }
      else ResultInputs::addNumbers(name, {defaultNumber});

      useResultInputsIndex();
    }
  }

  void Parameter::setData(mt::CR_STR input) {
    bool isEmpty = true;

    // ignore empty input
    for (mt::CR_CH ch : input) {
      if (ch != ' ' && ch != '\t' && ch != '\n') {
        isEmpty = false;
        break;
      }
    }

    if (isEmpty) return;

    // overwrite
    if (!accumulating) resetData(RESET_FLAG::DISCARD);

    if (argumentType == TEXT) {
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
      mt::VEC_LD numArgs;
      mt_uti::Scanner<mt::LD>::parseNumbers(input, numArgs);

      if (!numArgs.empty()) {
        if (!used) {
          ResultInputs::addNumbers(name, numArgs);
          useResultInputsIndex();
        }
        // accumulated
        else ResultInputs::pushNumbers(
          resultInputsIndex,
          mt_uti::Scanner<mt::LD>::parseNumbers(input)
        );
      }
      else Message::printNeatDialog(
        Message::ERROR_FLAG,
        std::string(matching ? getSentenceSubject(this) : "")
        + "only accepts numeric values",
        !Command::matching
      );
    }
  }

  void Parameter::resetData(RESET_FLAG resetFlag) {
    if (used) {

      // backup registered vector
      if (resetFlag == RESET_FLAG::BACKUP) {
        if (argumentType == TEXT) {
          textsBackup = ResultInputs::getTexts(resultInputsIndex);
        }
        else numbersBackup = ResultInputs::getNumbers(resultInputsIndex);
      }
      // clean the backup
      else if (resetFlag == RESET_FLAG::DISCARD) {
        resetBackupData();
      }

      // pop registered vector
      Command::resetData(resetFlag);

      // clean registered vector
      if (resetFlag != RESET_FLAG::DISCARD && !accumulating) {

        if (argumentType == TEXT) {
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

  const std::string Parameter::getNeedsString() const {
    return " needs" + std::string(
      argumentType == NUMBER ? " numeric " : " "
    ) + "arguments";
  }

  mt::USI Parameter::match() {

    if (!directInputs.empty()) {
      std::string copyName, copyInput;

      // copy to secure original strings
      copyMatchStrings(
        copyName, copyInput, directInputs.back()
      );

      // e.g. '-name argument'
      if (copyName == DashTest::cleanSingle(copyInput)) {
        const mt::USI flag = popBackSet();

        // redirected to first child or unused neighbor
        if (flag == PASSED_FLAG) {
          return middleMatch(true);
        }
        // 'directInputs' may now be empty (dialog may already invoked)
        else if (flag != CONTINUE_FLAG) {
          return flag;
        }

        // implicit argument
        if (!required) {
          initDefaultData();
          return COMPLETED_FLAG;
        }
        // has no argument
        else if (Command::dialogued) {
          Command::matching = false;

          Message::printNeatDialog(
            Message::ERROR_FLAG,
            "the last " + getLevelName() + getNeedsString(), 1
          );

          return question();
        }

        // no dialog
        return FAILED_FLAG;
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
  mt::USI Parameter::middleMatch(mt::CR_BOL needUnusedNeighbor) {
    Command::matching = true;

    return matchTo(static_cast<Cm*>(
      getContinuation(needUnusedNeighbor)
    ));
  }

  mt::USI Parameter::channelTheParent() {

    // question in the middle, back to match
    if (!directInputs.empty()) {
      return middleMatch(false);
    }

    return Command::channelTheParent();
  }

  // argument input
  mt::USI Parameter::answerSpecial(mt::CR_STR bufferStr) {
    setData(bufferStr);
    return downTheChannel();
  }

  mt::USI Parameter::questionEnterTest() {

    // question in the middle, back to match
    if (!directInputs.empty()) {
      Command::selecting = false;
      return middleMatch(false);
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

  /**
   * Might call 'question' for arguments
   * before selection in 'dialog'.
   */
  mt::USI Parameter::dialog() {

    const bool noArguments = (
      (argumentType == TEXT && textsBackup.empty()) ||
      (argumentType == NUMBER && numbersBackup.empty())
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
      if (argumentType == TEXT) {
        ResultInputs::addTexts(name, textsBackup);
      }
      else ResultInputs::addNumbers(name, numbersBackup);

      useResultInputsIndex();
    }

    // no need to set argument exclusively
    return Command::dialog();
  }
}

#endif // __CLI_MENU__PARAMETER_CPP__