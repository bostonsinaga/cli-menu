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

  void Toggle::initData(mt::CR_VEC_BOL data) {
    INPUTS.result.addConditions(name, data);
    useResultInputsIndex();
  }

  void Toggle::setData(mt::CR_BOL condition) {
    if (!used) {
      initData({condition});
    }
    else INPUTS.result.pushCondition(
      INPUTS.index, condition
    );
  }

  void Toggle::resetData(
    mt::CR_BOL discarded
  ) {
    if (used) {
      // backup vector member
      conditions = INPUTS.result.getConditions(INPUTS.index);

      // pop vector
      Command::resetData(discarded);

      // clean vector member
      if (!(discarded || accumulating)) {
        INPUTS.result.clearCondition(INPUTS.index);
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

  mt::USI Toggle::match() {

    if (INPUTS.direct.size()) {
      std::string copyName, copyInput;

      // copy to secure original strings
      copyMatchStrings(
        copyName, copyInput, INPUTS.direct.back()
      );

      if (copyName == DashTest::cleanDouble(copyInput)) {

        INPUTS.direct.pop_back();
        Command::lastCom = this;
        resetData(false);

        if (isParent()) {
          setData(true);

          // redirected to first child
          return matchTo(
            static_cast<Cm*>(children.front())
          );
        }
        // toddler
        else {
          // actually there is no need for argument
          if (INPUTS.direct.size() > 0) {

            int boolFlag = Control::booleanTest(
              mt_uti::StrTools::getStringToLowercase(
                *(INPUTS.direct.end() - 2)
              )
            );

            // between 1 or 2 is true
            if (boolFlag) {
              setData(Control::revealBoolean(boolFlag));
            }
            else setData(true);
          }

          return matchTo(getUnusedNeighbor(this));
        }
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

  mt::USI Toggle::answerControl(mt::CR_STR controlStr) {

    if (Control::backTest(controlStr)) {
      const mt::USI flag = tryToGoBack();

      if (flag != ERROR_FLAG) {
        return flag;
      }
    }
    else if (Control::clipboardTest(controlStr)) {
      printClipboardError();
    }
    else if (Control::enterTest(controlStr)) {
      // pointing to first child
      if (isParent()) {
        return dialogTo(
          static_cast<Cm*>(children.front())
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
        Command::lastCom = chooseLastCommand();
        setData(false);
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

      // only available for toddlers
      const mt::USI tryToSelectFlag = tryToSelect(
        "condition is given"
      );

      if (tryToSelectFlag != ERROR_FLAG) {
        return tryToSelectFlag;
      }
    }
    else Control::printError();

    return PASSED_FLAG;
  }

  mt::USI Toggle::answerSpecial(mt::CR_STR cinStr) {

    int boolFlag = Control::booleanTest(cinStr);

    // condition input
    if (boolFlag) {

      Command::lastCom = chooseLastCommand();
      setData(Control::revealBoolean(boolFlag));

      // inside ultimate only
      if (isSupporter()) return questionTo(
        getUnusedNeighbor(this)
      );
      // dead end
      else if (isToddler()) {
        return COMPLETED_FLAG;
      }
      // back to this dialog
      return Command::dialog();
    }
    else Message::printNeatDialog(
      Message::ERROR_FLAG,
      "only accept boolean values"
    );

    return PASSED_FLAG;
  }

  mt::USI Toggle::dialog() {

    const bool needQuestion = parent && !selecting && isToddler();

    if (conditions.empty()) {
      if (needQuestion) {
        return question();
      }
    }
    // remember the past
    else initData(conditions);

    // inverted in base method
    selecting = false;

    // no need to set condition exclusively on parent
    if (!used) setData(true);

    return Command::dialog();
  }
}

#endif // __CLI_MENU__TOGGLE_CPP__