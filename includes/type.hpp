#ifndef __CLI_MENU__TYPE_HPP__
#define __CLI_MENU__TYPE_HPP__

#include "command.hpp"

namespace cli_menu {

  /** PARAMETER */

  class Word;
  class Number;
  class Boolean;

  template <typename T>
  concept ParameterType =
    std::is_same_v<T, Word> ||
    std::is_same_v<T, Number> ||
    std::is_same_v<T, Boolean>;

  class Parameter : public Command {
  protected:
    Parameter(
      mt::CR_STR keyw,
      mt::CR_STR desc,
      mt::CR<CODE_CALLBACK> calb
    ) : Command(keyw, desc, calb) {}

    /** Input & Output Methods */

    void clipboardInputPaste() override;
    void clipboardOutputCopy() override;
    void printOutput() override;
    void printDescendantOutputs() override;
    void resetOutput() override;
    void resetDescendantOutputs() override;

    template <UNORMAP_COMVEC_TYPE T>
    void printInput_temp();

    template <UNORMAP_COMVEC_TYPE T>
    void printDescendantInputs_temp();

    template <UNORMAP_COMVEC_TYPE T>
    void resetInput_temp();

    template <UNORMAP_COMVEC_TYPE T>
    void resetDescendantInputs_temp();

    /**
     * Avoid keyword duplication (will destroy existing child
     * if the keyword is the same as the new child keyword).
     * 
     * If this is sterilized, the new child cannot replace
     * the same keyword or even be added as a child.
     */
    void replaceExistingChildByKeyword(Command *newChild);

  public:
    Parameter() = delete;

    Word *addWord(
      mt::CR_STR keyw,
      mt::CR_STR desc,
      mt::CR<CODE_CALLBACK> calb = Command::defaultCallback,
      mt::CR_BOL applyPresetHelpList = true
    );

    Number *addNumber(
      mt::CR_STR keyw,
      mt::CR_STR desc,
      mt::CR<CODE_CALLBACK> calb = Command::defaultCallback,
      mt::CR_BOL applyPresetHelpList = true
    );

    Boolean *addBoolean(
      mt::CR_STR keyw,
      mt::CR_STR desc,
      mt::CR<CODE_CALLBACK> calb = Command::defaultCallback,
      mt::CR_BOL applyPresetHelpList = true
    );

    // will be defined in 'preset.cpp'
    void setPresetHelpList();
  };

  /** WORD */

  class Word : public Parameter {
  protected:
    void printInput() override;
    void printDescendantInputs() override;
    void resetInput() override;
    void resetDescendantInputs() override;
    void strargv(mt::CR_STR raw) override;

    Word(
      mt::CR_STR keyw,
      mt::CR_STR desc,
      mt::CR<CODE_CALLBACK> calb
    );

    friend class Parameter;

  public:
    Word() = delete;
    void destroy() override;
  };

  /** NUMBER */

  class Number : public Parameter {
  protected:
    void printInput() override;
    void printDescendantInputs() override;
    void resetInput() override;
    void resetDescendantInputs() override;
    void strargv(mt::CR_STR raw) override;

    Number(
      mt::CR_STR keyw,
      mt::CR_STR desc,
      mt::CR<CODE_CALLBACK> calb
    );

    friend class Parameter;

  public:
    Number() = delete;
    void destroy() override;
  };

  /** BOOLEAN */

  enum BOOLEAN_INSTANT_QUESTION_CODE {
    BOOLEAN_INSTANT_QUESTION_NO,
    BOOLEAN_INSTANT_QUESTION_YES,
    BOOLEAN_INSTANT_QUESTION_CANCELED
  };

  class Boolean : public Parameter {
  protected:
    void clipboardInputPaste() override;
    void printInput() override;
    void printDescendantInputs() override;
    void resetInput() override;
    void resetDescendantInputs() override;
    void strargv(mt::CR_STR raw) override;

    Boolean(
      mt::CR_STR keyw,
      mt::CR_STR desc,
      mt::CR<CODE_CALLBACK> calb
    );

    friend class Parameter;

  public:
    Boolean() = delete;
    void destroy() override;

    // ask yes or no
    static BOOLEAN_INSTANT_QUESTION_CODE instantQuestion(
      const SENTENCE_CODE &responseCode,  // the 'xMessage::sentences[responseCode]' is expected
      mt::CR_STR replacementText          // to have 'xManager::placeholder' that will be replaced by 'replacementText'.
    );
  };
}

#include "type.tpp"
#endif // __CLI_MENU__TYPE_HPP__