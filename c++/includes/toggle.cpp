#ifndef __CLI_MENU__TOGGLE_CPP__
#define __CLI_MENU__TOGGLE_CPP__

#include "toggle.h"

namespace cli_menu {

  Toggle::Toggle(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    CR_SP_CALLBACK callback_in,
    mt::CR_BOL required_in
  ):
  Command(name_in, description_in, callback_in, required_in) {}

  Toggle::Toggle(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    CR_SP_PLAIN_CALLBACK callback_in,
    mt::CR_BOL required_in
  ):
  Command(name_in, description_in, callback_in, required_in) {}

  Toggle::Toggle(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL required_in
  ):
  Command(name_in, description_in, required_in) {}

  std::string Toggle::getDashedName() {
    return "--" + getName();
  }

  std::string Toggle::getFullName() {
    return getDashedName();
  }

  void Toggle::setData(mt::CR_BOL cond, mt::CR_STR str) {
    condition = cond;
  }

  void Toggle::pullData(
    ParamData &paramData,
    mt::VEC_UI &usedIndexes
  ) {
    paramData.conditions.push_back(condition);
    paramData.texts.push_back("");
    paramData.numbers.push_back({});
    deepPull(paramData, usedIndexes);
  }

  bool Toggle::match(mt::VEC_STR &inputs) {
    std::string thisName = getName();

    mt::UI begin = 1,
      end = inputs.size(),
      ultiTier = 1,
      thisTier = this->getTier();

    if (getUltimate()) {
      ultiTier = getUltimate()->getTier();
    }

    if (thisTier <= ultiTier) {
      begin = thisTier;
      end = thisTier + 1;
      if (begin >= inputs.size()) return false;
    }

    for (int i = begin; i < end; i++) {
      if (inputs[i].empty()) continue;

      std::string testName = inputs[i];

      if (DashTest::cleanDouble(testName) &&
        testName == thisName
      ) {
        condition = true;
        inputs[i] = "";
        return true;
      }
    }
    return false;
  }
}

#endif // __CLI_MENU__TOGGLE_CPP__