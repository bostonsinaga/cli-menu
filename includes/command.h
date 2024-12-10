#ifndef __CLI_MENU__COMMAND_H__
#define __CLI_MENU__COMMAND_H__

#include "control.h"

namespace cli_menu {
  /**
   * It is recommended to use derived classes
   * for feature completeness.
   */
  class Command : public mt_ds::tree::Node {
  public:
    // required by 'getFullName' method
    struct FullNameProfile {
      bool useDash, useInputType, useLevelName;
      Color nameColor;
    };

    static const FullNameProfile
      defaultFullNameProfile,
      helpFullNameProfile;

  protected:
    using TREE = mt_ds::tree::Node;

    typedef mt_ds::tree::VEC_NODE VEC_TREE;
    typedef mt_ds::tree::CR_VEC_NODE CR_VEC_TREE;
    typedef const FullNameProfile& CR_FullNameProfile;
    typedef Command Cm;

  private:
    bool propagatingCallback = true;
    SP_CALLBACK callback = nullptr;
    SP_PLAIN_CALLBACK plainCallback = nullptr;
    VEC_TREE requiredItems;

    static Command *circularCheckpoint;
    static const int disguiseCount = 1;
    static const mt::USI disguiseFlags[disguiseCount];
    static const std::string disguiseNames[disguiseCount][2];

    virtual void setData(
      ParamData &paramData,
      mt::CR_STR argument
    ) {}

    virtual void setData(
      ParamData &paramData,
      mt::CR_BOL condition
    ) {}

    void setChildren(
      CR_VEC_TREE newChildren,
      mt::CR_BOL needEmpty,
      mt::CR_BOL validating
    ) override;

    TREE* dismantle(mt::CR_INT index) override;
    bool run(ParamData &paramData);

    std::string getDialogStatusString(
      mt::CR_BOL usingAbbreviations,
      mt::CR_BOL withBrackets,
      mt::CR_BOL forcedToDependenceVersion = false
    );

    void printDialogStatus(
      mt::CR_BOL usingAbbreviations,
      mt::CR_BOL withBrackets,
      mt::CR_BOL forcedToDependenceVersion = false
    );

    std::string getHelpDialogStatusString();
    void printHelpDialogStatus();

    virtual std::string getFillingStatusString(
      mt::CR_BOL usingAbbreviations
    );

    void updateRequiredItems(
      Command *command,
      mt::CR_BOL adding
    );

    void collapseUltimateItems(
      Command *newUltimate,
      VEC_TREE &united
    );

    // this only invoked by 'ultimate'
    void relateToDependence(
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
    std::string description;
    int paramDataIndex = -1;

    bool questionedGroup = false,
      required = false,
      selecting = false,
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

    mt::UI getRequiredCount();
    Color getMainLabelColor();
    std::string getMainLabel();
    Command *getUnusedNeighbor(Command *start);
    void updateRequiredUsed(mt::CR_BOL adding);

    // format: 'separator + ultimate + separator + this + separator'
    std::string getFullNameWithUltimate(
      mt::CR_STR separator = " ",
      CR_FullNameProfile fullNameProfile = Command::defaultFullNameProfile,
      mt::CR_BOL startWithSeparator = true,
      mt::CR_BOL endWithSeparator = false
    );

    std::string getLevelName(
      mt::CR_BOL toEndUser
    );

    std::string getChildrenLevelName(
      mt::CR_BOL toEndUser,
      mt::CR_BOL onlyRequired
    );

    // error message selectors for controls
    bool doUltimateAllowEnter(mt::CR_BOL fromChild = false);
    void printRequiredNextError();
    void printNullptrNextError();
    void printOrphanError();

    Command *chooseLastCommand(
      mt::CR_BOL onlyParent = false
    );

    // secure original name
    static void copyMatchName(
      std::string &hookName,
      mt::CR_STR oriName
    );

    // secure original input
    static void copyMatchInput(
      std::string &hookInput,
      mt::CR_STR oriInput
    );

    // secure original strings
    static void copyMatchStrings(
      std::string &hookName, std::string &hookInput,
      mt::CR_STR oriName, mt::CR_STR oriInput
    );

    void changeTreeNamesToLowercase();
    void changeTreeNamesToUppercase();

    virtual mt::USI match(
      mt::VEC_STR &inputs,
      ParamData &paramData,
      Command **lastCom
    ) { return FLAG::FAILED; }

    mt::USI matchTo(
      Command *target,
      mt::VEC_STR &inputs,
      ParamData &paramData,
      Command **lastCom
    );

    virtual mt::USI question(
      mt::VEC_STR &inputs,
      ParamData &paramData,
      Command **lastCom
    ) { return FLAG::FAILED; }

    mt::USI questionTo(
      Command *target,
      mt::VEC_STR &inputs,
      ParamData &paramData,
      Command **lastCom
    );

    virtual mt::USI dialog(
      mt::VEC_STR &inputs,
      ParamData &paramData,
      Command **lastCom
    );

    mt::USI dialogTo(
      Command *target,
      mt::VEC_STR &inputs,
      ParamData &paramData,
      Command **lastCom
    );

    // point to neighbor if input not matched
    mt::USI askNeighbor(
      mt::VEC_STR &inputs,
      ParamData &paramData,
      Command **lastCom
    );

    // 'inputs' is empty and has 'requiredItems'
    bool isMatchNeedDialog(mt::CR_BOL withMessage = true);

    // prioritize the rest of direct inputs queue
    bool printDirectInputsQueueError(
      mt::VEC_STR &inputs,
      mt::CR_STR controlName
    );

    // reusable selection control handler
    mt::USI tryToSkipWithSelection(
      mt::VEC_STR &inputs,
      ParamData &paramData,
      Command **lastCom,
      mt::CR_STR additionalMessage
    );

    /**
     * Prints '-' signs horizontally before names list or
     * names list with prominent background color.
     */
    void printAfterBoundaryLine(std::string comName);

  public:
    enum FLAG {
      CANCELED,  // original
      COMPLETED, // original
      ERROR,     // pseudo
      FAILED,    // original
      PASSED     // pseudo
    };

    Command() {}

    Command(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      CR_SP_CALLBACK callback_in,
      mt::CR_BOL propagatingCallback_in = true
    );

    Command(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      CR_SP_PLAIN_CALLBACK callback_in,
      mt::CR_BOL propagatingCallback_in = true
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
    virtual std::string getInheritanceName() { return "command"; }
    virtual std::string getDashedName() { return name; }

    virtual std::string getFullName(
      CR_FullNameProfile fullNameProfile = Command::defaultFullNameProfile
    ) { return name; }

    void setCallback(CR_SP_CALLBACK callback_in);
    void setCallback(CR_SP_PLAIN_CALLBACK callback_in);
    void setAsUltimate();
    void resignFromUltimate();
    void setRequired(mt::CR_BOL isIt) { required = isIt; }

    std::string getDescription() { return description; }
    Command *getUltimate() { return ultimate; }

    /**
     * NOTE :
     * Cannot change 'children' if this a dependence.
     */

    bool isUltimate() { return this == ultimate; }
    bool isGroup() { return !(ultimate || children.empty()); }
    bool isDependence() { return parent && parent == ultimate; }
    bool isIndependence() { return !ultimate && children.empty(); }
    bool isRequired() { return required; }
    bool isOptional() { return !required; }
    bool isContainer() { return !ultimate || isUltimate(); }
    bool isUsed() { return used; }
    bool isParent() { return isGroup() || isUltimate(); }
    bool isToddler() { return isIndependence() || isDependence(); }

    bool isRequiredContainerEmpty() {
      return !used && !ultimate && isRequired();
    }

    std::string getInlineRootNames(
      mt::CR_STR separator = " ",
      CR_FullNameProfile fullNameProfile = Command::defaultFullNameProfile,
      mt::CR_BOL selfOnly = false,
      mt::CR_BOL startWithSeparator = true,
      mt::CR_BOL endWithSeparator = false
    );

    void printChildrenNamesDescriptions(
      mt::CR_BOL startWithBoundaryLine = true,
      mt::CR_BOL endWithBoundaryLine = true
    );

    virtual void printHelp();
    virtual void printError();
  };
}

#endif // __CLI_MENU__COMMAND_H__