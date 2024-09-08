#ifndef __CLI_MENU__PARAMETER_CPP__
#define __CLI_MENU__PARAMETER_CPP__

#include "parameter.h"

namespace cli_menu {

  Parameter::Parameter(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL type_in,
    CR_SP_CALLBACK callback_in,
    mt::CR_BOL required_in
  ):
  Command::Command(name_in, description_in, callback_in, required_in) {
    type = type_in;
  }

  Parameter::Parameter(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL type_in,
    CR_SP_PLAIN_CALLBACK callback_in,
    mt::CR_BOL required_in
  ):
  Command::Command(name_in, description_in, callback_in, required_in) {
    type = type_in;
  }

  Parameter::Parameter(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL type_in,
    mt::CR_BOL required_in
  ):
  Command::Command(name_in, description_in, required_in) {
    type = type_in;
  }

  std::string Parameter::getStringifiedType() {
    if (type == TEXT) return "TEXT";
    return "NUMBER";
  }

  std::string Parameter::getDashedName() {
    return "-" + getName();
  }

  std::string Parameter::getFullName() {
    return getDashedName() + "[" + getStringifiedType() + "]";
  }

  void Parameter::pullData(
    ParamData &paramData,
    mt::VEC_UI &usedIndexes
  ) {
    if (type == TEXT) {
      paramData.texts.push_back(argument);
    }
    else {
      paramData.numbers.push_back(
        mt_uti::Scanner<double>::parseNumbers(argument)
      );
    }

    paramData.conditions.push_back(false);
    deepPull(paramData, usedIndexes);
  }

  bool Parameter::match(mt::VEC_STR &inputs) {

    bool incomplete = false;
    std::string thisName = getName();

    int begin = 1,
      end = inputs.size() - 1;

    mt::UI ultiTier = 1,
      thisTier = this->getTier();

    if (getUltimate()) {
      ultiTier = getUltimate()->getTier();
    }

    if (thisTier <= ultiTier) {
      begin = thisTier;
      end = thisTier + 1;
      if (begin >= inputs.size()) return false;
      else if (end >= inputs.size()) incomplete = true;
    }

    for (int i = begin; i < end; i++) {
      if (inputs[i].empty()) continue;

      int j = i + 1;
      std::string testName = inputs[i];

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