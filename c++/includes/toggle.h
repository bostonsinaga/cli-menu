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
    // always called after ultimate (closed question)
    Command *question(ParamData &paramData) override;

    Command *match(
      mt::VEC_STR &inputs,
      ParamData &paramData
    ) override;

  public:
    Toggle(): Command() {}

    Toggle(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      CR_SP_CALLBACK callback_in,
      Command *holder_in = nullptr,
      mt::CR_BOL required_in = false,
      mt::CR_BOL accumulating_in = false
    );

    Toggle(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      CR_SP_PLAIN_CALLBACK callback_in,
      Command *holder_in = nullptr,
      mt::CR_BOL required_in = false,
      mt::CR_BOL accumulating_in = false
    );

    Toggle(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      Command *holder_in = nullptr,
      mt::CR_BOL required_in = false,
      mt::CR_BOL accumulating_in = false
    );

    std::string getDashedName() override;
    std::string getFullName() override;
    mt::USI getInheritanceFlag() override { return TOGGLE; }
  };
}

#endif // __CLI_MENU__TOGGLE_H__