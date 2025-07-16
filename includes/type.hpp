#ifndef __CLI_MENU__TYPE_HPP__
#define __CLI_MENU__TYPE_HPP__

#include "command.hpp"

namespace cli_menu {

  /** WORD */

  class Word : public Command {
  protected:
    void copyPaste() override;
    void pushUnormap(mt::CR_STR input) override;

  public:
    Word() = delete;

    Word(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      const COMMAND_CALLBACK &callback_in
    );
  };

  /** NUMBER */

  class Number : public Command {
  protected:
    void copyPaste() override;
    void pushUnormap(mt::CR_STR input) override;

  public:
    Number() = delete;

    Number(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      const COMMAND_CALLBACK &callback_in
    );
  };

  /** TOGGLE */

  class Toggle : public Command {
  protected:
    void copyPaste() override;
    void pushUnormap(mt::CR_STR input) override;

  public:
    Toggle() = delete;

    Toggle(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      mt::CR_BOL required_in,
      const COMMAND_CALLBACK &callback_in
    );
  };
}

#endif // __CLI_MENU__TYPE_HPP__