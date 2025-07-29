#ifndef __CLI_MENU__TYPE_HPP__
#define __CLI_MENU__TYPE_HPP__

#include "command.hpp"

namespace cli_menu {

  /** WORD */

  class Word : public Command {
  private:
    void clipboardPaste() override;
    void pushUnormap(mt::CR_STR input) override;
    void resetUnormap() override;

  public:
    Word() = delete;

    Word(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      const COMMAND_CALLBACK &callback_in
    );
  };

  /** NUMBER */

  class Number : public Command {
  private:
    void clipboardPaste() override;
    void pushUnormap(mt::CR_STR input) override;
    void resetUnormap() override;

  public:
    Number() = delete;

    Number(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      const COMMAND_CALLBACK &callback_in
    );
  };

  /** TOGGLE */

  class Toggle : public Command {
  private:
    void clipboardPaste() override;
    void pushUnormap(mt::CR_STR input) override;
    void resetUnormap() override;

  public:
    Toggle() = delete;

    Toggle(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      const COMMAND_CALLBACK &callback_in
    );
  };
}

#endif // __CLI_MENU__TYPE_HPP__