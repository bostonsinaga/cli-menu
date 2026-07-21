#ifndef __CLI_MENU__LANGUAGE_HPP__
#define __CLI_MENU__LANGUAGE_HPP__

#include "console.hpp"

namespace cli_menu {

  enum SENTENCE_CODE {
    SENTENCE_ARGUMENT_REQUIRED,
    SENTENCE_BOOLEAN_INSTANT_QUESTION_FORBIDDEN_CONTROLLER,
    SENTENCE_CLIPBOARD_OPEN_FAILURE,
    SENTENCE_CLIPBOARD_GLOBAL_LOCK_FAILURE,
    SENTENCE_CLIPBOARD_GLOBAL_ALLOC_FAILURE,
    SENTENCE_CLIPBOARD_GET_DATA_FAILURE,
    SENTENCE_CLIPBOARD_COPY_SUCCEED,
    SENTENCE_CLIPBOARD_PASTE_SUCCEED,
    SENTENCE_EMPTY_INPUT_THIS,
    SENTENCE_EMPTY_INPUT_DESCENDANTS,
    SENTENCE_EMPTY_OUTPUT_THIS,
    SENTENCE_EMPTY_OUTPUT_DESCENDANTS,
    SENTENCE_EMPTY_OUTPUT_NAMED,
    SENTENCE_FILE_OVERWRITE_QUESTION,
    SENTENCE_FILE_WRITE_FAILURE,
    SENTENCE_FILE_WRITE_SUCCEED,
    SENTENCE_FORBIDDEN_HIDDEN_PASTE,
    SENTENCE_INTERRUPTION_DIALOG,
    SENTENCE_KEYWORD_NOT_FOUND,
    SENTENCE_MODE_ALREADY_MODIFYING,
    SENTENCE_MODE_ALREADY_SELECTING,
    SENTENCE_MODE_SWITCH_TO_MODIFICATION,
    SENTENCE_MODE_SWITCH_TO_SELECTION,
    SENTENCE_PARAMETER_ALONE,
    SENTENCE_PARAMETER_AT_LEAF,
    SENTENCE_PARAMETER_AT_ROOT,
    SENTENCE_PARAMETER_NOT_FOUND,
    SENTENCE_PARAMETER_REQUIRED,
    SENTENCE_PARENT_STRICT,
    SENTENCE_PROGRAM_TERMINATED,
    SENTENCE_PROGRAM_ERROR,
    SENTENCE_PROGRAM_CANCELED,
    SENTENCE_PROGRAM_DONE,
    SENTENCE_RESET_INPUT_THIS,
    SENTENCE_RESET_INPUT_DESCENDANTS,
    SENTENCE_RESET_OUTPUT_THIS,
    SENTENCE_RESET_OUTPUT_DESCENDANTS
  };

  enum STRINGIFIED_TYPE_COMMAND_CODE {
    STRINGIFIED_TYPE_INPUT_WORD,
    STRINGIFIED_TYPE_INPUT_NUMBER,
    STRINGIFIED_TYPE_INPUT_BOOLEAN
  };

  /**
   * Can be obtained after testing
   * the input string in 'Control' class.
   */
  enum CONTROL_CODE {
    CONTROL_COMMAND_HELP,
    CONTROL_CONTROLLER_LIST,
    CONTROL_CHILDREN_ENTER,
    CONTROL_CHILDREN_EXECUTE,
    CONTROL_CHILDREN_LIST,
    CONTROL_NEIGHBOR_NEXT,
    CONTROL_NEIGHBOR_PREVIOUS,
    CONTROL_SWITCH_MODIFY,
    CONTROL_SWITCH_SELECT,
    CONTROL_CLEAR_SCREEN,
    CONTROL_VIEW_INPUT_THIS,
    CONTROL_VIEW_INPUT_DESCENDANTS,
    CONTROL_VIEW_OUTPUT_THIS,
    CONTROL_VIEW_OUTPUT_DESCENDANTS,
    CONTROL_RESET_INPUT_THIS,
    CONTROL_RESET_INPUT_DESCENDANTS,
    CONTROL_RESET_OUTPUT_THIS,
    CONTROL_RESET_OUTPUT_DESCENDANTS,
    CONTROL_RESET_DATA_THIS,
    CONTROL_RESET_DATA_DESCENDANTS,
    CONTROL_COPY_OUTPUT,
    CONTROL_PASTE_INPUT,
    CONTROL_PARENT_BACK,
    CONTROL_ROOT_BACK,
    CONTROL_PROGRAM_QUIT,
    CONTROL_UNKNOWN
  };

  enum PROGRAM_LABEL_CODE {
    PROGRAM_LABEL_VERSION,
    PROGRAM_LABEL_AUTHOR,
    PROGRAM_LABEL_LINK
  };

  enum PRESET_KEYWORD_CODE {
    PRESET_KEYWORD_IN,
    PRESET_KEYWORD_OUT,
    PRESET_KEYWORD_HELP,
    PRESET_KEYWORD_LIST
  };

  constexpr int SENTENCES_TOTAL = 37;
  constexpr int CONTROLS_TOTAL = 25;
  constexpr int CREATOR_TYPES_TOTAL = 3;
  constexpr int PROGRAM_LABELS_TOTAL = 3;
  constexpr int PRESET_KEYWORDS_TOTAL = 4;

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
      static mt::UNORMAP_STR<mt::ARR_STR<SENTENCES_TOTAL>> sentences;
      static CONSOLE_CODE consoleCodes[SENTENCES_TOTAL];
      static mt::UNORMAP_STR<std::string> welcomeToString;
    };

    struct xControl {
      static mt::UNORMAP_STR<mt::ARR_STR<CONTROLS_TOTAL>> terms;

      static mt::UNORMAP_STR<std::string>
        abbreviationsTitle,
        booleanAvailableValuesTitle;
    };

    struct xBooleanizer {
      inline static mt_uti::Booleanizer object;
      static mt::UNORMAP_STR<std::string> yesOrNoLabel;
    };

    struct xCreator {
      static mt::UNORMAP_STR<mt::ARR_STR<CREATOR_TYPES_TOTAL>> stringifiedTypes;
    };

    struct xProgram {
      static mt::UNORMAP_STR<mt::ARR_STR<PROGRAM_LABELS_TOTAL>> labels;
    };

    struct xPreset {
      static mt::UNORMAP_STR<mt::ARR_STR<PRESET_KEYWORDS_TOTAL>> keywords, descriptions;
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
        mt::CR_STR argumentRequiredSentence,
        mt::CR_STR booleanInstantQuestionForbiddenControllerSentence,
        mt::CR_STR sentenceClipboardOpenFailure,
        mt::CR_STR sentenceClipboardGlobalLockFailure,
        mt::CR_STR sentenceClipboardGlobalAllocFailure,
        mt::CR_STR sentenceClipboardGetDataFailure,
        mt::CR_STR sentenceClipboardCopySucceed,
        mt::CR_STR sentenceClipboardPasteSucceed,
        mt::CR_STR emptyInputThisSentence,
        mt::CR_STR emptyInputDescendantsSentence,
        mt::CR_STR emptyOutputThisSentence,
        mt::CR_STR emptyOutputDescendantsSentence,
        mt::CR_STR emptyOutputNamedSentence,
        mt::CR_STR fileOverwriteQuestionSentence,
        mt::CR_STR fileWriteFailureSentence,
        mt::CR_STR fileWriteSucceedSentence,
        mt::CR_STR forbiddenHiddenPasteSentence,
        mt::CR_STR interruptionDialogSentence,
        mt::CR_STR keywordNotFoundSentence,
        mt::CR_STR modeAlreadyModifyingSentence,
        mt::CR_STR modeAlreadySelectingSentence,
        mt::CR_STR modeSwitchToModificationSentence,
        mt::CR_STR modeSwitchToSelectionSentence,
        mt::CR_STR parameterAloneSentence,
        mt::CR_STR parameterAtLeafSentence,
        mt::CR_STR parameterAtRootSentence,
        mt::CR_STR parameterNotFoundSentence,
        mt::CR_STR parameterRequiredSentence,
        mt::CR_STR parentStrictSentence,
        mt::CR_STR programTerminatedSentence,
        mt::CR_STR programErrorSentence,
        mt::CR_STR programCanceledSentence,
        mt::CR_STR programDoneSentence,
        mt::CR_STR resetInputThisSentence,
        mt::CR_STR resetInputDescendantsSentence,
        mt::CR_STR resetOutputThisSentence,
        mt::CR_STR resetOutputDescendantsSentence
      );

      static void printResponse(
        const SENTENCE_CODE &responseCode,
        mt::CR_BOL withYesOrNoLabel = false
      );

      static void printTemplateResponse(
        const SENTENCE_CODE &responseCode,
        mt::CR_STR replacementText,
        mt::CR_BOL withYesOrNoLabel = false
      );

      static std::string getWelcomeToString();
    };

    struct ageControl {

      static void setTerms(
        std::string commandHelpTerm,
        std::string controllerListTerm,
        std::string childrenEnterTerm,
        std::string childrenExecuteTerm,
        std::string childrenListTerm,
        std::string neighborNextTerm,
        std::string neighborPreviousTerm,
        std::string switchModifyTerm,
        std::string switchSelectTerm,
        std::string cleanScreenTerm,
        std::string viewInputThisTerm,
        std::string viewInputDescendantsTerm,
        std::string viewOutputThisTerm,
        std::string viewOutputDescendantsTerm,
        std::string resetInputThisTerm,
        std::string resetInputDescendantsTerm,
        std::string resetOutputThisTerm,
        std::string resetOutputDescendantsTerm,
        std::string resetDataThisTerm,
        std::string resetDataDescendantsTerm,
        std::string copyOutputTerm,
        std::string pasteInputTerm,
        std::string parentBackTerm,
        std::string rootBackTerm,
        std::string programQuitTerm
      );

      static void setAbbreviationsTitle(mt::CR_STR title);
      static void setBooleanAvailableValuesTitle(mt::CR_STR title);

      static std::string getTerm(const CONTROL_CODE &code);
      static std::string getAbbreviationsTitle();
      static std::string getBooleanAvailableValuesTitle();
    };

    struct ageBooleanizer {

      static void setTerms(
        mt::CR_VEC_STR existingTrueTerms,
        mt::CR_VEC_STR existingFalseTerms
      );

      static void setYesOrNoLabel(
        mt::CR_STR existingYesOrNoLabel
      );

      static bool test(mt::CR_STR raw);
      static mt::PAIR<mt::VEC_STR> getTerms();
      static std::string getYesOrNoLabel();
    };

    struct ageCreator {

      static void setStringifiedTypes(
        mt::CR_STR wordStringifiedType,
        mt::CR_STR numberStringifiedType,
        mt::CR_STR booleanStringifiedType
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

      static std::string fileOutDefaultExtension;
      static mt::PAIR_STR fileOutBracketsForNumbering;

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
        const PRESET_KEYWORD_CODE &code
      );

      static std::string getDescription(
        const PRESET_KEYWORD_CODE &code
      );
    };
  };
}

#endif // __CLI_MENU__LANGUAGE_HPP__