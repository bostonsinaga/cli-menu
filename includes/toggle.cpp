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

  std::string Toggle::getFullName() {
    return getDashedName() + getMainLabel();
  }

  mt::USI Toggle::match(
    mt::VEC_STR &inputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    std::string copyName, copyInput;

    if (inputs.size() > 0) {
      const int i = inputs.size() - 1;

      Command::copyMatchStrings(
        copyName, copyInput,
        name, inputs[i]
      );

      if (copyName == DashTest::cleanDouble(copyInput)) {
        inputs.pop_back();
        *lastCom = this;

        if (isParent()) {
          setData(paramData, true);

          // redirected to first child
          return matchTo(
            static_cast<Cm*>(children.front()), inputs, paramData, lastCom
          );
        }
        // toddler
        else {
          // actually there is no need for argument
          if (inputs.size() > 0) {

            int boolFlag = Control::booleanTest(
              mt_uti::StrTools::getStringToLowercase(inputs[i-1])
            );

            // between 1 or 2 is true
            if (boolFlag) {
              setData(paramData, Control::revealBoolean(boolFlag));
            }
            else setData(paramData, true);
          }

          return matchTo(
            getUnusedNeighbor(this), inputs, paramData, lastCom
          );
        }
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

  mt::USI Toggle::question(
    mt::VEC_STR &inputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    std::string buffer;
    const bool notDependence = !isDependence();

    // question display on non-supporters
    if (notDependence) questionedGroup = true;

    printAfterBoundaryLine(notDependence ?
      getInlineRootNames() : getFullNameWithUltimate()
    );

    while (true) {
      Message::setDialogInput(buffer);
      mt_uti::StrTools::changeStringToLowercase(buffer);
      int boolFlag = Control::booleanTest(buffer);

      // value input
      if (boolFlag) {
        *lastCom = ultimate;

        setData(
          paramData,
          Control::revealBoolean(boolFlag)
        );

        // toddler
        return questionTo(
          getUnusedNeighbor(this), inputs, paramData, lastCom
        );
      }
      else if (Control::cancelTest(buffer)) {
        break; // returns 'FLAG::CANCELED' below
      }
      else if (Control::enterTest(buffer)) {
        // pointing to first child
        if (isParent()) {
          return dialogTo(
            static_cast<Cm*>(children.front()), inputs, paramData, lastCom
          );
        }
        // directly completed
        else if (doUltimateAllowEnter()) {
          *lastCom = chooseLastCommand();
          setData(paramData, false);
          return FLAG::COMPLETED;
        }
      }
      else if (Control::nextTest(buffer)) {
        // proceed to next question
        if (isOptional() || (isRequired() && used)) {
          *lastCom = ultimate;

          // pointing to neighbor
          if (notDependence) {
            if (next) return dialogTo(
              static_cast<Cm*>(next), inputs, paramData, lastCom
            );
            else printNullptrNextError();
          }
          // supporter
          else return questionTo(
            getUnusedNeighbor(this), inputs, paramData, lastCom
          );
        }
        // required items are not complete
        else printRequiredNextError();
      }
      else if (Control::selectTest(buffer)) {

        const mt::USI tryToSkipWithSelectionFlag = tryToSkipWithSelection(
          inputs, paramData, lastCom,
          "condition is given"
        );

        if (tryToSkipWithSelectionFlag != FLAG::ERROR) {
          return tryToSkipWithSelectionFlag;
        }
      }
      else Message::printNeatDialogError(
        "only accept boolean values"
      );
    }

    return FLAG::CANCELED;
  }

  mt::USI Toggle::dialog(
    mt::VEC_STR &inputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    if ((!selecting && isParent()) || isToddler()) {
      return question(inputs, paramData, lastCom);
    }

    // this may set to true in 'printDirectInputsQueueError' method
    selecting = false;

    // no need to set condition exclusively
    setData(paramData, true);
    return Command::dialog(inputs, paramData, lastCom);
  }
}

#endif // __CLI_MENU__TOGGLE_CPP__