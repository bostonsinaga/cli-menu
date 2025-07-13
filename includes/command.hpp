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

  typedef const COMMAND_CODE& CR_COMMAND_CODE;

  class Command : public mt_ds::GeneralTree {
  private:
    std::string description;
    bool required, selecting = false;

    inline static bool
      dialogued = true,
      propagation = true;

    // the program will stop if the callback returns false
    typedef std::function<bool()> CALLBACK;
    typedef const CALLBACK& CR_CALLBACK;
    CALLBACK callback = []()->bool { return true; };

    COMMAND_CODE match(mt::CR_VEC_STR raws);
    COMMAND_CODE dialog();
    COMMAND_CODE enter();
    COMMAND_CODE goToNeighbor(mt_ds::LinkedList* neighbor);
    COMMAND_CODE resultInput(mt::CR_STR input);

    void printHelp();
    void printList();

  protected:
    std::string hyphens, keyword;

    Command(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in = false
    );

    virtual void copyPaste() {}
    virtual void pushUnormap(mt::CR_STR raw) {}
    virtual void resetUnormap() {}

  public:
    Command() = delete;

    // dialogue by default
    static void setNoDialog() { dialogued = false; }

    // propagate by default
    static void stopPropagation() { propagation = false; }

    // start match and dialog
    void run(
      mt::CR_INT argc,
      char *argv[]
    );
  };
}

#endif // __CLI_MENU__COMMAND_HPP__