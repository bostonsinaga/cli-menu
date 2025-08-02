#ifndef __CLI_MENU__COMMAND_HPP__
#define __CLI_MENU__COMMAND_HPP__

#include "result.hpp"
#include "clipboard.hpp"

namespace cli_menu {

  // final status codes
  enum COMMAND_CODE {
    COMMAND_FAILED, COMMAND_ONGOING,
    COMMAND_REQUIRED, COMMAND_SUCCEEDED, COMMAND_CANCELED
  };

  typedef std::function<bool()> COMMAND_CALLBACK;

  class Command : public mt_ds::GeneralTree {
  private:
    std::string description;

    bool editing = true,
      strict = false,
      localDialogued = true,
      localPropagation = true;

    inline static bool
      globalDialogued = true,
      globalPropagation = true;

    // return false to stop the program 
    COMMAND_CALLBACK callback = []()->bool { return true; };

    // prohibit controllers
    inline static bool interruptionDialogued = false;

    // accumulate keywords up to root
    std::string generateSequentialRootNames();

    // dialog interactions (extended runtime input)
    COMMAND_CODE dialog();
    COMMAND_CODE enter();
    COMMAND_CODE callCallback();
    COMMAND_CODE goToNeighbor(mt_ds::LinkedList* node);
    COMMAND_CODE goDown(mt::CR_STR input);

    void printHelp();
    void printList(mt::CR_BOL withHelp);
    void printInterruptionDialoguedResponse();

    // useful when parent is strict
    Command *getFirstRequiredNeighbor();

    bool testHyphens(mt::CR_STR input) {
      return hyphens + keyword == input;
    }

  protected:
    bool required = false;
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
      const COMMAND_CALLBACK &callback_in
    );

    /**
     * Entry point to dialog interactions.
     * The 'raws' only expected as keywords or arguments.
     */
    COMMAND_CODE match();

    // better called after callback call
    void clipboardCopy() {
      Clipboard::copyText(Result::getUltimate(keyword));
    }

    virtual void clipboardPaste() {}
    virtual void pushUnormap(mt::CR_STR input) { required = false; }
    virtual void resetUnormap() {}

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
    void noDialogue() {
      localDialogued = false;
    }

    // global
    static void banDialogue() {
      Command::globalDialogued = false;
    }

    /**
     * Prevent 'callCallback' from bubbling callback calls to the root.
     * Only call this callback.
     */

    // local
    void noPropagation() {
      localPropagation = false;
    }

    // global
    static void banPropagation() {
      Command::globalPropagation = false;
    }

    /**
     * Arguments must be provided explicitly
     * to be able to call the 'callCallback'.
     */
    void makeRequired() { required = true; }

    /**
     * All the required descendants must be completed
     * to be able to call the 'callCallback'.
     */
    void makeStrict() { strict = true; }
  };
}

#endif // __CLI_MENU__COMMAND_HPP__