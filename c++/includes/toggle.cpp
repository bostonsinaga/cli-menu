#ifndef __CLI_MENU__TOGGLE_CPP__
#define __CLI_MENU__TOGGLE_CPP__

#include "toggle.h"

namespace cli_menu {

  Toggle::Toggle(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    CR_SP_CALLBACK callback_in,
    Command *holder_in,
    mt::CR_BOL required_in,
    mt::CR_BOL accumulating_in
  ):
  Command(
    name_in, description_in,
    callback_in, holder_in,
    required_in, accumulating_in
  ) {}

  Toggle::Toggle(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    CR_SP_PLAIN_CALLBACK callback_in,
    Command *holder_in,
    mt::CR_BOL required_in,
    mt::CR_BOL accumulating_in
  ):
  Command(
    name_in, description_in,
    callback_in, holder_in,
    required_in, accumulating_in
  ) {}

  Toggle::Toggle(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    Command *holder_in,
    mt::CR_BOL required_in,
    mt::CR_BOL accumulating_in
  ):
  Command(
    name_in, description_in,
    holder_in, required_in,
    accumulating_in
  ) {}

  void Toggle::setData(
    ParamData &paramData,
    mt::CR_BOL cond
  ) {
    // safety for recall or pointing back/previous
    if (!used) updateRequiredSelf(false);

    used = true;
    paramData.conditions.push_back(cond);
    paramData.texts.push_back("");
    paramData.numbers.push_back({});
  }

  std::string Toggle::getDashedName() {
    return "--" + name;
  }

  std::string Toggle::getFullName() {
    return getDashedName() + getMainLabel();
  }

  Command *Toggle::match(
    mt::VEC_STR &inputs,
    ParamData &paramData
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

        if (isGroup()) {

          // callback or print error
          if (items.size() == 0) {
            setData(paramData, true);
            return this;
          }
          // redirected to first item
          return matchTo(items[0], inputs, paramData);
        }
        // supporter
        else {
          // actually there is no need for argument
          if (inputs.size() > 0) {
            int boolFlag = Control::booleanTest(
              mt_uti::StrTools::getStringToLowercase(inputs[i-1])
            );

            if (boolFlag) {
              setData(paramData, Control::revealBoolean(boolFlag));
            }
            else setData(paramData, true);
          }
          return matchTo(getUnusedNext(this), inputs, paramData);
        }
      }
      // pointing to neighbor or itself (Program)
      return matchTo(getUnusedNext(this), inputs, paramData);
    }
    // 'inputs' completion
    else if (Command::dialogued) {
      return dialogTo(holder, paramData);
    }
    // callback or print error
    else return this;
  }

  Command *Toggle::question(ParamData &paramData) {
    std::string buffer;
    printAfterBoundaryLine(getFullNameWithUltimate());

    while (true) {
      Message::setDialogInput(buffer);
      mt_uti::StrTools::changeStringToLowercase(buffer);
      int boolFlag = Control::booleanTest(buffer);

      if (boolFlag) {
        setData(paramData, Control::revealBoolean(boolFlag));
        return questionTo(getUnusedNext(this), paramData);
      }
      else if (Control::cancelTest(buffer)) {
        break; // returns nullptr below
      }
      else if (Control::enterTest(buffer)) {
        // directly completed
        if (getRequiredCount() == 0 && isOptional()) {
          return ultimate;
        }
        // required items are not complete
        else Command::printDialogError(cannotProcessErrorString);
      }
      else if (Control::nextTest(buffer)) {
        // proceed to next question
        if (isOptional()) {
          return questionTo(getUnusedNext(this), paramData);
        }
        // required items are not complete
        else Command::printDialogError(cannotSkipErrorString);
      }
      else if (Control::selectTest(buffer)) {
        return dialog(paramData);
      }
      else Command::printDialogError(
        "Only accept boolean values"
      );
    }

    return nullptr; // canceled
  }
}

#endif // __CLI_MENU__TOGGLE_CPP__