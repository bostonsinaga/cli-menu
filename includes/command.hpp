#ifndef __CLI_MENU__COMMAND_HPP__
#define __CLI_MENU__COMMAND_HPP__

#include "result.hpp"
#include "clipboard.hpp"

namespace cli_menu {

  // status codes
  enum COMMAND_CODE {
    COMMAND_FAILED, COMMAND_CANCELED,
    COMMAND_SUCCEEDED, COMMAND_TERMINATED,
    COMMAND_ONGOING, COMMAND_PSEUDO_ENDED
  };

  // phase codes
  enum COMMAND_PHASE_CODE {
    COMMAND_PHASE_MATCH,
    COMMAND_PHASE_DIALOG,
    COMMAND_PHASE_MATCH_IN_DIALOG
  };

  // callback codes
  enum COMMAND_CALLBACK_CODE {
    COMMAND_CALLBACK_FAILED,
    COMMAND_CALLBACK_CANCELED,
    COMMAND_CALLBACK_SUCCEEDED
  };

  typedef std::function<COMMAND_CALLBACK_CODE(Command*)> COMMAND_CALLBACK;

  class Command : public mt_ds::GeneralTree {
  private:
    int pseudosCount = 0;

    bool editing = true,
      pseudo = false,
      strict = false,
      localDialogued = true,
      localPropagation = true,
      asInput = false,
      asOutput = false;

    inline static bool
      globalDialogued = true,
      globalPropagation = true;

    inline static COMMAND_PHASE_CODE phaseCode = COMMAND_PHASE_MATCH;

    // always set this code before moving to another node
    COMMAND_CODE statusCode = COMMAND_ONGOING;

    // return false to stop the program 
    COMMAND_CALLBACK callback;

    // prohibit controllers after match
    inline static bool interruptionDialogued = false;

    // accumulate keywords up to root
    std::string generateSequentialRootNames();

    /**
     * Invoke input or output callbacks.
     * Will return 'COMMAND_CALLBACK_SUCCEEDED' by default
     * if there is no 'asInput' or 'asOutput' condition from the children.
     */
    COMMAND_CALLBACK_CODE iterateInOutCallbacks(
      const std::function<bool(Command*)> &asWhatCallback
    );

    /**
     * Invoke input-process-output callbacks.
     * Will return 'COMMAND_CALLBACK_SUCCEEDED' by default
     * if this has no callback and no children with 'asInput' and 'asOutput' conditions.
     */
    COMMAND_CALLBACK_CODE triggerCallbacks();

    // return this node with its status set
    Command *setStatus(const COMMAND_CODE &code);

    // after dialog interactions
    Command *enter();
    Command *igniteCallbacks();
    Command *goDown(mt::CR_STR input);
    Command *goToNeighbor(const mt_ds::GeneralTree::DIRECTION &direction);

    // an error message in dialog when switching mode / moving position
    void printInterruptionDialoguedResponse();

    /**
     * Check whether strict parent has incomplete required child.
     * If so, print error and return the first found.
     */
    Command *strictParentHasRequired(mt::CR_BOL onlyOrtho);

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
      COMMAND_CALLBACK callback_in
    );

    Command(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in
    );

    // better called after callbacks call
    void clipboardCopy() {
      Clipboard::copyText(Result::concatOutputs(this));
    }

    // modify the 'Result' values
    virtual void clipboardPaste() {}
    virtual void pushUnormap(mt::CR_STR input) {}
    virtual void resetUnormap() {}

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

    // member variable access
    const bool isRequired() const { return required.first; }
    const std::string getHyphens() const { return hyphens; }
    const std::string getKeyword() const { return keyword; }
    const std::string getDescription() const { return description; }
    const COMMAND_CODE getStatusCode() const { return statusCode; }

    /**
     * Will not open dialog to complete the required.
     * Directly display 'COMMAND_FAILED'.
     */

    // local
    void noDialogue(mt::CR_BOL condition = true) {
      localDialogued = !condition;
    }

    // global
    static void banDialogue(mt::CR_BOL condition = true) {
      Command::globalDialogued = !condition;
    }

    // ask both
    bool isDialogued() {
      return Command::globalDialogued && localDialogued;
    }

    /**
     * Prevent 'igniteCallbacks' from bubbling callbacks to the root.
     * Make this will only call its input-process-output callbacks.
     */

    // local
    void noPropagation(mt::CR_BOL condition = true) {
      localPropagation = !condition;
    }

    // global
    static void banPropagation(mt::CR_BOL condition = true) {
      Command::globalPropagation = !condition;
    }

    // ask both
    bool isPropagated() {
      return Command::globalPropagation && localPropagation;
    }

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
    void makePseudo(mt::CR_BOL condition = true);

    /**
     * Make arguments must be provided explicitly
     * to be able to call the 'igniteCallbacks'.
     */
    void makeRequired(mt::CR_BOL condition = true) {
      required = {condition, condition};
    }

    /**
     * Make all the required descendants must be
     * completed to be able to call the 'igniteCallbacks'.
     */
    void makeStrict(mt::CR_BOL condition = true) {
      strict = condition;
    }

    /**
     * Make this cannot have children
     * and the existing can be deleted.
     */
    void makeSterilized(
      mt::CR_BOL condition = true,
      mt::CR_BOL willDestroy = false
    );

    /**
     * Make parent call this callback
     * before or after its own callback.
     */

    // before
    void registerAsInput() {
      if (getParent()) {
        asInput = true;
        asOutput = false;
      }
    }

    // after
    void registerAsOutput() {
      if (getParent()) {
        asInput = false;
        asOutput = true;
      }
    }

    /** Display information about this node */

    // displayed once
    void printWelcome();
    void printControllersHints();

    // print keyword, description, and children keywords
    void printHelp();

    /**
     * Print keyword and type.
     * Display '*' for required node.
     */
    void printKeyword(
      const CONSOLE_CODE &consoleCode,
      mt::CR_SZ numberOfIndents
    );

    // print children keywords
    void printList(
      const CONSOLE_CODE &consoleCode,
      mt::CR_SZ numberOfIndents,
      mt::CR_BOL displayAtLeafWarning
    );

    // print unordered map values associated with this
    virtual bool printInput() { return false; }
  };
}

#include "result.tpp"
#endif // __CLI_MENU__COMMAND_HPP__