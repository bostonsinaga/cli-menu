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

  std::string Toggle::getDashedName() {
    return "--" + name;
  }

  std::string Toggle::getFullName() {
    return getDashedName() + getMainLabel();
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
    std::string thisName = name;

    if (Command::isTemporaryLetterCaseChange()) {
      mt_uti::StrTools::changeStringToUppercase(thisName);
    }

    mt::UI begin = 1,
      end = inputs.size(),
      ultiLevel = 1,
      thisLevel = level;

    if (ultimate) {
      ultiLevel = ultimate->getLevel();
    }

    if (thisLevel <= ultiLevel) {
      begin = thisLevel;
      end = thisLevel + 1;
      if (begin >= inputs.size()) return false;
    }

    for (int i = begin; i < end; i++) {
      if (inputs[i].empty()) continue;

      std::string testName = inputs[i];
      Command::onFreeChangeInputLetterCase(testName);

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