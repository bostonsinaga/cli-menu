#ifndef __CLI_MENU__TYPE_HPP__
#define __CLI_MENU__TYPE_HPP__

#include "data.hpp"
#include "command.hpp"

namespace cli_menu {

  /** PARAMETER */

  template <class T, class U>
  class Parameter : virtual public Command {
  private:
    void clipboardOutputCopy() override;
    void printInput() override;
    void printOutput() override;
    void printDescendantInputs() override;
    void printDescendantOutputs() override;
    void resetInput() override;
    void resetOutput() override;
    void resetDescendantInputs() override;
    void resetDescendantOutputs() override;

  protected:
    Parameter(
      mt::CR_STR keyw,
      mt::CR_STR desc,
      mt::CR<CODE_CALLBACK> calb
    ) : Command(keyw, desc, calb) {}

  public:
    Parameter() = delete;

    // generated data
    Data<T> input;  // in dialog or match
    Data<U> output; // after callback
  };

  /** CREATOR */

  class Word;
  class Number;
  class Boolean;

  typedef Parameter<std::string, std::string> ParameterWord;
  typedef Parameter<double, std::string> ParameterNumber;
  typedef Parameter<bool, std::string> ParameterBoolean;

  template <typename T>
  concept ParameterType =
    std::is_same_v<T, Word> ||
    std::is_same_v<T, Number> ||
    std::is_same_v<T, Boolean>;

  class Creator : virtual public Command {
  protected:
    Creator(
      mt::CR_STR keyw,
      mt::CR_STR desc,
      mt::CR<CODE_CALLBACK> calb
    ) : Command(keyw, desc, calb) {}

    /**
     * Avoid keyword duplication (will destroy existing child
     * if the keyword is the same as the new child keyword).
     * 
     * If this is sterilized, the new child cannot replace
     * the same keyword or even be added as a child.
     */
    void replaceExistingChildByKeyword(Command *newChild);

  public:
    Creator() = delete;

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

  class Word : public ParameterWord, public Creator {
  protected:
    void clipboardInputPaste() override;
    void strargv(mt::CR_STR raw) override;

    Word(
      mt::CR_STR keyw,
      mt::CR_STR desc,
      mt::CR<CODE_CALLBACK> calb
    );

    friend class Creator;

  public:
    Word() = delete;
  };

  /** NUMBER */

  class Number : public ParameterNumber, public Creator {
  protected:
    void clipboardInputPaste() override;
    void strargv(mt::CR_STR raw) override;

    Number(
      mt::CR_STR keyw,
      mt::CR_STR desc,
      mt::CR<CODE_CALLBACK> calb
    );

    friend class Creator;

  public:
    Number() = delete;
  };

  /** BOOLEAN */

  enum BOOLEAN_INSTANT_QUESTION_CODE {
    BOOLEAN_INSTANT_QUESTION_NO,
    BOOLEAN_INSTANT_QUESTION_YES,
    BOOLEAN_INSTANT_QUESTION_CANCELED
  };

  class Boolean : public ParameterBoolean, public Creator {
  protected:
    void clipboardInputPaste() override;
    void strargv(mt::CR_STR raw) override;

    Boolean(
      mt::CR_STR keyw,
      mt::CR_STR desc,
      mt::CR<CODE_CALLBACK> calb
    );

    friend class Creator;

  public:
    Boolean() = delete;

    // ask yes or no
    static BOOLEAN_INSTANT_QUESTION_CODE instantQuestion(
      const SENTENCE_CODE &responseCode,  // the 'xMessage::sentences[responseCode]' is expected
      mt::CR_STR replacementText          // to have 'xManager::placeholder' that will be replaced by 'replacementText'.
    );
  };
}

#include "type.tpp"
#endif // __CLI_MENU__TYPE_HPP__