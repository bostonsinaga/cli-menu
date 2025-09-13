#ifndef __CLI_MENU__TYPE_HPP__
#define __CLI_MENU__TYPE_HPP__

#include "command.hpp"

namespace cli_menu {

  class Word;
  class Number;
  class Toggle;

  /** CREATOR */

  class Creator : public Command {
  private:
    /**
     * Avoid keyword duplication (will destroy the existing command
     * if the keyword is equal to 'newCommand' keyword).
     * 
     * If this is 'sterilized', the 'newCommand' cannot replace
     * the same keyword or even be added as a child.
     */
    void replaceExistingKeyword(Command *newCommand);

  protected:
    Creator(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      CR_COMMAND_CALLBACK callback_in
    );

    Creator(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in
    );

    // add '--help' and '--list' toggles for each command
    void setPresetHelpList(); // defined at 'preset.cpp'

    friend class Preset;

  public:
    Creator() = delete;

    Word *createWord(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      CR_COMMAND_CALLBACK callback_in
    );

    Word *createWord(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in
    );

    Number *createNumber(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      CR_COMMAND_CALLBACK callback_in
    );

    Number *createNumber(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in
    );

    Toggle *createToggle(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      CR_COMMAND_CALLBACK callback_in
    );

    Toggle *createToggle(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in
    );
  };

  /** WORD */

  class Word : public Creator {
  protected:
    void clipboardPaste() override;
    void pushUnormap(mt::CR_STR input) override;
    void resetUnormap() override;
    bool printInput() override;

    Word(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      CR_COMMAND_CALLBACK callback_in
    );

    Word(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in
    );

    friend class Creator;
    friend class Preset;

  public:
    Word() = delete;
  };

  /** NUMBER */

  class Number : public Creator {
  protected:
    void clipboardPaste() override;
    void pushUnormap(mt::CR_STR input) override;
    void resetUnormap() override;
    bool printInput() override;

    Number(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      CR_COMMAND_CALLBACK callback_in
    );

    Number(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in
    );

    friend class Creator;
    friend class Preset;

  public:
    Number() = delete;
  };

  /** TOGGLE */

  class Toggle : public Creator {
  protected:
    void clipboardPaste() override;
    void pushUnormap(mt::CR_STR input) override;
    void resetUnormap() override;
    bool printInput() override;

    Toggle(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      CR_COMMAND_CALLBACK callback_in
    );

    Toggle(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in
    );

    friend class Creator;
    friend class Preset;

  public:
    Toggle() = delete;

    static bool instantBooleanize() {
      return true;
    }
  };
}

#endif // __CLI_MENU__TYPE_HPP__