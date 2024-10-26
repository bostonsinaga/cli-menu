#ifndef __CLI_MENU__PARAMETER_CPP__
#define __CLI_MENU__PARAMETER_CPP__

#include "parameter.h"

namespace cli_menu {

  Parameter::Parameter(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    mt::CR_BOL type_in,
    mt::CR_BOL accumulating_in,
    CR_SP_CALLBACK callback_in
  ):
  Command::Command(
    name_in, description_in,
    required_in, parent_in,
    callback_in
  ) {
    type = type_in;
    accumulating = accumulating_in;
  }

  Parameter::Parameter(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    mt::CR_BOL type_in,
    mt::CR_BOL accumulating_in,
    CR_SP_PLAIN_CALLBACK callback_in
  ):
  Command::Command(
    name_in, description_in,
    required_in, parent_in,
    callback_in
  ) {
    type = type_in;
    accumulating = accumulating_in;
  }

  Parameter::Parameter(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in,
    Command *parent_in,
    mt::CR_BOL type_in,
    mt::CR_BOL accumulating_in
  ):
  Command::Command(
    name_in, description_in,
    required_in, parent_in
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
    else {
      paramData.texts.push_back("");
      paramData.numbers.push_back(
        mt_uti::Scanner<double>::parseNumbers(str)
      );
    }

    paramData.conditions.push_back(false);
    used = true;
  }

  bool Parameter::popBackSet(
    mt::VEC_STR &inputs,
    ParamData &paramData
  ) {
    // only capture the last reversed 'inputs'
    if (inputs.size() > 0) {
      setData(paramData, inputs[inputs.size() - 1]);
      inputs.pop_back();
      return true;
    }
    return false;
  }

  mt::USI Parameter::notPopBackSet(
    ParamData &paramData,
    Command **lastCom
  ) {
    // has no argument
    if (Command::dialogued) {

      Message::printDialogError(
        "The last " + getLevelName() + " needs an argument.", 1
      );

      return question(paramData, lastCom);
    }
    // no dialog
    return FLAG::ERROR;
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

  bool Parameter::isGroupNeedQuestion() {
    return isSupporter() || questionedGroup;
  }

  bool Parameter::onEnter(
    ParamData &paramData,
    Command **lastCom
  ) {
    // need argument
    if (!used && isRequired()) {
      Message::printDialogError(
        "This " + getLevelName() + " needs an argument."
      );
    }
    // directly completed
    else if (getRequiredCount() == 0) {
      *lastCom = chooseLastCommand();
      return true;
    }
    // required items are not complete
    else printEnterError();

    return false;
  }

  mt::USI Parameter::match(
    mt::VEC_STR &inputs,
    ParamData &paramData,
    Command **lastCom
  ) {
    std::string copyName, copyInput;

    if (inputs.size() > 0) {

      // copy to secure original strings
      Command::copyMatchNames(
        copyName, copyInput,
        name, inputs[inputs.size() - 1]
      );

      if (copyName == DashTest::cleanSingle(copyInput)) {
        inputs.pop_back();
        *lastCom = this;

        if (isParent()) {

          // 'inputs' may be empty
          if (!popBackSet(inputs, paramData)) {
            return notPopBackSet(paramData, lastCom);
          }

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
          // has argument
          if (popBackSet(inputs, paramData)) {
            return matchTo(getUnusedNeighbor(this), inputs, paramData, lastCom);
          }
          // 'inputs' is empty
          return notPopBackSet(paramData, lastCom);
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

  mt::USI Parameter::question(
    ParamData &paramData,
    Command **lastCom
  ) {
    mt::VEC_STR valVec;
    std::string buffer;
    const bool notSupporter = !isSupporter();

    // to be able to display 'question' version in 'printDialogStatus' call
    if (notSupporter) questionedGroup = true;

    printAfterBoundaryLine(notSupporter ?
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
        if (onEnter(paramData, lastCom)) {
          checkout(paramData, valVec);
          return FLAG::COMPLETED;
        }
      }
      else if (Control::nextTest(controlStr)) {
        // proceed to next question
        if (isOptional() ||
          (isRequired() && used)
        ) {
          *lastCom = chooseLastCommand();
          checkout(paramData, valVec);

          // back to selection
          if (isGroup()) {
            return dialog(paramData, lastCom);
          }
          // toddler
          else if (!ultimate) {
            return dialogTo(getUnusedNeighbor(this), paramData, lastCom);
          }

          // directly ask first supporter
          return questionTo(
            isUltimate() ? static_cast<Cm*>(children[0]) : getUnusedNeighbor(this),
            paramData, lastCom
          );
        }
        // required items are not complete
        else printNextError();
      }
      else if (Control::selectTest(controlStr)) {
        return dialog(paramData, lastCom);
      }
      // value input
      else {
        bool isEmpty = true;

        // ignore empty 'buffer'
        for (char &ch : buffer) {
          if (ch != ' ' && ch != '\t' && ch != '\n') {
            isEmpty = false;
            break;
          }
        }

        if (isEmpty) continue;

        // to be able to '.enter'
        used = true;
        valVec.push_back(buffer);
      }
    }

    return FLAG::CANCELED;
  }
}

#endif // __CLI_MENU__PARAMETER_CPP__