#ifndef __CLI_MENU__PARAMETER_H__
#define __CLI_MENU__PARAMETER_H__

#include "command.h"

namespace cli_menu {

  class Parameter : public Command {
  private:
    bool accumulating = false,
      argumentType = false;

    static const std::string needsArgStr;

    void setData(
      ParamData &paramData,
      mt::CR_STR argument
    ) override;

    void resetArgument(ParamData &paramData);

    static bool checkArgument(
      LINKED_LIST *node,
      mt::CR_STR copyInput,
      bool &found
    );

    LINKED_LIST *getContinuation(
      mt::CR_BOL needUnused = false
    );

    mt::USI popBackSet(
      mt::VEC_STR &directInputs,
      ParamData &paramData,
      Command **lastCom
    );

    mt::USI notPopBackSet(
      mt::VEC_STR &directInputs,
      ParamData &paramData,
      Command **lastCom
    );

    mt::USI middleMatch(
      mt::VEC_STR &directInputs,
      ParamData &paramData,
      Command **lastCom,
      mt::CR_BOL needUnused = false
    );

  protected:
    std::string getFillingStatusString(
      mt::CR_BOL usingAbbreviations
    ) override;

    mt::USI match(
      mt::VEC_STR &directInputs,
      ParamData &paramData,
      Command **lastCom
    ) override;

    // always called after ultimate (open question)
    mt::USI question(
      mt::VEC_STR &directInputs,
      ParamData &paramData,
      Command **lastCom
    ) override;

    mt::USI dialog(
      mt::VEC_STR &directInputs,
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
      mt::CR_BOL argumentType_in,
      mt::CR_BOL accumulating_in,
      CR_SP_CALLBACK callback_in,
      mt::CR_BOL propagatingCallback_in = true
    );

    Parameter(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      mt::CR_BOL argumentType_in,
      mt::CR_BOL accumulating_in,
      CR_SP_PLAIN_CALLBACK callback_in,
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

    bool getArgumentType() { return argumentType; }
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