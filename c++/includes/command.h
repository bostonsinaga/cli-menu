#ifndef __CLI_MENU__COMMAND_H__
#define __CLI_MENU__COMMAND_H__

#include "control.h"

namespace cli_menu {
  /**
   * It is recommended to use derived classes
   * for feature completeness.
   */
  class Command : public mt_ds::tree::Node {
  protected:
    typedef mt_ds::tree::Node TREE;
    typedef mt_ds::tree::VEC_NODE VEC_TREE;
    typedef mt_ds::tree::CR_VEC_NODE CR_VEC_TREE;
    typedef Command Cm;

  private:
    std::string description;
    SP_CALLBACK callback = nullptr;
    SP_PLAIN_CALLBACK plainCallback = nullptr;
    VEC_TREE requiredItems;
    bool required = false;

    static Command *circularCheckpoint;
    static const int disguiseCount = 1;
    static const mt::USI disguiseFlags[disguiseCount];
    static const std::string disguiseNames[disguiseCount][2];

    bool run();
    bool run(ParamData &paramData);

    void printDialogStatus();
    virtual std::string getFillingStatusString();
    virtual bool isGroupNeedQuestion() { return isSupporter(); }

    /**
     * Specifying 'Group' or 'Command' string
     * as children level name.
     */
    void printGroupNotFound();

    void updateRequiredItems(
      Command *command,
      mt::CR_BOL adding
    );

    void collapseUltimateItems(
      Command *newUltimate,
      VEC_TREE &united
    );

    static bool isTemporaryLetterCaseChange();
    static void onFreeChangeInputLetterCase(std::string &strIn);

    /**
     * A condition setter for 'disguised' where the derived class
     * will behave as its base class (hiding its own abilities)
     * due to certain circumstances. This is a one way process.
     */
    static void checkDisguise(
      Command *command,
      mt::CR_STR occurrenceMethodName
    );

  protected:
    Command *ultimate = nullptr;
    bool used = false;

    /**
     * Requires a conditional statement on this variable
     * in derived class that listed in 'disguiseFlags'.
     */
    bool disguised = false;

    // can only be set through the 'Program'
    static bool usingCaseSensitiveName,
      usingLowercaseName,
      usingUppercaseName,
      usingDashesBoundaryLine,
      dialogued;

    static const std::string
      error_string_enter,
      error_string_next;

    bool runTo(
      Command *target,
      ParamData &paramData
    );

    std::string getBranchLeafString(
      mt::CR_INT spacesCount,
      mt::CR_INT columnIndex,
      mt::CR_BOL withDescription
    );

    mt::UI getRequiredCount();
    Color getMainLabelColor();
    std::string getMainLabel();
    std::string getFullNameWithUltimate();
    Command *getUnusedNeighbor(Command *start);
    void updateRequiredSelf(mt::CR_BOL adding);

    // secure original strings
    static void copyMatchNames(
      std::string &hookName, std::string &hookInput,
      mt::CR_STR oriName, mt::CR_STR oriInput
    );

    void changeTreeNamesToLowercase();
    void changeTreeNamesToUppercase();

    virtual mt::USI match(
      mt::VEC_STR &inputs,
      ParamData &paramData,
      Command **lastCom
    ) { return FLAG::ERROR; }

    mt::USI matchTo(
      Command *target,
      mt::VEC_STR &inputs,
      ParamData &paramData,
      Command **lastCom
    );

    virtual mt::USI question(
      ParamData &paramData,
      Command **lastCom
    ) { return FLAG::ERROR; }

    mt::USI questionTo(
      Command *target,
      ParamData &paramData,
      Command **lastCom
    );

    mt::USI dialog(
      ParamData &paramData,
      Command **lastCom
    );

    mt::USI dialogTo(
      Command *target,
      ParamData &paramData,
      Command **lastCom
    );

    // point to neighbor if input not matched
    mt::USI askNeighbor(
      mt::VEC_STR &inputs,
      ParamData &paramData,
      Command **lastCom
    );

    /**
     * Invoked in 'match' when
     * 'inputs' is empty and has 'requiredItems'.
     */
    bool isMatchNeedDialog();

    /**
     * Prints '-' signs horizontally before names list or
     * names list with prominent background color.
     */
    void printAfterBoundaryLine(std::string comName);

  public:
    enum FLAG { COMPLETED, ERROR, CANCELED };

    Command() {}

    Command(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      CR_SP_CALLBACK callback_in,
      Command *parent_in = nullptr,
      mt::CR_BOL required_in = false
    );

    Command(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      CR_SP_PLAIN_CALLBACK callback_in,
      Command *parent_in = nullptr,
      mt::CR_BOL required_in = false
    );

    Command(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      Command *parent_in = nullptr,
      mt::CR_BOL required_in = false
    );

    virtual mt::USI getInheritanceFlag() { return COMMAND; }
    virtual std::string getDashedName() { return name; }
    virtual std::string getFullName() { return name; }

    void setCallback(CR_SP_CALLBACK callback_in);
    void setCallback(CR_SP_PLAIN_CALLBACK callback_in);
    void setAsUltimate();
    void resignFromUltimate();
    void setRequired(mt::CR_BOL isIt) { required = isIt; }

    std::string getDescription() { return description; }
    Command *getUltimate() { return ultimate; }

    /**
     * NOTE :
     * Cannot change 'children' if this a supporter.
     */

    bool isUltimate() { return this == ultimate; }
    bool isGroup() { return !ultimate || isUltimate(); }
    bool isSupporter() { return parent && parent == ultimate; }
    bool isRequired() { return isGroup() || required; }
    bool isOptional() { return !isRequired(); }
    bool isUsed() { return used; }

    std::string getInlineRootNames(
      mt::CR_STR separator,
      mt::CR_BOL fully
    );

    std::string getAccumulatedInlineRootNames(
      mt::CR_STR separator,
      mt::CR_BOL fully
    );

    std::string getBranchLeafString(
      int spacesCount,
      mt::CR_BOL withDescription
    );

    virtual void printHelp();
    virtual void printError();
  };
}

#endif // __CLI_MENU__COMMAND_H__