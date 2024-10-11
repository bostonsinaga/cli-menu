#ifndef __CLI_MENU__PARAMETER_H__
#define __CLI_MENU__PARAMETER_H__

#include "command.h"

namespace cli_menu {

  class Parameter : public Command {
  private:
    std::string argument;

    bool accumulating = false,
      type = false;

    void setData(
      ParamData &paramData,
      mt::CR_STR str
    );

    // used in 'question' for 'setData'
    void checkout(
      ParamData &paramData,
      mt::CR_VEC_STR valVec
    );

  protected:
    // always called after ultimate (open question)
    Command *question(ParamData &paramData) override;

    Command *match(
      mt::VEC_STR &inputs,
      ParamData &paramData
    ) override;

  public:
    enum {TEXT, NUMBER};
    Parameter(): Command() {}

    Parameter(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL type_in,
      CR_SP_CALLBACK callback_in,
      Command *holder_in = nullptr,
      mt::CR_BOL required_in = false,
      mt::CR_BOL accumulating_in = false
    );

    Parameter(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL type_in,
      CR_SP_PLAIN_CALLBACK callback_in,
      Command *holder_in = nullptr,
      mt::CR_BOL required_in = false,
      mt::CR_BOL accumulating_in = false
    );

    Parameter(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL type_in,
      Command *holder_in = nullptr,
      mt::CR_BOL required_in = false,
      mt::CR_BOL accumulating_in = false
    );

    bool getType() { return type; }
    std::string getStringifiedType();
    std::string getDashedName() override;
    std::string getFullName() override;
    std::string getFillingStatusString() override;
    mt::USI getInheritanceFlag() override { return PARAMETER; }

    void setAccumulating(mt::CR_BOL cond) { accumulating = cond; }
    bool isAccumulating() { return accumulating; }
  };
}

#endif // __CLI_MENU__PARAMETER_H__