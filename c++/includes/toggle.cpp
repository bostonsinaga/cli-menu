#ifndef __CLI_MENU__TOGGLE_CPP__
#define __CLI_MENU__TOGGLE_CPP__

#include "toggle.h"

namespace cli_menu {

  Toggle::Toggle(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    CR_SP_CALLBACK callback_in
  ):
  Command(
    name_in, description_in,
    required_in, parent_in,
    callback_in
  ) {}

  Toggle::Toggle(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    CR_SP_PLAIN_CALLBACK callback_in
  ):
  Command(
    name_in, description_in,
    required_in, parent_in,
    callback_in
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
    mt::CR_BOL cond
  ) {
    paramData.conditions.push_back(cond);
    paramData.texts.push_back("");
    paramData.numbers.push_back({});
    updateRequiredSelf(false);
  }

  std::string Toggle::getDashedName() {
    return "--" + name;
  }

  std::string Toggle::getFullName() {
    return getDashedName() + getMainLabel();
  }

  bool Toggle::onEnter(
    ParamData &paramData,
    Command **lastCom
  ) {
    // directly completed
    if (getRequiredCount() == 0) {
      *lastCom = ultimate ? ultimate : this;
      return true;
    }
    // required items are not complete
    else printEnterError();

    return false;
  }

  mt::USI Toggle::match(
    mt::VEC_STR &inputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    std::string copyName, copyInput;

    if (inputs.size() > 0) {
      const int i = inputs.size() - 1;

      Command::copyMatchNames(
        copyName, copyInput,
        name, inputs[i]
      );

      if (copyName == DashTest::cleanDouble(copyInput)) {
        inputs.pop_back();
        *lastCom = this;

        if (isParent()) {
          setData(paramData, true);

          // invoke callback or print error
          if (children.size() == 0) {
            return FLAG::ERROR;
          }

          // redirected to first child
          return matchTo(
            static_cast<Cm*>(children[0]),
            inputs, paramData, lastCom
          );
        }
        // supporter
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

          return matchTo(getUnusedNeighbor(this), inputs, paramData, lastCom);
        }
      }

      // point to neighbor if input not matched
      return askNeighbor(inputs, paramData, lastCom);
    }
    // 'inputs' completion
    else if (isMatchNeedDialog()) {
      return dialogTo(
        static_cast<Cm*>(parent),
        paramData, lastCom
      );
    }
    // invoke callback
    else if (ultimate && getRequiredCount() == 0) {
      *lastCom = ultimate;
      return FLAG::COMPLETED;
    }
    // print error of incompleteness
    return FLAG::ERROR;
  }

  mt::USI Toggle::question(
    ParamData &paramData,
    Command **lastCom
  ) {
    std::string buffer;

    printAfterBoundaryLine(!isSupporter() ?
      getInlineRootNames() : getFullNameWithUltimate()
    );

    while (true) {
      Message::setDialogInput(buffer);
      mt_uti::StrTools::changeStringToLowercase(buffer);
      int boolFlag = Control::booleanTest(buffer);

      if (boolFlag) {
        *lastCom = ultimate;
        setData(paramData, Control::revealBoolean(boolFlag));
        return questionTo(getUnusedNeighbor(this), paramData, lastCom);
      }
      else if (Control::cancelTest(buffer)) {
        break; // returns 'FLAG::CANCELED' below
      }
      else if (Control::enterTest(buffer)) {
        if (onEnter(paramData, lastCom)) {
          if (!used) setData(paramData, false);
          return FLAG::COMPLETED;
        }
      }
      else if (Control::nextTest(buffer)) {
        // proceed to next question
        if (isOptional() ||
          (isRequired() && used)
        ) {
          *lastCom = ultimate;
          return questionTo(getUnusedNeighbor(this), paramData, lastCom);
        }
        // required items are not complete
        else printNextError();
      }
      else if (Control::selectTest(buffer)) {
        return dialog(paramData, lastCom);
      }
      else Message::printDialogError("Only accept boolean values.");
    }

    return FLAG::CANCELED;
  }
}

#endif // __CLI_MENU__TOGGLE_CPP__