#ifndef __CLI_MENU__TOGGLE_H__
#define __CLI_MENU__TOGGLE_H__

#include "command.h"

namespace cli_menu {

  class Toggle : public Command {
  private:
    bool condition = false;
    ~Toggle() { condition = false; }

  public:
    Toggle(): Command() {}

    Toggle(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      CR_SP_CALLBACK callback_in,
      mt::CR_BOL required_in = false
    );

    Toggle(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      CR_SP_PLAIN_CALLBACK callback_in,
      mt::CR_BOL required_in = false
    );

    Toggle(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in = false
    );

    bool match(mt::VEC_STR &inputs);
    bool getCondition() { return condition; }
    mt::USI getInheritanceFlag() { return TOGGLE; }
    std::string getDashedName();
    std::string getFullName();
    void setData(mt::CR_BOL cond) { condition = cond; }

    void pullData(
      ParamData &paramData,
      mt::VEC_UI &usedIndexes
    );
  };
}

#endif // __CLI_MENU__TOGGLE_H__