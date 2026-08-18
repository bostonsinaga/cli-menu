#ifndef __CLI_MENU__LANGUAGE_HPP__
#define __CLI_MENU__LANGUAGE_HPP__

#include "console.hpp"

namespace cli_menu {

  enum SENTENCE_CODE {
    SENTENCE_ARGUMENT_ADDED,
    SENTENCE_ARGUMENT_REQUIRED,
    SENTENCE_CLIPBOARD_OPEN_FAILURE,
    SENTENCE_CLIPBOARD_GLOBAL_LOCK_FAILURE,
    SENTENCE_CLIPBOARD_GLOBAL_ALLOC_FAILURE,
    SENTENCE_CLIPBOARD_GET_DATA_FAILURE,
    SENTENCE_CLIPBOARD_COPY_SUCCEED,
    SENTENCE_CLIPBOARD_PASTE_SUCCEED,
    SENTENCE_COMMAND_UNREDO_NO_NEW_VISITS,
    SENTENCE_EMPTY_INPUT_THIS,
    SENTENCE_EMPTY_OUTPUT_THIS,
    SENTENCE_EMPTY_OUTPUT_NAMED,
    SENTENCE_FILE_OVERWRITE_QUESTION,
    SENTENCE_FILE_WRITE_FAILURE,
    SENTENCE_FILE_WRITE_SUCCEED,
    SENTENCE_FORBIDDEN_CONTROLLER,
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
    SENTENCE_PARAMETER_REQUIRED,
    SENTENCE_PARENT_STRICT,
    SENTENCE_PROGRAM_TERMINATED,
    SENTENCE_PROGRAM_ERROR,
    SENTENCE_PROGRAM_CANCELED,
    SENTENCE_PROGRAM_DONE,
    SENTENCE_RESET_INPUT_THIS,
    SENTENCE_RESET_INPUT_DESCENDANTS,
    SENTENCE_RESET_OUTPUT_THIS,
    SENTENCE_RESET_OUTPUT_DESCENDANTS,
    SENTENCE_UNKNOWN_VALUE
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
    CONTROL_CHILDREN_LIST,
    CONTROL_CLEAR_SCREEN,
    CONTROL_NEIGHBOR_NEXT,
    CONTROL_NEIGHBOR_PREVIOUS,
    CONTROL_CHILDREN_ENTER,
    CONTROL_CHILDREN_EXECUTE,
    CONTROL_SWITCH_MODIFY,
    CONTROL_SWITCH_SELECT,
    CONTROL_VIEW_INPUT_THIS,
    CONTROL_VIEW_INPUT_CHILDREN,
    CONTROL_VIEW_OUTPUT_THIS,
    CONTROL_VIEW_OUTPUT_CHILDREN,
    CONTROL_RESET_INPUT_THIS,
    CONTROL_RESET_INPUT_DESCENDANTS,
    CONTROL_RESET_OUTPUT_THIS,
    CONTROL_RESET_OUTPUT_DESCENDANTS,
    CONTROL_RESET_DATA_THIS,
    CONTROL_RESET_DATA_DESCENDANTS,
    CONTROL_COPY_OUTPUT,
    CONTROL_PASTE_INPUT,
    CONTROL_NODE_LEVEL_UNDO,
    CONTROL_NODE_LEVEL_REDO,
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
    PRESET_KEYWORD_LIST,
    PRESET_KEYWORD_COLOR_SET
  };

  constexpr int SENTENCE_TOTAL = 37;
  constexpr int CONTROL_TOTAL = 27;
  constexpr int PARAMETER_TYPE_TOTAL = 3;
  constexpr int PROGRAM_LABEL_TOTAL = 3;
  constexpr int PRESET_KEYWORD_TOTAL = 5;

  class Langu final {
  private:
    struct xManager {
      inline static std::string
      // default is english
        defaultISOCode = "en",
        currentISOCode = defaultISOCode,
        // will be replaced with text
        placeholder = "$";
    };

    struct xMessage {
      static mt::UNORMAP_STR<mt::ARR_STR<SENTENCE_TOTAL>> sentences;
      static CONSOLE_CODE consoleCodes[SENTENCE_TOTAL];
      static mt::UNORMAP_STR<std::string> welcomeToString;
    };

    struct xColorSet {
      static mt::UNORMAP_STR<std::string> title;
      static mt::UNORMAP_STR<mt::ARR_STR<COLOR_TOTAL>> names;
    };

    struct xControl {
      static mt::UNORMAP_STR<mt::ARR_STR<CONTROL_TOTAL>> terms;

      static mt::UNORMAP_STR<std::string>
        abbreviationsTitle,
        abbreviationsDescription,
        booleanAvailableValuesTitle,
        booleanAvailableValuesDescription;
    };

    struct xBooleanizer {
      inline static mt_uti::Booleanizer object;
      static mt::UNORMAP_STR<std::string> yesOrNoLabel;
    };

    struct xParameter {
      static mt::UNORMAP_STR<mt::ARR_STR<PARAMETER_TYPE_TOTAL>> stringifiedTypes;
    };

    struct xProgram {
      static mt::UNORMAP_STR<mt::ARR_STR<PROGRAM_LABEL_TOTAL>> labels;
    };

    struct xPreset {
      static mt::UNORMAP_STR<mt::ARR_STR<PRESET_KEYWORD_TOTAL>> keywords, descriptions;
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

      // insert each replacement into each placeholder
      static std::string replaceTemplateString(
        mt::CR_STR templateString,
        mt::VEC_STR replacementStrings
      );
    };

    struct ageMessage {
      static void setSentences(
        mt::CR_STR argumentAddedSentence,
        mt::CR_STR argumentRequiredSentence,
        mt::CR_STR clipboardOpenFailureSentence,
        mt::CR_STR clipboardGlobalLockFailureSentence,
        mt::CR_STR clipboardGlobalAllocFailureSentence,
        mt::CR_STR clipboardGetDataFailureSentence,
        mt::CR_STR clipboardCopySucceedSentence,
        mt::CR_STR clipboardPasteSucceedSentence,
        mt::CR_STR commandUnredoNoNewVisitsSentence,
        mt::CR_STR emptyInputThisSentence,
        mt::CR_STR emptyOutputThisSentence,
        mt::CR_STR emptyOutputNamedSentence,
        mt::CR_STR fileOverwriteQuestionSentence,
        mt::CR_STR fileWriteFailureSentence,
        mt::CR_STR fileWriteSucceedSentence,
        mt::CR_STR forbiddenControllerSentence,
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
        mt::CR_STR parameterRequiredSentence,
        mt::CR_STR parentStrictSentence,
        mt::CR_STR programTerminatedSentence,
        mt::CR_STR programErrorSentence,
        mt::CR_STR programCanceledSentence,
        mt::CR_STR programDoneSentence,
        mt::CR_STR resetInputThisSentence,
        mt::CR_STR resetInputDescendantsSentence,
        mt::CR_STR resetOutputThisSentence,
        mt::CR_STR resetOutputDescendantsSentence,
        mt::CR_STR unknownValueSentence
      );

      static void printResponse(
        const SENTENCE_CODE &responseCode,
        mt::CR_BOL withYesOrNoLabel = false
      );

      static void printTemplateResponse(
        const SENTENCE_CODE &responseCode,
        mt::VEC_STR replacementStrings,
        mt::CR_BOL withYesOrNoLabel = false
      );

      static std::string getWelcomeToString();
    };

    struct ageColorSet {
      static void setTitle(mt::CR_STR title);

      static void setNames(
        mt::CR_STR azureName,
        mt::CR_STR blackName,
        mt::CR_STR blueName,
        mt::CR_STR brownName,
        mt::CR_STR canaryName,
        mt::CR_STR chartreuseName,
        mt::CR_STR chocolateName,
        mt::CR_STR crimsonName,
        mt::CR_STR cyanName,
        mt::CR_STR forestgreenName,
        mt::CR_STR goldName,
        mt::CR_STR grayName,
        mt::CR_STR greenName,
        mt::CR_STR lightblueName,
        mt::CR_STR lightgreenName,
        mt::CR_STR limeName,
        mt::CR_STR magentaName,
        mt::CR_STR maroonName,
        mt::CR_STR mintName,
        mt::CR_STR navyblueName,
        mt::CR_STR oliveName,
        mt::CR_STR orangeName,
        mt::CR_STR pinkName,
        mt::CR_STR purpleName,
        mt::CR_STR redName,
        mt::CR_STR royalblueName,
        mt::CR_STR salmonName,
        mt::CR_STR silverName,
        mt::CR_STR skyblueName,
        mt::CR_STR tealName,
        mt::CR_STR violetName,
        mt::CR_STR wengeName,
        mt::CR_STR wheatName,
        mt::CR_STR whiteName,
        mt::CR_STR yellowName
      );

      static std::string getTitle();
      static std::string getName(mt::CR<COLOR_CODE> code);
    };

    struct ageControl {
      static void setTerms(
        mt::CR_STR commandHelpTerm,
        mt::CR_STR controllerListTerm,
        mt::CR_STR childrenEnterTerm,
        mt::CR_STR childrenExecuteTerm,
        mt::CR_STR childrenListTerm,
        mt::CR_STR neighborNextTerm,
        mt::CR_STR neighborPreviousTerm,
        mt::CR_STR switchModifyTerm,
        mt::CR_STR switchSelectTerm,
        mt::CR_STR cleanScreenTerm,
        mt::CR_STR viewInputThisTerm,
        mt::CR_STR viewInputChildrenTerm,
        mt::CR_STR viewOutputThisTerm,
        mt::CR_STR viewOutputChildrenTerm,
        mt::CR_STR resetInputThisTerm,
        mt::CR_STR resetInputDescendantsTerm,
        mt::CR_STR resetOutputThisTerm,
        mt::CR_STR resetOutputDescendantsTerm,
        mt::CR_STR resetDataThisTerm,
        mt::CR_STR resetDataDescendantsTerm,
        mt::CR_STR copyOutputTerm,
        mt::CR_STR pasteInputTerm,
        mt::CR_STR nodeLevelUndoTerm,
        mt::CR_STR nodeLevelRedoTerm,
        mt::CR_STR parentBackTerm,
        mt::CR_STR rootBackTerm,
        mt::CR_STR programQuitTerm
      );

      void setAbbreviationsTitle(mt::CR_STR title);
      void setAbbreviationsDescription(mt::CR_STR description);
      void setBooleanAvailableValuesTitle(mt::CR_STR title);
      void setBooleanAvailableValuesDescription(mt::CR_STR description);

      static std::string getTerm(mt::CR<CONTROL_CODE> code);
      static std::string getAbbreviationsTitle();
      static std::string getAbbreviationsDescription();
      static std::string getBooleanAvailableValuesTitle();
      static std::string getBooleanAvailableValuesDescription();
    };

    struct ageBooleanizer {
      static void setTerms(
        mt::CR_VEC_STR existingTrueTerms,
        mt::CR_VEC_STR existingFalseTerms
      );

      static void setYesOrNoLabel(
        mt::CR_STR existingYesOrNoLabel
      );

      static mt_uti::BOOLEANIZER_CODE test(mt::CR_STR raw);
      static mt::PAIR<mt::VEC_STR> getTerms();
      static std::string getYesOrNoLabel();
    };

    struct ageParameter {
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
        mt::CR_STR listKeyword,
        mt::CR_STR colorSetKeyword
      );

      static void setDescription(
        mt::CR_STR inDescription,
        mt::CR_STR outDescription,
        mt::CR_STR helpDescription,
        mt::CR_STR listDescription,
        mt::CR_STR colorSetDescription
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