#ifndef __CLI_MENU__PARAMETER_H__
#define __CLI_MENU__PARAMETER_H__

#include "command.h"

namespace cli_menu {

  class Parameter : public Command {
  private:
    bool accumulating = false,
      type = false;

    void setData(
      ParamData &paramData,
      mt::CR_STR argument
    ) override;

    void resetArgument(ParamData &paramData);

    bool popBackSet(
      mt::VEC_STR &inputs,
      ParamData &paramData
    );

    mt::USI notPopBackSet(
      ParamData &paramData,
      Command **lastCom
    );

  protected:
    std::string getFillingStatusString() override;

    mt::USI match(
      mt::VEC_STR &inputs,
      ParamData &paramData,
      Command **lastCom
    ) override;

    // always called after ultimate (open question)
    mt::USI question(
      ParamData &paramData,
      Command **lastCom
    ) override;

    mt::USI dialog(
      ParamData &paramData,
      Command **lastCom
    ) override;

  public:
    enum {TEXT, NUMBER};
    Parameter(): Command() {}

    Parameter(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      mt::CR_BOL type_in,
      mt::CR_BOL accumulating_in,
      CR_SP_CALLBACK callback_in
    );

    Parameter(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      mt::CR_BOL type_in,
      mt::CR_BOL accumulating_in,
      CR_SP_PLAIN_CALLBACK callback_in
    );

    Parameter(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      mt::CR_BOL type_in,
      mt::CR_BOL accumulating_in
    );

    bool getType() { return type; }
    std::string getStringifiedType();
    std::string getDashedName() override;
    std::string getFullName() override;
    mt::USI getInheritanceFlag() override { return PARAMETER; }

    void setAccumulating(mt::CR_BOL cond) { accumulating = cond; }
    bool isAccumulating() { return accumulating; }
  };
}

#endif // __CLI_MENU__PARAMETER_H__