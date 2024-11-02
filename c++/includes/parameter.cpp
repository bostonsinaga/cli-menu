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
      if (type == TEXT) {
        paramData.texts.push_back(argument);
        paramData.numbers.push_back({});
      }
      // space or newline is a separator
      else {
        paramData.texts.push_back("");
        paramData.numbers.push_back(
          mt_uti::Scanner<double>::parseNumbers(argument)
        );
      }

      paramData.conditions.push_back(false);
      paramDataIndex = paramData.texts.size() - 1;
      updateRequiredUsed(false);
    }
    // accumulated to get multiline input
    else {
      if (type == TEXT) {
        paramData.texts[paramDataIndex] += argument;
      }
      // space or newline is a separator
      else mt_uti::VecTools<double>::concat(
        paramData.numbers[paramDataIndex],
        mt_uti::Scanner<double>::parseNumbers(argument)
      );
    }
  }

  void Parameter::resetArgument(ParamData &paramData) {
    if (used && !accumulating) {
      if (type == TEXT) {
        paramData.texts[paramDataIndex] = "";
      }
      else paramData.numbers[paramDataIndex] = {};
    }
  }

  bool Parameter::popBackSet(
    mt::VEC_STR &inputs,
    ParamData &paramData
  ) {
    // only capture the last reversed 'inputs'
    if (inputs.size() > 0) {
      resetArgument(paramData);
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
    std::string usedStr;

    if (!used) return "emp";
    else if (accumulating) usedStr = "acc";
    else usedStr = "cor";

    return Color::getString(usedStr, Color::MAGENTA);
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
        // toddler
        else {
          // has argument
          if (popBackSet(inputs, paramData)) {
            return matchTo(
              getUnusedNeighbor(this),
              inputs, paramData, lastCom
            );
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
    std::string buffer;
    const bool notSupporter = !isSupporter();
    resetArgument(paramData);

    // question display on non-supporters
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
        // need argument
        if (!used && isRequired()) {
          Message::printDialogError(
            "This " + getLevelName() + " needs an argument."
          );
        }
        // pointing to first child
        else if (isParent()) {
          return dialogTo(
            static_cast<Cm*>(children[0]), paramData, lastCom
          );
        }
        // directly completed
        else if (doUltimateAllowEnter()) {
          *lastCom = chooseLastCommand();
          return FLAG::COMPLETED;
        }
      }
      else if (Control::nextTest(controlStr)) {
        // proceed to next question
        if (isOptional() || (isRequired() && used)) {
          *lastCom = chooseLastCommand();

          if (notSupporter) {
            // back to selection
            if (isParent()) {
              return dialog(paramData, lastCom);
            }
            // pointing to neighbor
            else if (next) return dialogTo(
              static_cast<Cm*>(next), paramData, lastCom
            );
            else printSingleNextError();
          }
          // supporter
          else return questionTo(
            getUnusedNeighbor(this), paramData, lastCom
          );
        }
        // required items are not complete
        else printRequiredNextError();
      }
      else if (Control::selectTest(controlStr)) {
        return dialog(paramData, lastCom);
      }
      // value input
      else setData(paramData, buffer);
    }

    return FLAG::CANCELED;
  }

  mt::USI Parameter::dialog(
    ParamData &paramData,
    Command **lastCom
  ) {
    if (!used || isToddler()) {
      return question(paramData, lastCom);
    }

    // no need to set argument exclusively
    return Command::dialog(paramData, lastCom);
  }
}

#endif // __CLI_MENU__PARAMETER_CPP__