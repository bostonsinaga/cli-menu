#ifndef __CLI_MENU__TOGGLE_H__
#define __CLI_MENU__TOGGLE_H__

#include "command.h"

namespace cli_menu {

  class Toggle : public Command {
  private:
    bool condition = false;

  protected:
    ~Toggle() override { condition = false; }
    void setData(mt::CR_BOL cond) override;

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

    bool match(mt::VEC_STR &inputs) override;
    bool getCondition() { return condition; }
    mt::USI getInheritanceFlag() override { return TOGGLE; }
    std::string getDashedName() override;
    std::string getFullName() override;

    void pullData(
      ParamData &paramData,
      mt::VEC_UI &usedIndexes
    ) override;
  };
}

#endif // __CLI_MENU__TOGGLE_H__