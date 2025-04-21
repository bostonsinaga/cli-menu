#ifndef __CLI_MENU__PARAMETER_HPP__
#define __CLI_MENU__PARAMETER_HPP__

#include "command.hpp"

namespace cli_menu {

  class Parameter : public Command {
  public:
    enum PARAM_TYPE {PARAM_TEXT, PARAM_NUMBER};

  private:
    mt::VEC_STR textsBackup;
    mt::VEC_LD numbersBackup;
    PARAM_TYPE argumentType = PARAM_TEXT;
    std::string defaultText = "";
    long double defaultNumber = 0;

    void setInputTypeString();
    void initDefaultData() override;
    void setNumbers(mt::CR_VEC_LD numbers);
    void setData(mt::CR_STR input) override;
    void resetData(RESET_ENUM resetEnum) override;
    void resetBackupData() override;
    void clipboardAction() override;
    void viewAction() override;

    COMMAND_ENUM match() override;
    COMMAND_ENUM middleMatch(mt::CR_BOL needUnusedNeighbor);
    COMMAND_ENUM channelTheParent() override;
    COMMAND_ENUM answerSpecial(mt::CR_STR bufferStr) override;
    COMMAND_ENUM questionEnterTest() override;
    COMMAND_ENUM dialog() override;

  public:
    Parameter(): Command() {
      setInputTypeString();
    }

    Parameter(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      PARAM_TYPE argumentType_in,
      mt::CR_BOL accumulating_in,
      CM_CALLBACK callback_in,
      mt::CR_BOL propagatingCallback_in = true
    );

    Parameter(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      PARAM_TYPE argumentType_in,
      mt::CR_BOL accumulating_in
    );

    bool getArgumentType() const { return argumentType; }
    std::string getStringifiedArgumentType();
    std::string getDashedName() override;

    std::string getFullName(
      mt::CR_BOL useDash = true,
      mt::CR_BOL useInputType = true,
      mt::CR_BOL useLevelName = true,
      CR_CLR nameColor = Color()
    ) override;

    INHERITANCE_ENUM getInheritanceEnum() override {
      return INHERITANCE_PARAMETER;
    }

    void setAccumulating(mt::CR_BOL cond);
    bool isAccumulating() { return accumulating; }
  };
}

#endif // __CLI_MENU__PARAMETER_HPP__