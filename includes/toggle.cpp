#ifndef __CLI_MENU__TOGGLE_CPP__
#define __CLI_MENU__TOGGLE_CPP__

#include "toggle.h"

namespace cli_menu {

  Toggle::Toggle(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    mt::CR_BOL accumulating_in,
    RESULT_CALLBACK callback_in,
    mt::CR_BOL propagatingCallback_in
  ) : Command(
    name_in, description_in,
    required_in, parent_in,
    accumulating_in, callback_in,
    propagatingCallback_in
  ) {}

  Toggle::Toggle(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    mt::CR_BOL accumulating_in,
    PLAIN_CALLBACK callback_in,
    mt::CR_BOL propagatingCallback_in
  ) : Command(
    name_in, description_in,
    required_in, parent_in,
    accumulating_in, callback_in,
    propagatingCallback_in
  ) {}

  Toggle::Toggle(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    mt::CR_BOL accumulating_in
  ) : Command(
    name_in, description_in,
    required_in, parent_in,
    accumulating_in
  ) {}

  void Toggle::initData(
    ResultInputs &resultInputs,
    mt::CR_VEC_BOL data
  ) {
    resultInputs.addConditions(name, data);
    useResultInputsIndex(resultInputs);
  }

  void Toggle::setData(
    ResultInputs &resultInputs,
    mt::CR_BOL condition
  ) {
    if (!used) initData(resultInputs, {condition});
    else resultInputs.pushCondition(resultInputsIndex, condition);
  }

  void Toggle::resetData(
    ResultInputs &resultInputs,
    mt::CR_BOL discarded
  ) {
    if (used) {
      // backup vector member
      conditions = resultInputs.getConditions(resultInputsIndex);

      // pop vector
      Command::resetData(resultInputs, discarded);

      // clean vector member
      if (!(discarded || accumulating)) {
        resultInputs.clearCondition(resultInputsIndex);
      }
    }
  }

  std::string Toggle::getDashedName() {
    return "--" + name;
  }

  std::string Toggle::getFullName(
    mt::CR_BOL useDash,
    mt::CR_BOL useInputType,
    mt::CR_BOL useLevelName,
    CR_CLR nameColor
  ) {
    std::string nameStr, text;

    // choose name
    if (useDash) {
      nameStr = getDashedName();
    }
    else nameStr = name;

    // set 'nameStr' color
    nameStr = Color::getString(
      nameStr, nameColor
    );

    // final assignments
    text = nameStr;

    // add level name
    if (useLevelName) {
      text += getLevelLabel();
    }

    return text;
  }

  mt::USI Toggle::match(
    mt::VEC_STR &directInputs,
    ResultInputs &resultInputs,
    Command **lastCom
  ) {
    if (directInputs.size()) {
      std::string copyName, copyInput;

      // copy to secure original strings
      copyMatchStrings(
        copyName, copyInput, directInputs.back()
      );

      if (copyName == DashTest::cleanDouble(copyInput)) {

        directInputs.pop_back();
        *lastCom = this;
        resetData(resultInputs, false);

        if (isParent()) {
          setData(resultInputs, true);

          // redirected to first child
          return matchTo(
            static_cast<Cm*>(children.front()),
            directInputs, resultInputs, lastCom
          );
        }
        // toddler
        else {
          // actually there is no need for argument
          if (directInputs.size() > 0) {

            int boolFlag = Control::booleanTest(
              mt_uti::StrTools::getStringToLowercase(
                *(directInputs.end() - 2)
              )
            );

            // between 1 or 2 is true
            if (boolFlag) {
              setData(resultInputs, Control::revealBoolean(boolFlag));
            }
            else setData(resultInputs, true);
          }

          return matchTo(
            getUnusedNeighbor(this),
            directInputs, resultInputs, lastCom
          );
        }
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
      return COMPLETED_FLAG;
    }
    // print error of incompleteness
    return FAILED_FLAG;
  }

  mt::USI Toggle::answerControl(
    mt::CR_STR controlStr,
    mt::VEC_STR &directInputs,
    ResultInputs &resultInputs,
    Command **lastCom
  ) {
    if (Control::backTest(controlStr)) {

      const mt::USI flag = isItPossibleToGoBack(
        directInputs, resultInputs, lastCom
      );

      if (flag != ERROR_FLAG) return flag;
    }
    else if (Control::clipboardTest(controlStr)) {
      printClipboardError();
    }
    else if (Control::enterTest(controlStr)) {
      // pointing to first child
      if (isParent()) {
        return dialogTo(
          static_cast<Cm*>(children.front()), directInputs, resultInputs, lastCom
        );
      }
      // need condition
      else if (!used && required) {
        Message::printNeatDialog(
          Message::ERROR_FLAG,
          "this " + getLevelName() + " needs a condition"
        );
      }
      // directly completed
      else if (doesUltimateAllowEnter()) {
        *lastCom = chooseLastCommand();
        setData(resultInputs, false);
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
        Control::getSharedFlag() == Control::NEXT,
        directInputs, resultInputs, lastCom
      );

      if (tryToSkipFlag != ERROR_FLAG) {
        return tryToSkipFlag;
      }
    }
    else if (Control::quitTest(controlStr)) {
      Command::stopThreadsLoop();
    }
    else if (Control::selectTest(controlStr)) {

      // only available for toddlers
      const mt::USI tryToSelectFlag = tryToSelect(
        directInputs, resultInputs, lastCom,
        "condition is given"
      );

      if (tryToSelectFlag != ERROR_FLAG) {
        return tryToSelectFlag;
      }
    }
    else Control::printError();

    return PASSED_FLAG;
  }

  mt::USI Toggle::answerSpecial(
    mt::CR_STR cinStr,
    mt::VEC_STR &directInputs,
    ResultInputs &resultInputs,
    Command **lastCom
  ) {
    int boolFlag = Control::booleanTest(cinStr);

    // condition input
    if (boolFlag) {
      *lastCom = chooseLastCommand();

      setData(
        resultInputs,
        Control::revealBoolean(boolFlag)
      );

      // inside ultimate only
      if (isSupporter()) return questionTo(
        getUnusedNeighbor(this), directInputs, resultInputs, lastCom
      );
      // dead end
      else if (isToddler()) {
        return COMPLETED_FLAG;
      }
      // back to this dialog
      return Command::dialog(
        directInputs, resultInputs, lastCom
      );
    }
    else Message::printNeatDialog(
      Message::ERROR_FLAG,
      "only accept boolean values"
    );

    return PASSED_FLAG;
  }

  mt::USI Toggle::dialog(
    mt::VEC_STR &directInputs,
    ResultInputs &resultInputs,
    Command **lastCom
  ) {
    const bool needQuestion = parent && !selecting && isToddler();

    if (conditions.empty()) {
      if (needQuestion) {
        return question(directInputs, resultInputs, lastCom);
      }
    }
    // remember the past
    else initData(resultInputs, conditions);

    // inverted in base method
    selecting = false;

    // no need to set condition exclusively on parent
    if (!used) setData(resultInputs, true);

    return Command::dialog(directInputs, resultInputs, lastCom);
  }
}

#endif // __CLI_MENU__TOGGLE_CPP__