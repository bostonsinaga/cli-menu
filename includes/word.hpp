#ifndef __CLI_MENU__WORD_HPP__
#define __CLI_MENU__WORD_HPP__

#include "command.hpp"

namespace cli_menu {

  class Word : public Command {
  private:
    void copyPaste() override;
    void pushUnormap(mt::CR_STR raw) override;

  public:
    Word() = delete;

    Word(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in = false
    );
  };
}

#endif // __CLI_MENU__WORD_HPP__