#ifndef __CLI_MENU__PARAMETER_H__
#define __CLI_MENU__PARAMETER_H__

#include "command.h"

namespace cli_menu {

  class Parameter {
  private:
    std::string argument;
    bool type = false;

    ~Parameter() {
      argument = "";
      type = false;
    }

    void pullData(
      mt::CR_VEC_STR &TEXTS,
      mt::CR_VEC_DBL &NUMBERS,
      mt::CR_VEC_BOL &CONDITIONS
    );

  public:
    enum {TEXT, NUMBER};

    Parameter(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL type_in,
      const std::shared_ptr<CALLBACK> &callback_in
    );

    bool getType() { return type; }
    std::string getStringifiedType();
    std::string getRawArgument() { return argument; }
    std::string getName(bool withSubname = false);
    bool match(mt::VEC_STR &inputs);
    int getInheritanceFlag() { return PARAMETER; }
  };
}

#define __CLI_MENU__PARAMETER_H__