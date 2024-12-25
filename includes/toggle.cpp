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
    CALLBACK callback_in,
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

  void Toggle::setData(
    ResultInputs &resultInputs,
    mt::CR_BOL condition
  ) {
    if (!used) {
      resultInputs.conditions.push_back({condition});
      resultInputs.texts.push_back({});
      resultInputs.numbers.push_back({});
      paramDataIndex = resultInputs.conditions.size() - 1;
      updateRequiredUsed(false);
    }
    else resultInputs.conditions[paramDataIndex].push_back(condition);
  }

  void Toggle::resetArgument(ResultInputs &resultInputs) {
    if (used && !accumulating) {
      resultInputs.conditions[paramDataIndex] = {};
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
        resetArgument(resultInputs);

        if (isParent()) {
          setData(resultInputs, true);

          // redirected to first child
          return matchTo(
            static_cast<Cm*>(children.front()), directInputs, resultInputs, lastCom
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
              setData(resultInputs, Control::revealBoolean(boolFlag));
            }
            else setData(resultInputs, true);
          }

          return matchTo(
            getUnusedNeighbor(this), directInputs, resultInputs, lastCom
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
      return FLAG::COMPLETED;
    }
    // print error of incompleteness
    return FLAG::FAILED;
  }

  mt::USI Toggle::question(
    mt::VEC_STR &directInputs,
    ResultInputs &resultInputs,
    Command **lastCom
  ) {
    std::string buffer;
    resetArgument(resultInputs);

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
          resultInputs,
          Control::revealBoolean(boolFlag)
        );

        // inside ultimate only
        if (isDependence()) return questionTo(
          getUnusedNeighbor(this), directInputs, resultInputs, lastCom
        );
        // dead end
        else if (isIndependence()) {
          return FLAG::COMPLETED;
        }
        // back to this dialog
        return Command::dialog(
          directInputs, resultInputs, lastCom
        );
      }
      else if (Control::backTest(buffer)) {

        const mt::USI flag = isItPossibleToGoBack(
          directInputs, resultInputs, lastCom
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
            static_cast<Cm*>(children.front()), directInputs, resultInputs, lastCom
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
          setData(resultInputs, false);
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
          directInputs, resultInputs, lastCom
        );

        if (tryToSkipFlag != FLAG::ERROR) {
          return tryToSkipFlag;
        }
      }
      else if (Control::selectTest(buffer)) {

        // only available for toddlers
        const mt::USI tryToSelectFlag = tryToSelect(
          directInputs, resultInputs, lastCom,
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
    ResultInputs &resultInputs,
    Command **lastCom
  ) {
    // only for toddlers
    if (parent && !selecting && isToddler()) {
      return question(directInputs, resultInputs, lastCom);
    }

    // inverted in 'tryToSelect' method
    selecting = false;

    // no need to set condition exclusively on parent
    if (!used) setData(resultInputs, true);

    return Command::dialog(directInputs, resultInputs, lastCom);
  }
}

#endif // __CLI_MENU__TOGGLE_CPP__