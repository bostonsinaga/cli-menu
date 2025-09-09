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
    std::string description;
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
    COMMAND_CALLBACK callback = [](Command *current)->bool { return true; };

    // prohibit controllers after match
    inline static bool interruptionDialogued = false;

    // accumulate keywords up to root
    std::string generateSequentialRootNames();

    // dialog interactions (extended runtime input)
    COMMAND_CODE dialog();
    COMMAND_CODE enter();
    COMMAND_CODE callCallback();
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
    bool required = false, sterilized = false;
    std::string hyphens, keyword;
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

    /**
     * Entry point to dialog interactions.
     * The 'raws' only expected as keywords or arguments.
     */
    COMMAND_CODE match();

    // better called after callback call
    void clipboardCopy() {
      Clipboard::copyText(Result::concatUltimates(this));
    }

    virtual void clipboardPaste() {}
    virtual void pushUnormap(mt::CR_STR input) {}
    virtual void resetUnormap() {}

    // display help with list or just list
    void printHelp();
    void printList(mt::CR_BOL withHelp);

  public:
    Command() = delete;

    // member variable access
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
     * Prevent 'callCallback' from bubbling callback calls to the root.
     * Only call this callback.
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
     * invoke its callback without moving from the parent to itself.
     * 
     * Make this also cannot entered with 'enterTest',
     * 'nextTest', or 'previousTest' in dialog.
     * 
     * DOES NOT APPLY TO THE ROOT.
     */
    void makePseudo(mt::CR_BOL condition = true);

    /**
     * Make arguments must be provided explicitly
     * to be able to call the 'callCallback'.
     */
    void makeRequired(mt::CR_BOL condition = true) {
      required = condition;
    }

    /**
     * Make all the required descendants must be
     * completed to be able to call the 'callCallback'.
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
  };
}

#include "result.tpp"
#endif // __CLI_MENU__COMMAND_HPP__