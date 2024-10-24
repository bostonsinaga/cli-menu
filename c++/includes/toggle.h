#ifndef __CLI_MENU__TOGGLE_H__
#define __CLI_MENU__TOGGLE_H__

#include "command.h"

namespace cli_menu {

  class Toggle : public Command {
  private:
    void setData(
      ParamData &paramData,
      mt::CR_BOL cond
    );

  protected:
    bool onEnter(
      ParamData &paramData,
      Command **lastCom
    ) override;

    mt::USI match(
      mt::VEC_STR &inputs,
      ParamData &paramData,
      Command **lastCom
    ) override;

    // always called after ultimate (closed question)
    mt::USI question(
      ParamData &paramData,
      Command **lastCom
    ) override;

  public:
    Toggle(): Command() {}

    Toggle(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      CR_SP_CALLBACK callback_in
    );

    Toggle(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      CR_SP_PLAIN_CALLBACK callback_in
    );

    Toggle(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in
    );

    std::string getDashedName() override;
    std::string getFullName() override;
    mt::USI getInheritanceFlag() override { return TOGGLE; }
  };
}

#endif // __CLI_MENU__TOGGLE_H__