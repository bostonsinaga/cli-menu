#ifndef __CLI_MENU__TYPE_HPP__
#define __CLI_MENU__TYPE_HPP__

#include "command.hpp"

namespace cli_menu {

  class Word;
  class Number;
  class Boolean;

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
      COMMAND_CALLBACK callback_in
    );

    Creator(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in
    );

    // add '--help' and '--list' booleans for each command
    void setPresetHelpList(); // defined at 'preset.cpp'

    friend class Preset;

  public:
    Creator() = delete;

    Word *createWord(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      COMMAND_CALLBACK callback_in
    );

    Word *createWord(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in
    );

    Number *createNumber(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      COMMAND_CALLBACK callback_in
    );

    Number *createNumber(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in
    );

    Boolean *createBoolean(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      COMMAND_CALLBACK callback_in
    );

    Boolean *createBoolean(
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
      COMMAND_CALLBACK callback_in
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
      COMMAND_CALLBACK callback_in
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

  /** BOOLEAN */

  enum BOOLEAN_INSTANT_QUESTION_CODE {
    BOOLEAN_INSTANT_QUESTION_CANCELED,
    BOOLEAN_INSTANT_QUESTION_NO,
    BOOLEAN_INSTANT_QUESTION_YES
  };

  class Boolean : public Creator {
  protected:
    void clipboardPaste() override;
    void pushUnormap(mt::CR_STR input) override;
    void resetUnormap() override;
    bool printInput() override;

    Boolean(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in,
      COMMAND_CALLBACK callback_in
    );

    Boolean(
      mt::CR_STR keyword_in,
      mt::CR_STR description_in
    );

    friend class Creator;
    friend class Preset;

  public:
    Boolean() = delete;

    // ask yes or no
    static BOOLEAN_INSTANT_QUESTION_CODE instantQuestion(
      const SENTENCE_CODE &responseCode,  // The 'xMessage::sentences[responseCode]' is expected
      mt::CR_STR replacementText          // to have 'xManager::placeholder' that will be replaced by 'replacementText'.
    );
  };
}

#endif // __CLI_MENU__TYPE_HPP__