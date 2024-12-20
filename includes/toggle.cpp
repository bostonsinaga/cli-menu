#ifndef __CLI_MENU__TOGGLE_CPP__
#define __CLI_MENU__TOGGLE_CPP__

#include "toggle.h"

namespace cli_menu {

  Toggle::Toggle(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    CR_SP_CALLBACK callback_in,
    mt::CR_BOL propagatingCallback_in
  ) : Command(
    name_in, description_in,
    required_in, parent_in,
    callback_in, propagatingCallback_in
  ) {}

  Toggle::Toggle(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    CR_SP_PLAIN_CALLBACK callback_in,
    mt::CR_BOL propagatingCallback_in
  ) : Command(
    name_in, description_in,
    required_in, parent_in,
    callback_in, propagatingCallback_in
  ) {}

  Toggle::Toggle(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in
  ):
  Command(
    name_in, description_in,
    required_in, parent_in
  ) {}

  void Toggle::setData(
    ParamData &paramData,
    mt::CR_BOL condition
  ) {
    if (!used) {
      paramData.conditions.push_back(condition);
      paramData.texts.push_back("");
      paramData.numbers.push_back({});
      paramDataIndex = paramData.conditions.size() - 1;
      updateRequiredUsed(false);
    }
    else paramData.conditions[paramDataIndex] = condition;
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
    ParamData &paramData,
    Command **lastCom
  ) {
    std::string copyName, copyInput;

    if (directInputs.size() > 0) {
      const int i = directInputs.size() - 1;

      Command::copyMatchStrings(
        copyName, copyInput,
        name, directInputs[i]
      );

      if (copyName == DashTest::cleanDouble(copyInput)) {
        directInputs.pop_back();
        *lastCom = this;

        if (isParent()) {
          setData(paramData, true);

          // redirected to first child
          return matchTo(
            static_cast<Cm*>(children.front()), directInputs, paramData, lastCom
          );
        }
        // toddler
        else {
          // actually there is no need for argument
          if (directInputs.size() > 0) {

            int boolFlag = Control::booleanTest(
              mt_uti::StrTools::getStringToLowercase(directInputs[i-1])
            );

            // between 1 or 2 is true
            if (boolFlag) {
              setData(paramData, Control::revealBoolean(boolFlag));
            }
            else setData(paramData, true);
          }

          return matchTo(
            getUnusedNeighbor(this), directInputs, paramData, lastCom
          );
        }
      }

      // point to neighbor if input not matched
      return askNeighbor(directInputs, paramData, lastCom);
    }
    // 'directInputs' completion
    else if (isMatchNeedDialog()) {
      return dialogTo(
        static_cast<Cm*>(parent), directInputs, paramData, lastCom
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

  mt::USI Toggle::question(
    mt::VEC_STR &directInputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    std::string buffer;

    // question display on non-supporters
    if (isContainer()) questionedGroup = true;

    printAfterBoundaryLine(isContainer() ?
      getInlineRootNames() : getFullNameWithUltimate()
    );

    while (true) {
      Message::setDialogInput(buffer);
      mt_uti::StrTools::changeStringToLowercase(buffer);
      int boolFlag = Control::booleanTest(buffer);

      // value input
      if (boolFlag) {
        *lastCom = chooseLastCommand();

        setData(
          paramData,
          Control::revealBoolean(boolFlag)
        );

        // inside ultimate only
        if (isDependence()) return questionTo(
          getUnusedNeighbor(this), directInputs, paramData, lastCom
        );
        // dead end
        else if (isIndependence()) {
          return FLAG::COMPLETED;
        }
        // back to this dialog
        return Command::dialog(
          directInputs, paramData, lastCom
        );
      }
      else if (Control::backTest(buffer)) {

        const mt::USI flag = isItPossibleToGoBack(
          directInputs, paramData, lastCom
        );

        if (flag != FLAG::ERROR) return flag;
      }
      else if (Control::cancelTest(buffer)) {
        break; // returns 'FLAG::CANCELED' below
      }
      else if (Control::enterTest(buffer)) {
        // pointing to first child
        if (isParent()) {
          return dialogTo(
            static_cast<Cm*>(children.front()), directInputs, paramData, lastCom
          );
        }
        // need condition
        else if (!used && required) {
          Message::printNeatDialogError(
            "this " + getLevelName() + " needs a condition"
          );
        }
        // directly completed
        else if (doesUltimateAllowEnter()) {
          *lastCom = chooseLastCommand();
          setData(paramData, false);
          return FLAG::COMPLETED;
        }
      }
      else if (Control::helpTest(buffer)) {
        printHelp();
      }
      else if (Control::listTest(buffer)) {
        printList();
      }
      else if (
        Control::nextTest(buffer) ||
        Control::previousTest(buffer)
      ) {
        const mt::USI tryToSkipFlag = tryToSkip(
          Control::getSharedFlag() == Control::NEXT,
          directInputs, paramData, lastCom
        );

        if (tryToSkipFlag != FLAG::ERROR) {
          return tryToSkipFlag;
        }
      }
      else if (Control::selectTest(buffer)) {

        // only available for toddlers
        const mt::USI tryToSelectFlag = tryToSelect(
          directInputs, paramData, lastCom,
          "condition is given"
        );

        if (tryToSelectFlag != FLAG::ERROR) {
          return tryToSelectFlag;
        }
      }
      else Message::printNeatDialogError(
        "only accept boolean values"
      );
    }

    return FLAG::CANCELED;
  }

  mt::USI Toggle::dialog(
    mt::VEC_STR &directInputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    // only for toddlers
    if (parent && !selecting && isToddler()) {
      return question(directInputs, paramData, lastCom);
    }

    // inverted in 'tryToSelect' method
    selecting = false;

    // no need to set condition exclusively on parent
    if (!used) setData(paramData, true);

    return Command::dialog(directInputs, paramData, lastCom);
  }
}

#endif // __CLI_MENU__TOGGLE_CPP__