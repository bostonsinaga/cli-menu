#ifndef __CLI_MENU__COMMAND_HPP__
#define __CLI_MENU__COMMAND_HPP__

#include "clipboard.hpp"
#include "control.hpp"
#include "result.hpp"

namespace cli_menu {

  class Command : public mt_ds::GeneralTree {
  public:
    // final status codes
    enum CODE {
      ERROR, SUCCEED, CANCELED
    };

  private:
    std::string keyword, description;
    bool required;

    inline static bool
      dialogued = true,
      propagation = true;

    // the program will stop if the callback returns false
    typedef std::function<bool()> CALLBACK;
    typedef const CALLBACK& CR_CALLBACK;
    CALLBACK callback;

    CODE match(mt::CR_VEC_STR raws);
    CODE dialog();

  protected:
    std::string hyphens;
    Command() = delete;

    Command(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in = false
    );

    virtual void copyPaste() {}
    virtual void pushUnormap(mt::CR_STR raw) {}

  public:
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