#ifndef __CLI_MENU__PARAMETER_CPP__
#define __CLI_MENU__PARAMETER_CPP__

#include "parameter.h"

namespace cli_menu {

  Parameter::Parameter(
    mt::CR_STR name_in,
    mt::CR_STR description_in,
    mt::CR_BOL type_in,
    const std::shared_ptr<CALLBACK> &callback_in
  ):
  Command::Command(name_in, description_in, callback_in) {
    type = type_in;
  }

  std::string Parameter::getStringifiedType() {
    if (type == TEXT) return "TEXT";
    return "NUMBER";
  }

  std::string Parameter::getName(bool withSubname) {
    if (withSubname) {
      return name + "[" + getStringifiedType() + "]";
    }
    return name;
  }

  void Parameter::pullData(
    mt::CR_VEC_STR &TEXTS,
    mt::CR_VEC2_DBL &NUMBERS,
    mt::CR_VEC_BOL &CONDITIONS
  ) {
    if (type == TEXT) {
      TEXTS.push_back(argument);
    }
    else {
      NUMBERS.push_back(
        Reader::parseNumbers<double>(argument)
      );
    }

    CONDITIONS.push_back(false);
    deepPull(TEXTS, NUMBERS, CONDITIONS);
  }

  bool Parameter::match(mt::VEC_STR &inputs) {

    bool incomplete = false;
    int begin = 0, end = inputs.size() - 1;

    if (tier <= Command::ultimate->tier) {
      begin = tier;
      end = tier + 1;
      if (begin >= inputs.size()) return false;
      else if (end >= inputs.size()) incomplete = true;
    }

    for (int i = begin; i < end; i++) {

      int j = i + 1;
      std::string testName = inputs[i];

      if (DashTest::cleanSingle(testName) &&
        testName == name
      ) {
        inputs[i] = "";

        if (!incomplete) {
          argument = inputs[j];
          inputs[j] = "";
        }

        if (!Command::cleanCapturedPositionalInputs(inputs)) {
          if (tier > Command::ultimate->tier) {
            VecTools<std::string>::cutInterval(inputs, i, j);
          }
        }

        return !incomplete;
      }
    }
    return false;
  }
}

#define __CLI_MENU__PARAMETER_CPP__