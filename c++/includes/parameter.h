#ifndef __CLI_MENU__PARAMETER_H__
#define __CLI_MENU__PARAMETER_H__

#include "command.h"

namespace cli_menu {

  class Parameter : public Command {
  private:
    std::string argument;
    bool type = false;

    ~Parameter() {
      argument = "";
      type = false;
    }

  public:
    enum {TEXT, NUMBER};
    Parameter(): Command() {}

    Parameter(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL type_in,
      CR_SP_CALLBACK callback_in,
      mt::CR_BOL required_in = false
    );

    Parameter(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL type_in,
      CR_SP_PLAIN_CALLBACK callback_in,
      mt::CR_BOL required_in = false
    );

    Parameter(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL type_in,
      mt::CR_BOL required_in = false
    );

    bool match(mt::VEC_STR &inputs);
    bool getType() { return type; }
    std::string getStringifiedType();
    std::string getDashedName();
    std::string getFullName();
    std::string getRawArgument() { return argument; }
    mt::USI getInheritanceFlag() { return PARAMETER; }

    void setData(
      mt::CR_BOL cond = false,
      mt::CR_STR str = ""
    );

    void pullData(
      ParamData &paramData,
      mt::VEC_UI &usedIndexes
    );
  };
}

#endif // __CLI_MENU__PARAMETER_H__