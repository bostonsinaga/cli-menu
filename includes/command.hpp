#ifndef __CLI_MENU__COMMAND_HPP__
#define __CLI_MENU__COMMAND_HPP__

#include "result.hpp"
#include "clipboard.hpp"

namespace cli_menu {

  // final status codes
  enum COMMAND_CODE {
    COMMAND_FAILED, COMMAND_ONGOING,
    COMMAND_SUCCEEDED, COMMAND_TERMINATED,
    COMMAND_ENDED // silent exit
  };

  typedef std::function<bool(Command*)> COMMAND_CALLBACK;

  class Command : public mt_ds::GeneralTree {
  private:
    int pseudosCount = 0;

    bool editing = true,
      pseudo = false,
      strict = false,
      localDialogued = true,
      localPropagation = true;

    inline static bool
      matching = true,
      globalDialogued = true,
      globalPropagation = true;

    // return false to stop the program 
    COMMAND_CALLBACK processCallback;

    /**
     * The vector only contains if 'processCallback' exists.
     * The 'Command' will be passed to the callback.
     * 
     * This node can be said to be the 'Ultimate Command'
     * if one of these vectors contains the pair.
     */
    std::vector<mt::PAIR2<COMMAND_CALLBACK, Command*>>
      inputCallbacks, outputCallbacks;

    // prohibit controllers after match
    inline static bool interruptionDialogued = false;

    // accumulate keywords up to root
    std::string generateSequentialRootNames();

    // call input-process-output callbacks
    bool triggerCallbacks();

    // dialog interactions (extended runtime input)
    COMMAND_CODE dialog();
    COMMAND_CODE enter();
    COMMAND_CODE igniteCallbacks();
    COMMAND_CODE goDown(mt::CR_STR input);
    COMMAND_CODE goToNeighbor(const mt_ds::GeneralTree::DIRECTION &direction);

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

    /**
     * Entry point to dialog interactions.
     * The 'raws' only expected as keywords or arguments.
     * Note that the 'keyword' is not case sensitive.
     */
    COMMAND_CODE match();

    // better called after callbacks call
    void clipboardCopy() {
      Clipboard::copyText(Result::concatOutputs(this));
    }

    virtual void clipboardPaste() {}
    virtual void pushUnormap(mt::CR_STR input) {}
    virtual void resetUnormap() {}

  public:
    Command() = delete;

    /**
     * Input-output callbacks modifiers.
     * The 'processCallback' and the 'node'
     * must exist to be accepted by the vector.
     */

    void pushInputCallbacks(
      COMMAND_CALLBACK callback_in,
      Command* node
    );

    void pushOutputCallbacks(
      COMMAND_CALLBACK callback_in,
      Command* node
    );

    // removers
    void popInputCallbacks();
    void popOutputCallbacks();
    void clearInputCallbacks();
    void clearOutputCallbacks();

    // member variable access
    const bool isRequired() const { return required.first; }
    const std::string getHyphens() const { return hyphens; }
    const std::string getKeyword() const { return keyword; }
    const std::string getDescription() const { return description; }

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

    /**
     * Prevent 'igniteCallbacks' from bubbling callbacks to the root.
     * Make this will only call its own callbacks.
     */

    // local
    void noPropagation(mt::CR_BOL condition = true) {
      localPropagation = !condition;
    }

    // global
    static void banPropagation(mt::CR_BOL condition = true) {
      Command::globalPropagation = !condition;
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

    // display information about this
    void printWelcome();
    void printHelp();
    void printList(mt::CR_BOL withHelp);

    // print unordered map values associated with this
    virtual bool printInput() { return false; }
  };
}

#include "result.tpp"
#endif // __CLI_MENU__COMMAND_HPP__