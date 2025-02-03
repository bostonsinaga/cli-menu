#ifndef __CLI_MENU__TOGGLE_H__
#define __CLI_MENU__TOGGLE_H__

#include "command.h"

namespace cli_menu {

  class Toggle : public Command {
  private:
    mt::VEC_BOL conditionsBackup;
    bool defaultCondition = false;

    void initData(mt::CR_VEC_BOL data);
    void initDefaultData() override;
    void resetData(mt::CR_BOL discarded) override;
    void viewAction() override;

    const std::string getNeedsString() const override {
      return " needs conditions";
    }

    mt::USI match() override;
    mt::USI answerSpecial(mt::CR_STR bufferStr) override;
    mt::USI dialog() override;

  protected:
    void setData(mt::CR_BOL condition);

  public:
    Toggle(): Command() {}

    Toggle(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      mt::CR_BOL accumulating_in,
      CM_CALLBACK callback_in,
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