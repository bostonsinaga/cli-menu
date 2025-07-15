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
    bool required, selecting = false;

    inline static bool
      dialogued = true,
      propagation = true;

    // return false to stop the program 
    COMMAND_CALLBACK callback = []()->bool { return true; };

    // dialog interactions
    COMMAND_CODE dialog();
    COMMAND_CODE enter();
    COMMAND_CODE goToNeighbor(mt_ds::LinkedList* neighbor);
    COMMAND_CODE resultInput(mt::CR_STR input);

    // entry point to dialog interactions
    COMMAND_CODE match(mt::CR_VEC_STR raws);

    void printHelp();
    void printList();

  protected:
    std::string hyphens, keyword;

    virtual void copyPaste() {}
    virtual void pushUnormap(mt::CR_STR raw) {}
    virtual void resetUnormap() {}

  public:
    Command() = delete;

    Command(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      const COMMAND_CALLBACK &callback_in
    );

    // dialogue by default
    static void setNoDialog() {
      Command::dialogued = false;
    }

    // propagate by default
    static void stopPropagation() {
      Command::propagation = false;
    }

    // start match and dialog
    void run(mt::CR_INT argc, char *argv[]);
  };
}

#endif // __CLI_MENU__COMMAND_HPP__