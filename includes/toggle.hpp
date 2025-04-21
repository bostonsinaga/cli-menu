#ifndef __CLI_MENU__TOGGLE_H__
#define __CLI_MENU__TOGGLE_H__

#include "command.h"

namespace cli_menu {

  class Toggle : public Command {
  private:
    mt::VEC_BOL conditionsBackup;
    bool defaultCondition = false;

    void setInputTypeString() {
      inputTypeString = "conditions";
    }

    void initData(mt::CR_VEC_BOL data);
    void initDefaultData() override;
    void setCondition(mt::CR_BOL condition);
    void setConditions(mt::CR_VEC_BOL conditions);
    void setData(mt::CR_STR input) override;
    void resetData(RESET_ENUM resetEnum) override;
    void clipboardAction() override;
    void viewAction() override;
    void printTypeError();

    void resetBackupData() override {
      conditionsBackup = {};
    }

    COMMAND_ENUM match() override;
    COMMAND_ENUM answerSpecial(mt::CR_STR bufferStr) override;
    COMMAND_ENUM dialog() override;

  public:
    Toggle(): Command() {
      setInputTypeString();
    }

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

    INHERITANCE_ENUM getInheritanceEnum() override {
      return INHERITANCE_TOGGLE;
    }

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