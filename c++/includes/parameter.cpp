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
    required_in, accumulating_in
  ) { type = type_in; }

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
    required_in, accumulating_in
  ) { type = type_in; }

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
    holder_in, required_in,
    accumulating_in
  ) { type = type_in; }

  void Parameter::setData(mt::CR_STR str) {
    used = true;
    argument = str;
    if (isSupporter()) updateRequiredSelf(false);
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

  void Parameter::pullData(
    ParamData &paramData,
    mt::VEC_UI &usedIndexes
  ) {
    if (isSupporter()) {
      if (type == TEXT) {
        paramData.texts.push_back(argument);
        paramData.numbers.push_back({});
      }
      else paramData.numbers.push_back(
        mt_uti::Scanner<double>::parseNumbers(argument)
      );
      paramData.conditions.push_back(false);
    }
    deepPull(paramData, usedIndexes);
  }

  bool Parameter::match(mt::VEC_STR &inputs) {

    bool incomplete = false;
    std::string thisName = name;

    if (Command::isTemporaryLetterCaseChange()) {
      mt_uti::StrTools::changeStringToUppercase(thisName);
    }

    int begin = 1,
      end = inputs.size() - 1;

    mt::UI ultiLevel = 1,
      thisLevel = level;

    if (ultimate) {
      ultiLevel = ultimate->getLevel();
    }

    if (thisLevel <= ultiLevel) {
      begin = thisLevel;
      end = thisLevel + 1;
      if (begin >= inputs.size()) return false;
      else if (end >= inputs.size()) incomplete = true;
    }

    for (int i = begin; i < end; i++) {
      if (inputs[i].empty()) continue;

      int j = i + 1;
      std::string testName = inputs[i];
      Command::onFreeChangeInputLetterCase(testName);

      if (DashTest::cleanSingle(testName) &&
        testName == thisName
      ) {
        inputs[i] = "";

        if (!incomplete) {
          argument = inputs[j];
          inputs[j] = "";
        }

        return !incomplete;
      }
    }
    return false;
  }
}

#endif // __CLI_MENU__PARAMETER_CPP__