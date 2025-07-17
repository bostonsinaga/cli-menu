#ifndef __CLI_MENU__COMMAND_HPP__
#define __CLI_MENU__COMMAND_HPP__

#include "clipboard.hpp"
#include "result.hpp"

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
      required = false,
      strict = false;

    inline static bool
      dialogued = true,
      propagation = true;

    // return false to stop the program 
    COMMAND_CALLBACK callback = []()->bool { return true; };

    // accumulate keywords up to root
    std::string generateSequentialRootNames();

    // entry point to dialog interactions
    COMMAND_CODE match(mt::CR_VEC_STR raws);

    // dialog interactions
    COMMAND_CODE dialog();
    COMMAND_CODE enter();
    COMMAND_CODE callCallback();
    COMMAND_CODE goToNeighbor(mt_ds::LinkedList* neighbor);
    COMMAND_CODE goDown(mt::CR_STR input);

    void printHelp();
    void printList(mt::CR_BOL needErrorMessage);

  protected:
    std::string hyphens, keyword;
    LANGUAGE_COMMAND_STRINGIFIED_TYPE stringifiedTypeIndex;

    Command(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      const COMMAND_CALLBACK &callback_in
    );

    virtual void copyPaste() {}
    virtual void pushUnormap(mt::CR_STR raw) {}
    virtual void resetUnormap() {}

  public:
    Command() = delete;

    /**
     * Will not open dialog to complete the required.
     * Directly display 'COMMAND_FAILED'.
     */
    static void noDialogue() {
      Command::dialogued = false;
    }

    /**
     * Prevent 'callCallback' from bubbling callback calls to the root.
     * Only call this callback.
     */
    static void stopPropagation() {
      Command::propagation = false;
    }

    /**
     * Arguments must be provided explicitly to make
     * strict parent able to call the 'callCallback'.
     */
    void makeRequired() { required = true; }

    /**
     * All the required descendants must be completed
     * to be able to call the 'callCallback'.
     */
    void makeStrict() { strict = true; }

    /**
     * Start match and dialog.
     * This will not be deleted automatically.
     * Delete it manually by calling 'destroy'.
     */
    void run(mt::CR_INT argc, char *argv[]);
  };
}

#endif // __CLI_MENU__COMMAND_HPP__