#ifndef __CLI_MENU__LANGUAGE_HPP__
#define __CLI_MENU__LANGUAGE_HPP__

#include "console.hpp"

namespace cli_menu {

  enum LANGUAGE_CODE {
    LANGUAGE_ALREADY_MODIFYING,
    LANGUAGE_ALREADY_SELECTING,
    LANGUAGE_ARGUMENT_REQUIRED,
    LANGUAGE_CLIPBOARD_COPIED_FAILURE,
    LANGUAGE_CLIPBOARD_COPIED_SUCCEED,
    LANGUAGE_CLIPBOARD_FORBIDDEN_HIDDEN_PASTE,
    LANGUAGE_CLIPBOARD_GET_FAILURE,
    LANGUAGE_CLIPBOARD_LOCK_FAILURE,
    LANGUAGE_CLIPBOARD_MEMORY_ALLOCATION_FAILURE,
    LANGUAGE_CLIPBOARD_OPEN_FAILURE,
    LANGUAGE_CLIPBOARD_PASTED,
    LANGUAGE_INTERRUPTION_DIALOG,
    LANGUAGE_KEYWORD_NOT_FOUND,
    LANGUAGE_PARAMETER_ALONE,
    LANGUAGE_PARAMETER_AT_LEAF,
    LANGUAGE_PARAMETER_AT_ROOT,
    LANGUAGE_PARAMETER_NOT_FOUND,
    LANGUAGE_PARENT_STRICT,
    LANGUAGE_PROGRAM_TERMINATED,
    LANGUAGE_PROGRAM_FAILED,
    LANGUAGE_PROGRAM_SUCCEEDED
  };

  enum STRINGIFIED_TYPE_COMMAND_CODE {
    STRINGIFIED_TYPE_WORD,
    STRINGIFIED_TYPE_NUMBER,
    STRINGIFIED_TYPE_TOGGLE
  };

  /**
   * Can be obtained after testing
   * the input string in 'Control' class.
   */
  enum CONTROL_CODE {
    CONTROL_UNKNOWN,
    CONTROL_HELP,
    CONTROL_LIST,
    CONTROL_ENTER,
    CONTROL_BACK,
    CONTROL_NEXT,
    CONTROL_PREVIOUS,
    CONTROL_MODIFY,
    CONTROL_SELECT,
    CONTROL_RESET_THIS,
    CONTROL_VIEW_THIS,
    CONTROL_RESET_ALL,
    CONTROL_VIEW_ALL,
    CONTROL_COPY,
    CONTROL_PASTE,
    CONTROL_QUIT
  };

  enum PROGRAM_LABEL_CODE {
    PROGRAM_LABEL_VERSION,
    PROGRAM_LABEL_AUTHOR,
    PROGRAM_LABEL_LINK
  };

  enum PRESET_CODE {
    PRESET_IN,
    PRESET_OUT,
    PRESET_HELP,
    PRESET_LIST
  };

  class Langu final {
  private:
    struct xManager {
      inline static std::string
        // default english
        currentISOCode = "en",
        // will be replaced with text
        placeholder = "$";
    };

    struct xMessage {
      static constexpr int totalSentences = 21;
      static mt::STRUNORMAP<mt::ARR_STR<totalSentences>> sentences;
      static CONSOLE_CODE consoleCodes[totalSentences];
    };

    struct xControl {

      static constexpr int totalSymbols = 15;
      inline static size_t maxTermLength = 16;
      static mt::STRUNORMAP<mt::ARR_STR<totalSymbols>> terms;

      static mt::STRUNORMAP_STR
        abbreviationsTitle,
        toggleAvailableValuesTitle;

      static void limitTerm(
        const CONTROL_CODE &code,
        std::string &newTerm
      );
    };

    struct xBooleanizer {
      inline static mt_uti::Booleanizer object;
    };

    struct xCommand {
      static constexpr int totalTypes = 3;
      static mt::STRUNORMAP<mt::ARR_STR<totalTypes>> stringifiedTypes;
    };

    struct xProgram {
      static constexpr int totalLabels = 3;
      static mt::STRUNORMAP<mt::ARR_STR<totalLabels>> labels;
    };

    struct xPreset {
      static constexpr int totalKeywords = 4;
      static mt::STRUNORMAP<mt::ARR_STR<totalKeywords>> keywords, descriptions;
    };

  public:
    Langu() = delete;

    struct ageManager {
      // ask keyword existance in 'messages'
      static bool hasISOCode(mt::CR_STR existingISOCode);

      // change 'currentISOCode' value
      static void selectISOCode(mt::CR_STR existingISOCode);

      // sync 'currentISOCode' with 'selectISOCode()' manually
      static void addISOCode(mt::CR_STR newISOCode);
      static void removeISOCode(mt::CR_STR existingISOCode);

      // it is recommended to use special characters
      static void changePlaceholder(mt::CR_STR newPlaceholder) {
        xManager::placeholder = newPlaceholder;
      }
    };

    struct ageMessage {

      static void setSentences(
        mt::CR_STR alreadyModifyingSentence,
        mt::CR_STR alreadySelectingSentence,
        mt::CR_STR argumentRequiredSentence,
        mt::CR_STR clipboardCopiedFailureSentence,
        mt::CR_STR clipboardCopiedSucceedSentence,
        mt::CR_STR clipboardForbiddenHiddenPasteSentence,
        mt::CR_STR clipboardGetFailureSentence,
        mt::CR_STR clipboardLockFailureSentence,
        mt::CR_STR clipboardMemoryAllocationFailureSentence,
        mt::CR_STR clipboardOpenFailureSentence,
        mt::CR_STR clipboardPastedSentence,        
        mt::CR_STR interruptionDialogSentence,
        mt::CR_STR keywordNotFoundSentence,
        mt::CR_STR parameterAloneSentence,
        mt::CR_STR parameterAtLeafSentence,
        mt::CR_STR parameterAtRootSentence,
        mt::CR_STR parameterNotFoundSentence,
        mt::CR_STR parentStrictSentence,
        mt::CR_STR programTerminatedSentence,
        mt::CR_STR programFailedSentence,
        mt::CR_STR programSucceededSentence
      );

      static void printResponse(
        const LANGUAGE_CODE &responseCode
      );

      static void printTemplateResponse(
        const LANGUAGE_CODE &responseCode,
        mt::CR_STR replacementText
      );
    };

    struct ageControl {

      // string length is limited to 'maxTermLength'
      static void setTerms(
        std::string helpTerm,
        std::string listTerm,
        std::string enterTerm,
        std::string backTerm,
        std::string nextTerm,
        std::string previousTerm,
        std::string modifyTerm,
        std::string selectTerm,
        std::string resetThisTerm,
        std::string viewThisTerm,
        std::string resetAllTerm,
        std::string viewAllTerm,
        std::string copyTerm,
        std::string pasteTerm,
        std::string quitTerm
      );

      static void setAbbreviationsTitle(mt::CR_STR title);
      static void setToggleAvailableValuesTitle(mt::CR_STR title);

      static std::string getTerm(const CONTROL_CODE &code);
      static std::string getAbbreviationsTitle();
      static std::string getToggleAvailableValuesTitle();
    };

    struct ageBooleanizer {

      static void setTerms(
        mt::CR_VEC_STR existingTrueTerms,
        mt::CR_VEC_STR existingFalseTerms
      );

      static bool test(mt::CR_STR raw);
      static mt::PAIR<mt::VEC_STR> getTerms();
    };

    struct ageCommand {

      static void setStringifiedTypes(
        mt::CR_STR wordStringifiedType,
        mt::CR_STR numberStringifiedType,
        mt::CR_STR toggleStringifiedType
      );

      static std::string getStringifiedType(
        const STRINGIFIED_TYPE_COMMAND_CODE &code
      );
    };

    struct ageProgram {

      static void setLabels(
        mt::CR_STR versionLabel,
        mt::CR_STR authorLabel,
        mt::CR_STR linkLabel
      );

      static std::string getLabel(
        const PROGRAM_LABEL_CODE &code
      );
    };

    struct agePreset {

      static void setKeywords(
        mt::CR_STR inKeyword,
        mt::CR_STR outKeyword,
        mt::CR_STR helpKeyword,
        mt::CR_STR listKeyword
      );

      static void setDescription(
        mt::CR_STR inDescription,
        mt::CR_STR outDescription,
        mt::CR_STR helpDescription,
        mt::CR_STR listDescription
      );

      static std::string getKeyword(
        const PRESET_CODE &code
      );

      static std::string getDescription(
        const PRESET_CODE &code
      );
    };
  };
}

#endif // __CLI_MENU__LANGUAGE_HPP__