#ifndef __CLI_MENU__PARAMETER_H__
#define __CLI_MENU__PARAMETER_H__

#include "command.h"

namespace cli_menu {

  class Parameter : public Command {
  private:
    mt::VEC_STR textsBackup;
    mt::VEC_LD numbersBackup;
    bool argumentType = false;
    std::string defaultText = "";
    long double defaultNumber = 0;

    void initDefaultData() override;
    void setData(mt::CR_STR input) override;
    void resetData(RESET_FLAG resetFlag) override;
    void resetBackupData() override;
    void viewAction() override;

    void clipboardAction() override {
      setData(clipboard.paste());
    }

    const std::string getNeedsString() const override;

    mt::USI match() override;
    mt::USI middleMatch(mt::CR_BOL needUnusedNeighbor);
    mt::USI channelTheParent() override;
    mt::USI answerSpecial(mt::CR_STR bufferStr) override;
    mt::USI questionEnterTest() override;
    mt::USI dialog() override;

  public:
    enum {TEXT, NUMBER};
    Parameter(): Command() {}

    Parameter(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      mt::CR_BOL argumentType_in,
      mt::CR_BOL accumulating_in,
      CM_CALLBACK callback_in,
      mt::CR_BOL propagatingCallback_in = true
    );

    Parameter(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      mt::CR_BOL argumentType_in,
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

    mt::USI getInheritanceFlag() override { return PARAMETER; }
    void setAccumulating(mt::CR_BOL cond) { accumulating = cond; }
    bool isAccumulating() { return accumulating; }
  };
}

#endif // __CLI_MENU__PARAMETER_H__