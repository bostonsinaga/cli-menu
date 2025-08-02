#ifndef __CLI_MENU__TYPE_HPP__
#define __CLI_MENU__TYPE_HPP__

#include "command.hpp"

namespace cli_menu {

  class Word;
  class Number;
  class Toggle;

  /** CREATOR */

  class Creator : public Command {
  protected:
    Creator(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      const COMMAND_CALLBACK &callback_in
    );

  public:
    Creator() = delete;

    Word *createWord(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      const COMMAND_CALLBACK &callback_in
    );

    Number *createNumber(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      const COMMAND_CALLBACK &callback_in
    );

    Toggle *createToggle(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      const COMMAND_CALLBACK &callback_in
    );
  };

  /** WORD */

  class Word : public Creator {
  protected:
    void clipboardPaste() override;
    void pushUnormap(mt::CR_STR input) override;
    void resetUnormap() override;

    Word(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      const COMMAND_CALLBACK &callback_in
    );

    friend class Creator;

  public:
    Word() = delete;
  };

  /** NUMBER */

  class Number : public Creator {
  protected:
    void clipboardPaste() override;
    void pushUnormap(mt::CR_STR input) override;
    void resetUnormap() override;

    Number(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      const COMMAND_CALLBACK &callback_in
    );

    friend class Creator;

  public:
    Number() = delete;
  };

  /** TOGGLE */

  class Toggle : public Creator {
  protected:
    void clipboardPaste() override;
    void pushUnormap(mt::CR_STR input) override;
    void resetUnormap() override;

    Toggle(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      const COMMAND_CALLBACK &callback_in
    );

    friend class Creator;

  public:
    Toggle() = delete;
  };
}

#endif // __CLI_MENU__TYPE_HPP__