#ifndef __CLI_MENU__COMMAND_H__
#define __CLI_MENU__COMMAND_H__

#include "clipboard.h"
#include "control.h"
#include "result-inputs.h"

namespace cli_menu {

  // utilize 'ResultInputs' with its static members
  typedef std::function<void()> CM_CALLBACK;

  // enumerations that control the 'match', 'dialog', and 'question' calls
  enum COMMAND_ENUM {
    COMMAND_CANCELED,  // original
    COMMAND_COMPLETED, // original
    COMMAND_CONTINUE,  // pseudo
    COMMAND_FAILED,    // original
    COMMAND_PASSED     // pseudo
  };

  typedef const COMMAND_ENUM& CR_COMNUM;

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
    CM_CALLBACK callback;
    VEC_TREE requiredItems;

    static Command *circularCheckpoint;
    static const int disguiseCount = 1;
    static const INHERITANCE_ENUM disguiseEnums[disguiseCount];
    static const std::string disguiseNames[disguiseCount][2];

    virtual void initDefaultData() {}

    void setChildren(
      CR_VEC_TREE newChildren,
      mt::CR_BOL needEmpty,
      mt::CR_BOL validating
    ) override;

    TREE* dismantle(mt::CR_INT index) override;
    bool execute();

    std::string getDialogStatusString(
      mt::CR_BOL usingAbbreviations,
      mt::CR_BOL withBrackets,
      mt::CR_BOL forcedToSupporterVersion = false
    );

    void addRequiredItems(Command *command);
    void reduceRequiredItems(Command *command);
    void registerRequiredItems(Command *command);
    void revokeRequiredItems(Command *command);

    void collapseUltimateItems(
      Command *newUltimate,
      VEC_TREE &united
    );

    // this only called by 'ultimate'
    void relateToSupporter(
      TREE *node,
      mt::CR_BOL connected
    );

    // prioritize the rest of 'directInputs'
    bool isDirectInputsError(mt::CR_STR controlName);

    static bool checkCommandKeyword(
      LINKED_LIST *node,
      mt::CR_STR copyInput,
      bool &found
    );

    COMMAND_ENUM pointToNeighbor(mt::CR_BOL toNext);
    static bool isTemporaryLetterCaseChange();
    void onFreeChangeInputLetterCase(std::string &strIn);

    std::string getUnableToString(
      mt::CR_STR controlName,
      mt::CR_BOL isComplete
    );

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
    enum RESET_ENUM {
      RESET_INIT, RESET_BACKUP, RESET_DISCARD
    };

    Command *ultimate = nullptr;

    std::string description,
      inputTypeString = "values";

    bool accumulating = false,
      questionedGroup = false,
      required = false,
      used = false;

    /**
     * Requires a conditional statement on this variable
     * in derived class that listed in 'disguiseEnums'.
     */
    bool disguised = false;

    // can only be set through the 'Program'
    static bool usingCaseSensitiveName,
      usingLowercaseName,
      usingUppercaseName,
      usingDashesBoundaryLine,
      dialogued,
      matching,
      selecting;

    static Command *lastCom;
    static mt::VEC_STR directInputs;
    int resultInputsIndex = -1;

    void overwrite() {
      if (!accumulating) resetData(RESET_DISCARD);
    }

    virtual void setData(mt::CR_STR input) {}
    virtual void resetData(RESET_ENUM resetEnum);
    virtual void resetBackupData() {}
    bool executeTo(Command *target);

    mt::UI getRequiredCount();
    Color getLevelLabelColor();
    std::string getLevelLabel();
    Command *getUnusedNeighbor(Command *start);

    void useRequired();
    void unuseRequired();
    void useResultInputsIndex();

    // format: 'separator + ultimate + separator + this + separator'
    std::string getFullNameWithUltimate(
      mt::CR_STR separator = " ",
      mt::CR_BOL startWithSeparator = true,
      mt::CR_BOL endWithSeparator = false
    );

    std::string getLevelName(mt::CR_BOL isVerbose = false);
    std::string getChildrenLevelName(mt::CR_BOL onlyRequired);
    std::string getSubjectString(Command *subject);
    std::string getNeedsString(mt::CR_BOL isLast);

    bool doesUltimateAllowEnter(mt::CR_BOL fromChild = false);
    void printNullptrNeighborError();
    COMMAND_ENUM tryToGoBack();

    Command *chooseLastCommand(
      mt::CR_BOL onlyParent = false
    );

    void changeTreeNamesToLowercase();
    void changeTreeNamesToUppercase();

    virtual COMMAND_ENUM match() {
      return COMMAND_CONTINUE;
    }

    LINKED_LIST *getContinuation(
      mt::CR_BOL needUnusedNeighbor
    );

    // call this after passing 'DashTest::clean..'
    COMMAND_ENUM popBackSet();

    COMMAND_ENUM matchTo(Command *target);
    COMMAND_ENUM conversation(mt::CR_BOL dialogOn);
    COMMAND_ENUM downTheChannel(CR_COMNUM defaultEnum);

    virtual COMMAND_ENUM channelTheParent();

    COMMAND_ENUM answerControl(
      mt::CR_STR controlStr,
      mt::CR_BOL dialogOn
    );

    virtual COMMAND_ENUM answerSpecial(mt::CR_STR bufferStr);

    // callable in all levels
    COMMAND_ENUM question();
    COMMAND_ENUM questionTo(Command *target);

    virtual COMMAND_ENUM questionEnterTest() {
      return COMMAND_PASSED;
    }

    // only called in containers
    virtual COMMAND_ENUM dialog();
    COMMAND_ENUM dialogTo(Command *target);

    // point to neighbor if input not matched
    COMMAND_ENUM askNeighbor();

    // 'INPUTS.direct' is empty and has 'requiredItems'
    bool doesMatchNeedDialog(mt::CR_BOL withMessage = true);

    virtual void clipboardAction();
    virtual void viewAction() {}

    /**
     * Prints '-' signs horizontally before names list or
     * names list with prominent background color.
     */
    void printAfterBoundaryLine(std::string comName);
    void printQuestionBoundaryLine();

  public:
    Command() {}

    Command(
      mt::CR_STR name_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      Command *parent_in,
      mt::CR_BOL accumulating_in,
      CM_CALLBACK callback_in,
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
    virtual std::string getDashedName() { return name; }

    virtual INHERITANCE_ENUM getInheritanceEnum() {
      return INHERITANCE_COMMAND;
    }

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

    void setCallback(CM_CALLBACK callback_in) {
      callback = callback_in;
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