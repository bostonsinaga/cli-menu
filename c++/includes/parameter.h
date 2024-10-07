#ifndef __CLI_MENU__PARAMETER_H__
#define __CLI_MENU__PARAMETER_H__

#include "command.h"

namespace cli_menu {

  class Parameter : public Command {
  private:
    std::string argument;
    bool type = false;

    ~Parameter() override {
      argument = "";
      type = false;
    }

  protected:
    void setData(mt::CR_STR str) override;

    // always called after ultimate (open question)
    mt::USI question(Command **ultimateHook) override;

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

    bool match(mt::VEC_STR &inputs) override;
    bool getType() { return type; }
    std::string getStringifiedType();
    std::string getDashedName() override;
    std::string getFullName() override;
    std::string getRawArgument() { return argument; }
    mt::USI getInheritanceFlag() override { return PARAMETER; }

    void pullData(
      ParamData &paramData,
      mt::VEC_UI &usedIndexes
    ) override;
  };
}

#endif // __CLI_MENU__PARAMETER_H__