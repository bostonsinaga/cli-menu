#ifndef __CLI_MENU__PARAMETER_CPP__
#define __CLI_MENU__PARAMETER_CPP__

#include "parameter.h"

namespace cli_menu {

  Parameter::Parameter(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL type_in,
    CR_SP_CALLBACK callback_in,
    Command *holder_in,
    mt::CR_BOL required_in,
    mt::CR_BOL accumulating_in
  ):
  Command::Command(
    name_in, description_in,
    callback_in, holder_in,
    required_in
  ) {
    type = type_in;
    accumulating = accumulating_in;
  }

  Parameter::Parameter(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL type_in,
    CR_SP_PLAIN_CALLBACK callback_in,
    Command *holder_in,
    mt::CR_BOL required_in,
    mt::CR_BOL accumulating_in
  ):
  Command::Command(
    name_in, description_in,
    callback_in, holder_in,
    required_in
  ) {
    type = type_in;
    accumulating = accumulating_in;
  }

  Parameter::Parameter(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL type_in,
    Command *holder_in,
    mt::CR_BOL required_in,
    mt::CR_BOL accumulating_in
  ):
  Command::Command(
    name_in, description_in,
    holder_in, required_in
  ) {
    type = type_in;
    accumulating = accumulating_in;
  }

  void Parameter::setData(
    ParamData &paramData,
    mt::CR_STR str
  ) {
    if (type == TEXT) {
      paramData.texts.push_back(str);
      paramData.numbers.push_back({});
    }
    // space or newline is a separator
    else paramData.numbers.push_back(
      mt_uti::Scanner<double>::parseNumbers(str)
    );

    paramData.conditions.push_back(false);
    updateRequiredSelf(false);
  }

  void Parameter::checkout(
    ParamData &paramData,
    mt::CR_VEC_STR valVec
  ) {
    // concatenate multiline strings from 'valVec'
    std::string united = mt_uti::StrTools::uniteVector(valVec, "\n");

    // recall can be accumulated in argument
    setData(
      paramData,
      accumulating ? argument + united : united
    );
  }

  std::string Parameter::getStringifiedType() {
    if (type == TEXT) return "text";
    return "number";
  }

  std::string Parameter::getDashedName() {
    return "-" + name;
  }

  std::string Parameter::getFullName() {
    return getDashedName()
      + Color::getString(
        "<" + getStringifiedType() + ">",
        Command::usingDashesBoundaryLine ? Color::AZURE : Color(0, 95, 223)
      ) + getMainLabel();
  }

  std::string Parameter::getFillingStatusString() {
    if (!used) return "emp";
    else if (accumulating) return "acc";
    return "cor";
  }

  mt::USI Parameter::match(
    mt::VEC_STR &inputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    std::string copyName, copyInput;

    if (inputs.size() > 0) {
      const int i = inputs.size() - 1;

      // copy to secure original strings
      Command::copyMatchNames(
        copyName, copyInput,
        name, inputs[i]
      );

      if (copyName == DashTest::cleanSingle(copyInput)) {
        inputs.pop_back();
        *lastCom = this;

        if (isGroup()) {

          // call default callback or print error
          if (items.size() == 0) {

            // only capture the last reversed 'inputs'
            if (inputs.size() > 0) {
              setData(paramData, inputs[i-1]);
              inputs.pop_back();
            }

            return FLAG::COMPLETED;
          }

          // redirected to first item
          return matchTo(items[0], inputs, paramData, lastCom);
        }
        // supporter
        else {
          // 'inputs' has arguments
          if (inputs.size() > 0) {
            setData(paramData, inputs[i-1]);
            inputs.pop_back();
            return matchTo(getUnusedNext(this), inputs, paramData, lastCom);
          }
          // 'inputs' has no arguments
          else if (Command::dialogued) {
            return dialog(paramData, lastCom);
          }
          else return FLAG::ERROR;
        }
      }

      // pointing to neighbor
      return matchTo(getUnusedNext(this), inputs, paramData, lastCom);
    }
    // 'inputs' completion
    else if (Command::dialogued &&
      (isGroup() || (ultimate && getRequiredCount() != 0))
    ) {
      return dialogTo(holder, paramData, lastCom);
    }
    // invoke callback
    else if (ultimate && getRequiredCount() == 0) {
      *lastCom = this;
      return FLAG::COMPLETED;
    }
    // print error of incompleteness
    return FLAG::ERROR;
  }

  mt::USI Parameter::question(
    ParamData &paramData,
    Command **lastCom
  ) {
    mt::VEC_STR valVec;
    std::string buffer;

    printAfterBoundaryLine(getFullNameWithUltimate());

    while (true) {
      Message::setDialogInput(buffer);

      // copy to secure original input
      std::string controlStr = mt_uti::StrTools::getStringToLowercase(buffer);

      // control input
      if (Control::cancelTest(controlStr)) {
        break; // returns 'FLAG::CANCELED' below
      }
      else if (Control::enterTest(controlStr)) {
        // directly completed
        if (getRequiredCount() == 0) {
          *lastCom = ultimate;
          checkout(paramData, valVec);
          return FLAG::COMPLETED;
        }
        // required items are not complete
        else {
          Message::printDialogError(cannotProcessErrorString);
        }
      }
      else if (Control::nextTest(controlStr)) {
        // proceed to next question
        if (isOptional() ||
          (isRequired() && used)
        ) {
          *lastCom = ultimate;
          checkout(paramData, valVec);
          return questionTo(getUnusedNext(this), paramData, lastCom);
        }
        // required items are not complete
        else {
          Message::printDialogError(cannotSkipErrorString);
        }
      }
      else if (Control::selectTest(controlStr)) {
        return dialog(paramData, lastCom);
      }
      // value input
      else {
        // to be able to '.enter'
        updateRequiredSelf(false);
        valVec.push_back(buffer);
      }
    }

    return FLAG::CANCELED;
  }
}

#endif // __CLI_MENU__PARAMETER_CPP__