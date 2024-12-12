#ifndef __CLI_MENU__TOGGLE_H__
#define __CLI_MENU__TOGGLE_H__

#include "command.h"

namespace cli_menu {

  class Toggle : public Command {
  private:
    void setData(
      ParamData &paramData,
      mt::CR_BOL condition
    ) override;

  protected:
    mt::USI match(
      mt::VEC_STR &inputs,
      ParamData &paramData,
      Command **lastCom
    ) override;

    // always called after ultimate (closed question)
    mt::USI question(
      mt::VEC_STR &inputs,
      ParamData &paramData,
      Command **lastCom
    ) override;

    mt::USI dialog(
      mt::VEC_STR &inputs,
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
      CR_SP_CALLBACK callback_in,
      mt::CR_BOL propagatingCallback_in = true
    );

    Toggle(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      CR_SP_PLAIN_CALLBACK callback_in,
      mt::CR_BOL propagatingCallback_in = true
    );

    Toggle(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in
    );

    mt::USI getInheritanceFlag() override { return TOGGLE; }
    std::string getDashedName() override;

    std::string getFullName(
      CR_FullNameProfile fullNameProfile = Command::defaultFullNameProfile
    ) override;
  };
}

#endif // __CLI_MENU__TOGGLE_H__