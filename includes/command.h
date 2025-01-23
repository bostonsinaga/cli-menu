#ifndef __CLI_MENU__COMMAND_H__
#define __CLI_MENU__COMMAND_H__

#include "clipboard.h"
#include "control.h"
#include "result-inputs.h"

namespace cli_menu {
  /**
   * It is recommended to use derived classes
   * for feature completeness.
   */
  class Command : public mt_ds::tree::Node {
  protected:
    using LINKED_LIST = mt_ds::linked_list::Node;
    using TREE = mt_ds::tree::Node;

    typedef mt_ds::tree::VEC_NODE VEC_TREE;
    typedef mt_ds::tree::CR_VEC_NODE CR_VEC_TREE;
    typedef Command Cm;

  private:
    bool propagatingCallback = true;
    RESULT_CALLBACK callback;
    PLAIN_CALLBACK plainCallback;
    VEC_TREE requiredItems;

    static Command *circularCheckpoint;
    static const int disguiseCount = 1;
    static const mt::USI disguiseFlags[disguiseCount];
    static const std::string disguiseNames[disguiseCount][2];

    virtual void initDefaultData(
      ResultInputs &resultInputs
    ) {}

    void setChildren(
      CR_VEC_TREE newChildren,
      mt::CR_BOL needEmpty,
      mt::CR_BOL validating
    ) override;

    TREE* dismantle(mt::CR_INT index) override;
    bool run(ResultInputs &resultInputs);

    std::string getDialogStatusString(
      mt::CR_BOL usingAbbreviations,
      mt::CR_BOL withBrackets,
      mt::CR_BOL forcedToSupporterVersion = false
    );

    virtual std::string getFillingStatusString(
      mt::CR_BOL usingAbbreviations
    );

    void addRequiredItems(Command *command);
    void reduceRequiredItems(Command *command);
    void registerRequiredItems(Command *command);
    void revokeRequiredItems(Command *command);

    void collapseUltimateItems(
      Command *newUltimate,
      VEC_TREE &united
    );

    // this only invoked by 'ultimate'
    void relateToSupporter(
      TREE *node,
      mt::CR_BOL connected
    );

    // prioritize the rest of direct directInputs
    bool isDirectInputsError(
      mt::VEC_STR &directInputs,
      mt::CR_STR controlName
    );

    mt::USI pointToNeighbor(
      mt::CR_BOL toNext,
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
      Command **lastCom
    );

    static bool isTemporaryLetterCaseChange();
    void onFreeChangeInputLetterCase(std::string &strIn);

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
    Clipboard clipboard;
    std::string description;
    int resultInputsIndex = -1;

    bool accumulating = false,
      matching = true,
      questionedGroup = false,
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

    virtual void resetData(
      ResultInputs &resultInputs,
      mt::CR_BOL discarded
    );

    bool runTo(
      Command *target,
      ResultInputs &resultInputs
    );

    mt::UI getRequiredCount();
    Color getLevelLabelColor();
    std::string getLevelLabel();
    Command *getUnusedNeighbor(Command *start);

    void useRequired();
    void unuseRequired();
    void useResultInputsIndex(ResultInputs &resultInputs);

    // format: 'separator + ultimate + separator + this + separator'
    std::string getFullNameWithUltimate(
      mt::CR_STR separator = " ",
      mt::CR_BOL startWithSeparator = true,
      mt::CR_BOL endWithSeparator = false
    );

    std::string getLevelName(mt::CR_BOL isVerbose = false);
    std::string getChildrenLevelName(mt::CR_BOL onlyRequired);
    bool doesUltimateAllowEnter(mt::CR_BOL fromChild = false);
    void printNullptrNeighborError();

    mt::USI isItPossibleToGoBack(
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
      Command **lastCom
    );

    Command *chooseLastCommand(
      mt::CR_BOL onlyParent = false
    );

    void changeTreeNamesToLowercase();
    void changeTreeNamesToUppercase();
    static void stopThreadsLoop();

    virtual mt::USI match(
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
      Command **lastCom
    ) { return FAILED_FLAG; }

    mt::USI matchTo(
      Command *target,
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
      Command **lastCom
    );

    mt::USI conversation(
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
      Command **lastCom
    );

    virtual mt::USI answerControl(
      mt::CR_STR controlStr,
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
      Command **lastCom
    );

    virtual mt::USI answerSpecial(
      mt::CR_STR cinStr,
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
      Command **lastCom
    );

    virtual mt::USI question(
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
      Command **lastCom
    ) { return FAILED_FLAG; }

    mt::USI questionTo(
      Command *target,
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
      Command **lastCom
    );

    virtual mt::USI dialog(
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
      Command **lastCom
    );

    mt::USI dialogTo(
      Command *target,
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
      Command **lastCom
    );

    // point to neighbor if input not matched
    mt::USI askNeighbor(
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
      Command **lastCom
    );

    // 'directInputs' is empty and has 'requiredItems'
    bool isMatchNeedDialog(mt::CR_BOL withMessage = true);

    mt::USI tryToSkip(
      mt::CR_BOL toNext,
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
      Command **lastCom
    );

    mt::USI tryToSelect(
      mt::VEC_STR &directInputs,
      ResultInputs &resultInputs,
      Command **lastCom,
      mt::CR_STR additionalMessage
    );

    // clipboard only for parameter
    void printClipboardError();

    /**
     * Prints '-' signs horizontally before names list or
     * names list with prominent background color.
     */
    void printAfterBoundaryLine(std::string comName);
    void printQuestionBoundaryLine();

  public:
    enum {
      CANCELED_FLAG,  // original
      COMPLETED_FLAG, // original
      ERROR_FLAG,     // pseudo
      FAILED_FLAG,    // original
      PASSED_FLAG     // pseudo
    };

    Command() {}

    Command(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      mt::CR_BOL accumulating_in,
      RESULT_CALLBACK callback_in,
      mt::CR_BOL propagatingCallback_in = true
    );

    Command(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      mt::CR_BOL accumulating_in,
      PLAIN_CALLBACK callback_in,
      mt::CR_BOL propagatingCallback_in = true
    );

    Command(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      mt::CR_BOL accumulating_in
    );

    void addChild(
      TREE *node,
      mt::CR_BOL reconnected = true
    ) override;

    VEC_TREE releaseChildren() override;
    virtual mt::USI getInheritanceFlag() { return COMMAND; }
    virtual std::string getDashedName() { return name; }

    virtual std::string getFullName(
      mt::CR_BOL useDash = true,
      mt::CR_BOL useInputType = true,
      mt::CR_BOL useLevelName = true,
      CR_CLR nameColor = Color()
    ) { return name; }

    // secure original name
    void copyMatchName(std::string &hookName);

    // secure original input
    void copyMatchInput(
      std::string &hookInput,
      mt::CR_STR oriInput
    );

    // secure original strings
    void copyMatchStrings(
      std::string &hookName,
      std::string &hookInput,
      mt::CR_STR oriInput
    );

    void setCallback(RESULT_CALLBACK callback_in) {
      callback = callback_in;
    }

    void setCallback(PLAIN_CALLBACK callback_in) {
      plainCallback = callback_in;
    }

    void setAsUltimate();
    void resignFromUltimate();
    void setRequired(mt::CR_BOL isIt) { required = isIt; }

    std::string getDescription() { return description; }
    Command *getUltimate() { return ultimate; }

    /**
     * Note:
     * Cannot modify 'children' if this a supporter.
     */

    bool isUltimate() const { return this == ultimate; }
    bool isGroup() const { return !(ultimate || children.empty()); }
    bool isSupporter() const { return parent && parent == ultimate; }
    bool isRequired() const { return required; }
    bool isOptional() const { return !required; }
    bool isContainer() const { return !ultimate || isUltimate(); }
    bool isUsed() const { return used; }
    bool isParent() const { return !children.empty(); }
    bool isToddler() const { return children.empty(); }

    std::string getInlineRootNames(
      mt::CR_STR separator = " ",
      mt::CR_BOL startWithSeparator = true,
      mt::CR_BOL endWithSeparator = false
    );

    void printChildrenNamesDescriptions(
      mt::CR_BOL summarized,
      mt::CR_BOL startWithBoundaryLine = true,
      mt::CR_BOL endWithBoundaryLine = true
    );

    void printNoItems();
    void printList();
    virtual void printHelp();
    virtual void printError();
  };
}

#endif // __CLI_MENU__COMMAND_H__