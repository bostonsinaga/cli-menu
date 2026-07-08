#ifndef __CLI_MENU__COMMAND_HPP__
#define __CLI_MENU__COMMAND_HPP__

#include "clipboard.hpp"

namespace cli_menu {

  // callback status codes
  enum COMMAND_CODE {
    COMMAND_ERROR,
    COMMAND_DONE,
    COMMAND_CANCELED,
    COMMAND_TERMINATED,
    COMMAND_TERMINATED_SILENT,
    COMMAND_PSEUDO_SILENT,
    COMMAND_ONGOING
  };

  // parsing phase codes
  enum COMMAND_PHASE_CODE {
    COMMAND_PHASE_MATCH,
    COMMAND_PHASE_DIALOG,
    COMMAND_PHASE_MATCH_IN_DIALOG
  };

  // callback return codes
  enum COMMAND_CALLBACK_CODE {
    COMMAND_CALLBACK_ERROR,
    COMMAND_CALLBACK_DONE,
    COMMAND_CALLBACK_CANCELED
  };

  class Command;

  typedef std::function<COMMAND_CALLBACK_CODE(Command*)> CODE_CALLBACK;
  typedef std::function<void(Command*)> VOID_CALLBACK;
  typedef std::function<bool(Command*)> BOOL_CALLBACK;

  class Command : public mt_ds::GeneralTree {
  protected:
    static CODE_CALLBACK defaultCallback;

  private:
    int pseudosCount = 0;
    static constexpr int totalCommandCodes = 7;

    bool editing = true,
      pseudo = false,
      strict = false,
      dialogued = true,
      propagation = true,
      asInput = false,
      asOutput = false;

    inline static COMMAND_PHASE_CODE phaseCode = COMMAND_PHASE_MATCH;

    // this code (7th) always set before moving to another command
    COMMAND_CODE statusCodes[totalCommandCodes + 1] = {
      COMMAND_ERROR,
      COMMAND_DONE,
      COMMAND_CANCELED,
      COMMAND_TERMINATED,
      COMMAND_TERMINATED_SILENT,
      COMMAND_PSEUDO_SILENT,
      COMMAND_ONGOING,
      COMMAND_ONGOING
    };

    // return false to stop the program 
    CODE_CALLBACK callback = Command::defaultCallback;

    // prohibit controllers after match
    inline static bool interruptionDialogued = false;

    /**
     * Invoke input or output callbacks.
     * Will return 'COMMAND_CALLBACK_DONE' by default
     * if there is no 'asInput' or 'asOutput' condition from the children.
     */
    COMMAND_CALLBACK_CODE forEachInOutCallbacks(
      mt::CR<BOOL_CALLBACK> asWhatCallback
    );

    /**
     * Invoke input-process-output callbacks.
     * Will return 'COMMAND_CALLBACK_DONE' by default
     * if this has no callback and no children with 'asInput' and 'asOutput' conditions.
     */
    COMMAND_CALLBACK_CODE triggerCallbacks();

    // return this command with its status set
    Command *setStatus(mt::CR<COMMAND_CODE> code);

    // after dialog interactions
    Command *igniteCallbacks();
    Command *backTo(mt_ds::GeneralTree *topCommand);
    Command *enter();
    Command *execute();
    Command *goDown(mt::CR_STR input);
    Command *goToNeighbor(mt::CR<DIRECTION> direction);

    /**
     * Check whether strict parent has incomplete required child.
     * If so, print error and return the first found.
     */
    Command *strictParentHasRequired();

    // an error message in dialog when switching mode / moving position
    void printInterruptionDialoguedResponse();

    // input is expected as e.g. '--foo' or '-goo'
    bool testHyphens(mt::CR_STR input) {
      return hyphens + keyword == input;
    }

    /**
     * The number of children is reduced by the
     * number of pseudo-commands to ignoring them.
     */
    bool hasChildren() {
      return numberOfChildren() - pseudosCount > 0;
    }

  protected:
    // changeable and initial reference
    mt::PAIR_BOL required = {false, false};

    bool sterilized = false;
    std::string hyphens, keyword, description;
    STRINGIFIED_TYPE_COMMAND_CODE stringifiedTypeIndex;

    /**
     * Reversed string vector.
     * Accessed from behind and immediately 'pop_back()'.
     */
    inline static mt::VEC_STR raws = {};

    Command(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      mt::CR<CODE_CALLBACK> callback_in
    );

    /** End user generated data */

    virtual void clipboardInputPaste() {}
    virtual void clipboardOutputCopy() {}
    virtual void strargv(mt::CR_STR raw) {}
    virtual void printInput() {}
    virtual void printOutput() {}
    virtual void printDescendantInputs() {}
    virtual void printDescendantOutputs() {}
    virtual void resetInput() {}
    virtual void resetOutput() {}
    virtual void resetDescendantInputs() {}
    virtual void resetDescendantOutputs() {}

  public:
    Command() = delete;

    /**
     * Entry point to dialog interactions.
     * The 'raws' only expected as keywords or arguments.
     * Note that the 'keyword' is not case sensitive.
     */
    Command *match();

    // extended runtime input
    Command *dialog();

    // find one desired command in this level with given condition
    Command *findEach(
      mt::CR<BOOL_CALLBACK> condition,
      mt::CR<DIRECTION> direction = RIGHT
    );

    // member variable access
    const bool isRequired() const { return required.first; }
    const std::string getHyphens() const { return hyphens; }
    const std::string getKeyword() const { return keyword; }
    const std::string getDescription() const { return description; }

    // status code to be displayed after callback
    const COMMAND_CODE getStatusCode() const;

    // set status code done/error assign values to 'COMMAND_ONGOING'
    void silentStatus(mt::CR_VEC<COMMAND_CODE> onlyCodes = {});

    /**
     * Will not open dialog to complete the required.
     * Directly display the callback status.
     */
    void noDialogued() { dialogued = false; }
    bool isDialogued() const { return dialogued; }

    /**
     * Prevent 'igniteCallbacks' from bubbling callbacks to the root.
     * Make this will only call its input-process-output callbacks.
     */
    void noPropagation() { propagation = false; }
    bool isPropagation() const { return propagation; }

    /**
     * Make this will not appeared in help and list controls.
     * On command selection in parent dialog, will make this
     * invoke its callbacks without moving from the parent to itself.
     * 
     * Make this also cannot entered with 'enterTest',
     * 'nextTest', or 'previousTest' in dialog.
     * 
     * DOES NOT APPLY TO THE ROOT.
     */
    void makePseudo();

    /**
     * Make arguments must be provided explicitly
     * to be able to call the 'igniteCallbacks'.
     * Only applies to non-pseudo commands.
     */
    void makeRequired();

    /**
     * Make all the required descendants must be
     * completed to be able to call the 'igniteCallbacks'.
     */
    void makeStrict() { strict = true; }

    /**
     * Make this cannot have children
     * and the existing can be deleted.
     */
    void makeSterilized(mt::CR_BOL becomeLeaf = false);

    /**
     * Make parent call this callback
     * before or after its own callback.
     */
    void registerAsInput();  // before
    void registerAsOutput(); // after

    /** Display information about this command */

    // accumulate keywords up to root
    std::string generateSequentialRootNames();

    // displayed once
    void printWelcome();

    // print keyword, description, and children keywords
    void printHelp();

    /**
     * Print keyword and type.
     * Display '*' for required command.
     */
    void printKeyword(
      mt::CR<CONSOLE_CODE> consoleCode,
      mt::CR_SZ numberOfIndents
    );

    // print children keywords
    void printList(
      mt::CR<CONSOLE_CODE> consoleCode,
      mt::CR_SZ numberOfIndents,
      mt::CR_BOL displayAtLeafWarning
    );
  };
}

#endif // __CLI_MENU__COMMAND_HPP__