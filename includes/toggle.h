#ifndef __CLI_MENU__TOGGLE_H__
#define __CLI_MENU__TOGGLE_H__

#include "command.h"

namespace cli_menu {

  class Toggle : public Command {
  private:
    mt::VEC_BOL conditions;
    bool defaultCondition = false;

    void initData(
      ResultInputs &resultInputs,
      mt::CR_VEC_BOL data
    );

    void initDefaultData(
      ResultInputs &resultInputs
    ) override {
      initData(resultInputs, {defaultCondition});
    }

    void resetData(
      ResultInputs &resultInputs,
      mt::CR_BOL discarded
    ) override;

    mt::USI match(
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
      Command **lastCom
    ) override;

    mt::USI answerControl(
      mt::CR_STR controlStr,
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
      Command **lastCom
    ) override;

    mt::USI answerSpecial(
      mt::CR_STR cinStr,
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
      Command **lastCom
    ) override;

    mt::USI dialog(
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
      Command **lastCom
    ) override;

  protected:
    void setData(
      ResultInputs &resultInputs,
      mt::CR_BOL condition
    );

  public:
    Toggle(): Command() {}

    Toggle(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      mt::CR_BOL accumulating_in,
      RESULT_CALLBACK callback_in,
      mt::CR_BOL propagatingCallback_in = true
    );

    Toggle(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      mt::CR_BOL accumulating_in,
      PLAIN_CALLBACK callback_in,
      mt::CR_BOL propagatingCallback_in = true
    );

    Toggle(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      mt::CR_BOL accumulating_in
    );

    mt::USI getInheritanceFlag() override { return TOGGLE; }
    std::string getDashedName() override;

    std::string getFullName(
      mt::CR_BOL useDash = true,
      mt::CR_BOL useInputType = true,
      mt::CR_BOL useLevelName = true,
      CR_CLR nameColor = Color()
    ) override;
  };
}

#endif // __CLI_MENU__TOGGLE_H__