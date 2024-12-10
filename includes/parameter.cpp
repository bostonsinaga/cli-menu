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
    CR_SP_CALLBACK callback_in,
    mt::CR_BOL propagatingCallback_in
  ) : Command::Command(
    name_in, description_in,
    required_in, parent_in,
    callback_in, propagatingCallback_in
  ) {
    argumentType = argumentType_in;
    accumulating = accumulating_in;
  }

  Parameter::Parameter(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    mt::CR_BOL argumentType_in,
    mt::CR_BOL accumulating_in,
    CR_SP_PLAIN_CALLBACK callback_in,
    mt::CR_BOL propagatingCallback_in
  ) : Command::Command(
    name_in, description_in,
    required_in, parent_in,
    callback_in, propagatingCallback_in
  ) {
    argumentType = argumentType_in;
    accumulating = accumulating_in;
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
    required_in, parent_in
  ) {
    argumentType = argumentType_in;
    accumulating = accumulating_in;
  }

  void Parameter::setData(
    ParamData &paramData,
    mt::CR_STR argument
  ) {
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
        paramData.texts.push_back(argument);
        paramData.numbers.push_back({});
      }
      // space or newline is a separator
      else {
        paramData.texts.push_back("");
        paramData.numbers.push_back(
          mt_uti::Scanner<double>::parseNumbers(argument)
        );
      }

      paramData.conditions.push_back(false);
      paramDataIndex = paramData.texts.size() - 1;
      updateRequiredUsed(false);
    }
    // accumulated to get multiline input
    else {
      if (argumentType == TEXT) {
        paramData.texts[paramDataIndex] += argument;
      }
      // space or newline is a separator
      else mt_uti::VecTools<double>::concat(
        paramData.numbers[paramDataIndex],
        mt_uti::Scanner<double>::parseNumbers(argument)
      );
    }
  }

  void Parameter::resetArgument(ParamData &paramData) {
    if (used && !accumulating) {
      if (argumentType == TEXT) {
        paramData.texts[paramDataIndex] = "";
      }
      else paramData.numbers[paramDataIndex] = {};
    }
  }

  // check if 'copyInput' is a command keyword
  bool Parameter::checkArgument(
    LINKED_LIST *node,
    mt::CR_STR copyInput,
    bool &found
  ) {
    std::string copyName;

    Command::copyMatchName(
      copyName, static_cast<Parameter*>(node)->name
    );

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

  mt::USI Parameter::popBackSet(
    mt::VEC_STR &inputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    // only capture the last reversed 'inputs'
    if (inputs.size() > 0) {

      bool found = false,
        notDependence = isIndependence();

      std::string copyInput;
      LINKED_LIST *continuation;

      if (notDependence) continuation = nullptr;
      else continuation = getContinuation();

      if (continuation) {
        Command::copyMatchInput(
          copyInput, inputs[inputs.size() - 1]
        );

        continuation->iterate<mt::CR_STR, bool&>(
          Parameter::checkArgument, copyInput, found
        );

        // question in the middle
        if (found && required) {

          Message::printNeatDialogError(
            "the '" + name + "' " + getLevelName(true) + needsArgStr, 1
          );

          return question(inputs, paramData, lastCom);
        }
      }

      resetArgument(paramData);
      setData(paramData, inputs[inputs.size() - 1]);
      inputs.pop_back();

      if (notDependence) return FLAG::COMPLETED;
      return FLAG::PASSED;
    }

    return FLAG::ERROR;
  }

  mt::USI Parameter::notPopBackSet(
    mt::VEC_STR &inputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    // has no argument
    if (Command::dialogued) {

      Message::printNeatDialogError(
        "the last " + getLevelName(true) + needsArgStr, 1
      );

      return question(inputs, paramData, lastCom);
    }
    // no dialog
    return FLAG::FAILED;
  }

  std::string Parameter::getStringifiedArgumentType() {
    if (argumentType == TEXT) return "text";
    return "number";
  }

  std::string Parameter::getDashedName() {
    return "-" + name;
  }

  std::string Parameter::getFullName(
    CR_FullNameProfile fullNameProfile
  ) {
    std::string nameStr, inputTypeStr, text;

    // choose name
    if (fullNameProfile.useDash) {
      nameStr = getDashedName();
    }
    else nameStr = name;

    // set 'nameStr' color
    nameStr = Color::getString(
      nameStr, fullNameProfile.nameColor
    );

    // add input argumentType
    if (fullNameProfile.useInputType) {
      inputTypeStr = "<" + getStringifiedArgumentType() + ">";
    }

    /** Final assignments */

    text += nameStr + Color::getString(
      inputTypeStr,
      Command::usingDashesBoundaryLine ?
        Color::AZURE : Color(0, 95, 223)
    );

    // add level name
    if (fullNameProfile.useLevelName) {
      text += getMainLabel();
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

  mt::USI Parameter::match(
    mt::VEC_STR &inputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    if (inputs.size() > 0) {
      std::string copyName, copyInput;

      // copy to secure original strings
      Command::copyMatchStrings(
        copyName, copyInput, name, inputs[inputs.size() - 1]
      );

      if (copyName == DashTest::cleanSingle(copyInput)) {
        inputs.pop_back();
        *lastCom = this;

        const mt::USI popBackSetFlag = popBackSet(
          inputs, paramData, lastCom
        );

        // 'inputs' may be empty
        if (popBackSetFlag == FLAG::PASSED) {

          // redirected to first child or unused neighbor
          return middleMatch(
            inputs, paramData, lastCom, true
          );
        }
        else if (popBackSetFlag != FLAG::ERROR) {
          return popBackSetFlag;
        }

        // 'inputs' is empty, could invoke 'question'
        return notPopBackSet(inputs, paramData, lastCom);
      }

      // point to neighbor if input not matched
      return askNeighbor(inputs, paramData, lastCom);
    }
    // 'inputs' completion
    else if (isMatchNeedDialog()) {
      return dialogTo(
        static_cast<Cm*>(parent), inputs, paramData, lastCom
      );
    }
    // invoke callback
    else if (ultimate && getRequiredCount() == 0) {
      *lastCom = ultimate;
      return FLAG::COMPLETED;
    }
    // print error of incompleteness
    return FLAG::FAILED;
  }

  // match continues after question in the middle
  mt::USI Parameter::middleMatch(
    mt::VEC_STR &inputs,
    ParamData &paramData,
    Command **lastCom,
    mt::CR_BOL needUnused
  ) {
    return matchTo(
      static_cast<Cm*>(getContinuation(needUnused)),
      inputs, paramData, lastCom
    );
  }

  mt::USI Parameter::question(
    mt::VEC_STR &inputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    std::string buffer;
    const bool notDependence = !isDependence();
    resetArgument(paramData);

    // question display on non-dependences
    if (notDependence) questionedGroup = true;

    printAfterBoundaryLine(notDependence ?
      getInlineRootNames() : getFullNameWithUltimate()
    );

    while (true) {
      Message::setDialogInput(buffer);

      // copy to secure original input
      std::string controlStr = mt_uti::StrTools::getStringToLowercase(buffer);

      // control input
      if (Control::cancelTest(controlStr)) {
        break; // returns 'FLAG::CANCELED' below
      }
      else if (Control::enterTest(controlStr)) {
        // need argument
        if (isRequiredContainerEmpty()) {
          Message::printDialogError(
            "this " + getLevelName(true) + needsArgStr
          );
        }
        // question in the middle, back to match
        else if (!inputs.empty()) {
          return middleMatch(inputs, paramData, lastCom);
        }
        // pointing to first child
        else if (isParent()) {
          return dialogTo(
            static_cast<Cm*>(children.front()), inputs, paramData, lastCom
          );
        }
        // directly completed
        else if (doUltimateAllowEnter()) {
          *lastCom = chooseLastCommand();
          return FLAG::COMPLETED;
        }
      }
      else if (Control::helpTest(buffer)) {
        printHelp();
      }
      else if (Control::nextTest(controlStr)) {
        // proceed to next question
        if (isOptional() || (isRequired() && used)) {
          *lastCom = chooseLastCommand();

          // question in the middle, back to match
          if (!inputs.empty()) {
            return middleMatch(inputs, paramData, lastCom);
          }
          // parent
          else if (notDependence) {
            // back to selection
            if (isParent()) {
              return dialog(inputs, paramData, lastCom);
            }
            // pointing to neighbor
            else if (next) return dialogTo(
              static_cast<Cm*>(next), inputs, paramData, lastCom
            );
            else printNullptrNextError();
          }
          // dependence
          else return questionTo(
            getUnusedNeighbor(this), inputs, paramData, lastCom
          );
        }
        // required items are not complete
        else printRequiredNextError();
      }
      else if (Control::selectTest(controlStr)) {

        const mt::USI tryToSkipWithSelectionFlag = tryToSkipWithSelection(
          inputs, paramData, lastCom,
          "arguments are given"
        );

        if (tryToSkipWithSelectionFlag != FLAG::ERROR) {
          return tryToSkipWithSelectionFlag;
        }
      }
      // value input
      else setData(paramData, buffer);
    }

    return FLAG::CANCELED;
  }

  mt::USI Parameter::dialog(
    mt::VEC_STR &inputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    if ((!used && !selecting && isParent()) || isToddler()) {
      return question(inputs, paramData, lastCom);
    }

    // this was set to true after 'printDirectInputsQueueError' call
    selecting = false;

    // no need to set argument exclusively
    return Command::dialog(
      inputs, paramData, lastCom
    );
  }
}

#endif // __CLI_MENU__PARAMETER_CPP__