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
    using TREE = mt_ds::tree::Node;
    typedef mt_ds::tree::VEC_NODE VEC_TREE;
    typedef mt_ds::tree::CR_VEC_NODE CR_VEC_TREE;
    typedef Command Cm;

  private:
    std::string description;
    SP_CALLBACK callback = nullptr;
    SP_PLAIN_CALLBACK plainCallback = nullptr;
    VEC_TREE requiredItems;

    static Command *circularCheckpoint;
    static const int disguiseCount = 1;
    static const mt::USI disguiseFlags[disguiseCount];
    static const std::string disguiseNames[disguiseCount][2];

    void setChildren(
      CR_VEC_TREE newChildren,
      mt::CR_BOL needEmpty,
      mt::CR_BOL validating
    ) override;

    TREE* dismantle(mt::CR_INT index) override;

    bool run();
    bool run(ParamData &paramData);

    void printDialogStatus();
    virtual std::string getFillingStatusString();
    virtual bool isGroupNeedQuestion() { return isSupporter(); }

    void updateRequiredItems(
      Command *command,
      mt::CR_BOL adding
    );

    void collapseUltimateItems(
      Command *newUltimate,
      VEC_TREE &united
    );

    // this only invoked by 'ultimate'
    void relateToSupporter(
      TREE *node,
      mt::CR_BOL connected
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

    bool questionedGroup = false,
      required = false,
      used = false;

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
    Command *getUnusedNeighbor(Command *start);
    void updateRequiredSelf(mt::CR_BOL adding);

    // format: 'separator + ultimate + separator + this + separator'
    std::string getFullNameWithUltimate(
      mt::CR_STR separator = " ",
      mt::CR_BOL startWithSeparator = true,
      mt::CR_BOL endWithSeparator = false
    );

    std::string getLevelName(
      mt::CR_BOL toEndUser = true,
      mt::CR_BOL isFirstCapitalLetter = false
    );

    std::string getChildrenLevelName(
      mt::CR_BOL toEndUser = true,
      mt::CR_BOL onlyRequired = true,
      mt::CR_BOL isFirstCapitalLetter = false
    );

    /**
     * Error message generators of level that has neighbors
     * or children that its 'paramData' section must be set.
     */
    void printEnterError();
    void printNextError();

    virtual bool onEnter(
      ParamData &paramData,
      Command **lastCom
    ) { return FLAG::ERROR; }

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
      mt::CR_BOL required_in,
      Command *parent_in,
      CR_SP_CALLBACK callback_in
    );

    Command(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      CR_SP_PLAIN_CALLBACK callback_in
    );

    Command(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in
    );

    void addChild(
      TREE *node,
      mt::CR_BOL reconnected = true
    ) override;

    VEC_TREE releaseChildren() override;

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
    bool isGroup() { return !(ultimate || children.empty()); }
    bool isSupporter() { return parent && parent == ultimate; }
    bool isRequired() { return required; }
    bool isOptional() { return !required; }
    bool isContainer() { return !ultimate || isUltimate(); }
    bool isParent() { return isGroup() || isUltimate(); }
    bool isToddler() { return (!ultimate && children.empty()) || isSupporter(); }
    bool isUsed() { return used; }

    std::string getInlineRootNames(
      mt::CR_STR separator = " ",
      mt::CR_BOL fully = true,
      mt::CR_BOL startWithSeparator = true,
      mt::CR_BOL endWithSeparator = false
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