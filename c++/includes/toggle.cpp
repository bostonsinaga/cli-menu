#ifndef __CLI_MENU__TOGGLE_CPP__
#define __CLI_MENU__TOGGLE_CPP__

#include "toggle.h"

namespace cli_menu {

  Toggle::Toggle(
    mt::CR_VEC_STR names_in,
    mt::CR_VEC_STR descriptions_in,
    const std::shared_ptr<CALLBACK> &callback_in
  ):
  Command(name_in, description_in, callback_in) {}

  void Toggle::pullData(
    mt::CR_VEC_STR &TEXTS,
    mt::CR_VEC2_DBL &NUMBERS,
    mt::CR_VEC_BOL &CONDITIONS
  ) {
    CONDITIONS.push_back(condition);
    TEXTS.push_back("");
    NUMBERS.push_back({});
    deepPull(TEXTS, NUMBERS, CONDITIONS);
  }

  bool Toggle::getCondition() { return condition; }

  bool Toggle::match(mt::VEC_STR &inputs) {
    int begin = 0, end = inputs.size();

    if (tier <= Command::ultimate->tier) {
      begin = tier;
      end = tier + 1;
      if (begin >= inputs.size()) return false;
    }

    for (int i = begin; i < end; i++) {
      std::string testName = inputs[i];

      if (DashTest::cleanDouble(testName) &&
        testName == name
      ) {
        condition = true;
        inputs[i] = "";

        if (!Command::cleanCapturedPositionalInputs(inputs)) {
          if (tier > Command::ultimate->tier) {
            VecTools<std::string>::cutSingle(inputs, i);
          }
        }

        return true;
      }
    }
    return false;
  }
}

#endif // __CLI_MENU__TOGGLE_CPP__