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
    CALLBACK callback_in,
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

  void Parameter::setData(
    ResultInputs &resultInputs,
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
        resultInputs.addTexts(name, {argument});
      }
      // whitespace is separator
      else resultInputs.addNumbers(
        name, mt_uti::Scanner<mt::LD>::parseNumbers(argument)
      );

      resultInputsIndex = resultInputs.getLastIndex();
      updateRequiredUsed(false);
    }
    // accumulated
    else {
      if (argumentType == TEXT) {
        resultInputs.pushText(resultInputsIndex, argument);
      }
      // whitespace is separator
      else resultInputs.pushNumbers(
        resultInputsIndex,
        mt_uti::Scanner<mt::LD>::parseNumbers(argument)
      );
    }
  }

  void Parameter::resetInput(
    ResultInputs &resultInputs,
    mt::CR_BOL discarded
  ) {
    if (used) {
      Command::resetInput(resultInputs, discarded);

      if (!(discarded || accumulating)) {

        if (argumentType == TEXT) {
          resultInputs.clearText(resultInputsIndex);
        }
        else resultInputs.clearNumber(resultInputsIndex);
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
    mt::VEC_STR &directInputs,
    ResultInputs &resultInputs,
    Command **lastCom
  ) {
    // only capture the last reversed 'directInputs'
    if (directInputs.size() > 0) {
      bool found = false;

      std::string copyInput;
      LINKED_LIST *continuation;

      if (isIndependence()) continuation = nullptr;
      else continuation = getContinuation();

      if (continuation) {
        Command::copyMatchInput(
          copyInput, directInputs[directInputs.size() - 1]
        );

        continuation->iterate<mt::CR_STR, bool&>(
          Parameter::checkArgument, copyInput, found
        );

        // question in the middle
        if (found && required) {

          Message::printNeatDialogError(
            "the '" + name + "' " + getLevelName() + needsArgStr, 1
          );

          return question(directInputs, resultInputs, lastCom);
        }
      }

      resetInput(resultInputs, false);
      setData(resultInputs, directInputs[directInputs.size() - 1]);
      directInputs.pop_back();

      if (isIndependence()) return FLAG::COMPLETED;
      return FLAG::PASSED;
    }

    return FLAG::ERROR;
  }

  mt::USI Parameter::notPopBackSet(
    mt::VEC_STR &directInputs,
    ResultInputs &resultInputs,
    Command **lastCom
  ) {
    // has no argument
    if (Command::dialogued) {

      Message::printNeatDialogError(
        "the last " + getLevelName() + needsArgStr, 1
      );

      return question(directInputs, resultInputs, lastCom);
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

  mt::USI Parameter::match(
    mt::VEC_STR &directInputs,
    ResultInputs &resultInputs,
    Command **lastCom
  ) {
    if (directInputs.size() > 0) {
      std::string copyName, copyInput;

      // copy to secure original strings
      Command::copyMatchStrings(
        copyName, copyInput, name, directInputs[directInputs.size() - 1]
      );

      if (copyName == DashTest::cleanSingle(copyInput)) {
        directInputs.pop_back();
        *lastCom = this;

        const mt::USI flag = popBackSet(
          directInputs, resultInputs, lastCom
        );

        // 'directInputs' may be empty
        if (flag == FLAG::PASSED) {

          // redirected to first child or unused neighbor
          return middleMatch(
            directInputs, resultInputs, lastCom, true
          );
        }
        else if (flag != FLAG::ERROR) {
          return flag;
        }

        // 'directInputs' is empty, could invoke 'question'
        return notPopBackSet(directInputs, resultInputs, lastCom);
      }

      // point to neighbor if input not matched
      return askNeighbor(directInputs, resultInputs, lastCom);
    }
    // 'directInputs' completion
    else if (isMatchNeedDialog()) {
      return dialogTo(
        static_cast<Cm*>(parent), directInputs, resultInputs, lastCom
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
    mt::VEC_STR &directInputs,
    ResultInputs &resultInputs,
    Command **lastCom,
    mt::CR_BOL needUnused
  ) {
    return matchTo(
      static_cast<Cm*>(getContinuation(needUnused)),
      directInputs, resultInputs, lastCom
    );
  }

  mt::USI Parameter::question(
    mt::VEC_STR &directInputs,
    ResultInputs &resultInputs,
    Command **lastCom
  ) {
    std::string buffer;
    resetInput(resultInputs, false);

    // question display on non-dependences
    if (isContainer()) questionedGroup = true;

    printAfterBoundaryLine(isContainer() ?
      getInlineRootNames() : getFullNameWithUltimate()
    );

    while (true) {
      Message::setDialogInput(buffer);

      // copy to secure original input
      std::string controlStr = mt_uti::StrTools::getStringToLowercase(buffer);

      if (Control::backTest(controlStr)) {

        const mt::USI flag = isItPossibleToGoBack(
          directInputs, resultInputs, lastCom
        );

        if (flag != FLAG::ERROR) return flag;
      }
      else if (Control::cancelTest(controlStr)) {
        break; // returns 'FLAG::CANCELED' below
      }
      else if (Control::enterTest(controlStr)) {
        // need argument
        if (!used && required) {
          Message::printNeatDialogError(
            "this " + getLevelName() + needsArgStr
          );
        }
        // question in the middle, back to match
        else if (!directInputs.empty()) {
          return middleMatch(directInputs, resultInputs, lastCom);
        }
        // pointing to first child
        else if (isParent()) {
          return dialogTo(
            static_cast<Cm*>(children.front()), directInputs, resultInputs, lastCom
          );
        }
        // directly completed
        else if (doesUltimateAllowEnter()) {
          *lastCom = chooseLastCommand();
          return FLAG::COMPLETED;
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
          Control::getSharedFlag() == Control::NEXT,
          directInputs, resultInputs, lastCom
        );

        if (tryToSkipFlag != FLAG::ERROR) {
          return tryToSkipFlag;
        }
      }
      else if (Control::selectTest(controlStr)) {

        const mt::USI tryToSelectFlag = tryToSelect(
          directInputs, resultInputs, lastCom,
          "arguments are given"
        );

        if (tryToSelectFlag != FLAG::ERROR) {
          return tryToSelectFlag;
        }
      }
      // value input
      else setData(resultInputs, buffer);
    }

    return FLAG::CANCELED;
  }

  mt::USI Parameter::dialog(
    mt::VEC_STR &directInputs,
    ResultInputs &resultInputs,
    Command **lastCom
  ) {
    if ((!used && !selecting && isParent()) || isToddler()) {
      return question(directInputs, resultInputs, lastCom);
    }

    // inverted in 'tryToSelect' method
    selecting = false;

    // no need to set argument exclusively
    return Command::dialog(
      directInputs, resultInputs, lastCom
    );
  }
}

#endif // __CLI_MENU__PARAMETER_CPP__