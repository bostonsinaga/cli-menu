#ifndef __CLI_MENU__PARAMETER_H__
#define __CLI_MENU__PARAMETER_H__

#include "command.h"

namespace cli_menu {

  class Parameter : public Command {
  protected:
    mt::VEC_STR texts;
    mt::VEC_LD numbers;

    std::string defaultText = "";
    long double defaultNumber = 0;

    bool argumentType = false;
    static const std::string needsArgStr;

    void initDefaultData(
      ResultInputs &resultInputs
    ) override;

    void setData(
      ResultInputs &resultInputs,
      mt::CR_STR argument
    );

    void resetData(
      ResultInputs &resultInputs,
      mt::CR_BOL discarded
    ) override;

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
      ResultInputs &resultInputs,
      Command **lastCom
    );

    mt::USI notPopBackSet(
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
      Command **lastCom
    );

    mt::USI middleMatch(
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
      Command **lastCom,
      mt::CR_BOL needUnused = false
    );

    std::string getFillingStatusString(
      mt::CR_BOL usingAbbreviations
    ) override;

    mt::USI match(
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
      Command **lastCom
    ) override;

    // always called after ultimate (open question)
    mt::USI question(
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
      Command **lastCom
    ) override;

    mt::USI dialog(
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
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
      CALLBACK callback_in,
      mt::CR_BOL propagatingCallback_in = true
    );

    Parameter(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      mt::CR_BOL argumentType_in,
      mt::CR_BOL accumulating_in,
      PLAIN_CALLBACK callback_in,
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