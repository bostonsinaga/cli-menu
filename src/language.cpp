#ifndef __CLI_MENU__LANGUAGE_CPP__
#define __CLI_MENU__LANGUAGE_CPP__

#include "language.hpp"

namespace cli_menu {

  //_________|
  // MANAGER |
  //_________|

  bool Langu::ageManager::hasISOCode(mt::CR_STR existingISOCode) {
    return mt::STRUNORMAP_FOUND<mt::ARR_STR<Langu::xMessage::totalSentences>>(
      Langu::xMessage::sentences, existingISOCode
    );
  }

  void Langu::ageManager::selectISOCode(mt::CR_STR existingISOCode) {
    if (Langu::ageManager::hasISOCode(existingISOCode)) {
      Langu::xManager::currentISOCode = existingISOCode;
    }
  }

  void Langu::ageManager::addISOCode(mt::CR_STR newISOCode) {
    Langu::xMessage::sentences[newISOCode] = {};
    Langu::xControl::terms[newISOCode] = {};
    Langu::xControl::abbreviationsTitle[newISOCode] = {};
    Langu::xControl::booleanAvailableValuesTitle[newISOCode] = {};
    Langu::xBooleanizer::object.addTerms(newISOCode, {}, {});
    Langu::xCommand::stringifiedTypes[newISOCode] = {};
    Langu::xProgram::labels[newISOCode] = {};
  }

  void Langu::ageManager::removeISOCode(mt::CR_STR existingISOCode) {
    Langu::xMessage::sentences.erase(existingISOCode);
    Langu::xControl::terms.erase(existingISOCode);
    Langu::xControl::abbreviationsTitle.erase(existingISOCode);
    Langu::xControl::booleanAvailableValuesTitle.erase(existingISOCode);
    Langu::xBooleanizer::object.removeTerms(existingISOCode);
    Langu::xCommand::stringifiedTypes.erase(existingISOCode);
    Langu::xProgram::labels.erase(existingISOCode);
  }

  //_________|
  // MESSAGE |
  //_________|

  mt::STRUNORMAP<mt::ARR_STR<Langu::xMessage::totalSentences>> Langu::xMessage::sentences = {{"en", {    
    // SENTENCE_ARGUMENT_REQUIRED
    "Prohibited without explicit arguments.",
    // SENTENCE_ARGUMENT_RESET_THIS
    "This argument is removed.",
    // SENTENCE_ARGUMENT_RESET_ALL
    "All arguments are removed.",
    // SENTENCE_ARGUMENT_VIEW_EMPTY_THIS
    "This argument is empty.",
    // SENTENCE_BOOLEAN_INSTANT_QUESTION_FORBIDDEN_CONTROLLER
    "Forbidden controller on instant boolean question.",
    // SENTENCE_CLIPBOARD_COPY_FAILURE
    "Operation result is empty.",
    // SENTENCE_CLIPBOARD_COPY_SUCCEED
    "Copied to clipboard.",
    // SENTENCE_CLIPBOARD_GET_FAILURE,
    "Failed to get clipboard data.",
    // SENTENCE_CLIPBOARD_LOCK_FAILURE
    "Failed to lock clipboard data.",
    // SENTENCE_CLIPBOARD_MEMORY_ALLOCATION_FAILURE
    "Failed to allocate memory for clipboard operation.",
    // SENTENCE_CLIPBOARD_OPEN_FAILURE,
    "Failed to open clipboard.",
    // SENTENCE_CLIPBOARD_PASTE_SUCCEED
    "Pasted from clipboard.",
    // SENTENCE_EMPTY_OUTPUT
    "Output inside '$' is empty.",
    // SENTENCE_FILE_OVERWRITE_QUESTION
    "Are you sure you want to overwrite '$'?",
    // SENTENCE_FILE_WRITE_FAILURE
    "Cannot write file because the path or filename is invalid, permission is not granted, or the disk is full.",
    // SENTENCE_FILE_WRITE_SUCCEED
    "Output is written to '$'.",
    // SENTENCE_FORBIDDEN_HIDDEN_PASTE
    "Hidden text pasting is only available on insertion.",
    // SENTENCE_INTERRUPTION_DIALOG
    "Prohibited until the remaining direct inputs are processed.",
    // SENTENCE_KEYWORD_NOT_FOUND
    "'$' not found.",
    // SENTENCE_MODE_ALREADY_EDITING
    "Already in editing mode.",
    // SENTENCE_MODE_ALREADY_SELECTING
    "Already in selection mode.",
    // SENTENCE_MODE_SWITCH_TO_EDITING
    "Switch to editing mode.",
    // SENTENCE_MODE_SWITCH_TO_SELECTION
    "Switch to selection mode.",
    // SENTENCE_PARAMETER_ALONE
    "This node has no neighbors.",
    // SENTENCE_PARAMETER_AT_LEAF
    "This node does not contain any children.",
    // SENTENCE_PARAMETER_AT_ROOT
    "No more groups above.",
    // SENTENCE_PARAMETER_NOT_FOUND
    "Parameter not found.",
    // SENTENCE_PARAMETER_REQUIRED
    "'$' needs explicit arguments.",
    // SENTENCE_PARENT_STRICT
    "Parent needs explicit arguments for all its required children.",
    // SENTENCE_PROGRAM_TERMINATED
    "TERMINATED",
    // SENTENCE_PROGRAM_FAILED
    "FAILED",
    // SENTENCE_PROGRAM_CANCELED
    "CANCELED",
    // SENTENCE_PROGRAM_SUCCEEDED
    "SUCCEEDED"
  }}};

  CONSOLE_CODE Langu::xMessage::consoleCodes[Langu::xMessage::totalSentences] = {
    // SENTENCE_ARGUMENT_REQUIRED
    CONSOLE_ERROR,
    // SENTENCE_ARGUMENT_RESET_THIS
    CONSOLE_HINT_1,
    // SENTENCE_ARGUMENT_RESET_ALL
    CONSOLE_HINT_1,
    // SENTENCE_ARGUMENT_VIEW_EMPTY_THIS
    CONSOLE_WARNING,
    // SENTENCE_BOOLEAN_INSTANT_QUESTION_FORBIDDEN_CONTROLLER
    CONSOLE_ERROR,
    // SENTENCE_CLIPBOARD_COPY_FAILURE
    CONSOLE_WARNING,
    // SENTENCE_CLIPBOARD_COPY_SUCCEED
    CONSOLE_HINT_1,
    // SENTENCE_CLIPBOARD_GET_FAILURE,
    CONSOLE_ERROR,
    // SENTENCE_CLIPBOARD_LOCK_FAILURE
    CONSOLE_ERROR,
    // SENTENCE_CLIPBOARD_MEMORY_ALLOCATION_FAILURE
    CONSOLE_ERROR,
    // SENTENCE_CLIPBOARD_OPEN_FAILURE,
    CONSOLE_ERROR,
    // SENTENCE_CLIPBOARD_PASTE_SUCCEED
    CONSOLE_HINT_1,
    // SENTENCE_EMPTY_OUTPUT
    CONSOLE_WARNING,
    // SENTENCE_FILE_OVERWRITE_QUESTION
    CONSOLE_WARNING,
    // SENTENCE_FILE_WRITE_FAILURE
    CONSOLE_ERROR,
    // SENTENCE_FILE_WRITE_SUCCEED
    CONSOLE_HINT_1,
    // SENTENCE_FORBIDDEN_HIDDEN_PASTE
    CONSOLE_WARNING,
    // SENTENCE_INTERRUPTION_DIALOG
    CONSOLE_ERROR,
    // SENTENCE_KEYWORD_NOT_FOUND
    CONSOLE_ERROR,
    // SENTENCE_MODE_ALREADY_EDITING
    CONSOLE_WARNING,
    // SENTENCE_MODE_ALREADY_SELECTING
    CONSOLE_WARNING,
    // SENTENCE_MODE_SWITCH_TO_EDITING
    CONSOLE_HINT_1,
    // SENTENCE_MODE_SWITCH_TO_SELECTION
    CONSOLE_HINT_1,
    // SENTENCE_PARAMETER_ALONE
    CONSOLE_WARNING,
    // SENTENCE_PARAMETER_AT_LEAF
    CONSOLE_WARNING,
    // SENTENCE_PARAMETER_AT_ROOT
    CONSOLE_WARNING,
    // SENTENCE_PARAMETER_NOT_FOUND
    CONSOLE_ERROR,
    // SENTENCE_PARAMETER_REQUIRED
    CONSOLE_ERROR,
    // SENTENCE_PARENT_STRICT
    CONSOLE_ERROR,
    // SENTENCE_PROGRAM_TERMINATED
    CONSOLE_CANCEL,
    // SENTENCE_PROGRAM_FAILED
    CONSOLE_ERROR,
    // SENTENCE_PROGRAM_CANCELED
    CONSOLE_CANCEL,
    // SENTENCE_PROGRAM_SUCCEEDED
    CONSOLE_CORRECT
  };

  void Langu::ageMessage::setSentences(
    mt::CR_STR argumentRequiredSentence,
    mt::CR_STR argumentResetThisSentence,
    mt::CR_STR argumentResetAllSentence,
    mt::CR_STR argumentViewEmptyThis,
    mt::CR_STR booleanInstantQuestionForbiddenControllerSentence,
    mt::CR_STR clipboardCopyFailureSentence,
    mt::CR_STR clipboardCopySucceedSentence,
    mt::CR_STR clipboardGetFailureSentence,
    mt::CR_STR clipboardLockFailureSentence,
    mt::CR_STR clipboardMemoryAllocationFailureSentence,
    mt::CR_STR clipboardOpenFailureSentence,
    mt::CR_STR clipboardPasteSucceedSentence,
    mt::CR_STR emptyOutputSentence,
    mt::CR_STR fileOverwriteQuestionSentence,
    mt::CR_STR fileWriteFailureSentence,
    mt::CR_STR fileWriteSucceedSentence,
    mt::CR_STR forbiddenHiddenPasteSentence,
    mt::CR_STR interruptionDialogSentence,
    mt::CR_STR keywordNotFoundSentence,
    mt::CR_STR modeAlreadyEditingSentence,
    mt::CR_STR modeAlreadySelectingSentence,
    mt::CR_STR modeSwitchToEditingSentence,
    mt::CR_STR modeSwitchToSelectionSentence,
    mt::CR_STR parameterAloneSentence,
    mt::CR_STR parameterAtLeafSentence,
    mt::CR_STR parameterAtRootSentence,
    mt::CR_STR parameterNotFoundSentence,
    mt::CR_STR parameterRequiredSentence,
    mt::CR_STR parentStrictSentence,
    mt::CR_STR programTerminatedSentence,
    mt::CR_STR programFailedSentence,
    mt::CR_STR programCanceledSentence,
    mt::CR_STR programSucceededSentence
  ) {
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_ARGUMENT_RESET_THIS] = argumentResetThisSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_ARGUMENT_RESET_ALL] = argumentResetAllSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_ARGUMENT_REQUIRED] = argumentRequiredSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_ARGUMENT_VIEW_EMPTY_THIS] = argumentViewEmptyThis;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_BOOLEAN_INSTANT_QUESTION_FORBIDDEN_CONTROLLER] = booleanInstantQuestionForbiddenControllerSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_CLIPBOARD_COPY_FAILURE] = clipboardCopyFailureSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_CLIPBOARD_COPY_SUCCEED] = clipboardCopySucceedSentence;    
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_CLIPBOARD_GET_FAILURE] = clipboardGetFailureSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_CLIPBOARD_LOCK_FAILURE] = clipboardLockFailureSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_CLIPBOARD_MEMORY_ALLOCATION_FAILURE] = clipboardMemoryAllocationFailureSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_CLIPBOARD_OPEN_FAILURE] = clipboardOpenFailureSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_CLIPBOARD_PASTE_SUCCEED] = clipboardPasteSucceedSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_EMPTY_OUTPUT] = emptyOutputSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_FILE_OVERWRITE_QUESTION] = fileOverwriteQuestionSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_FILE_WRITE_FAILURE] = fileWriteFailureSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_FILE_WRITE_SUCCEED] = fileWriteSucceedSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_FORBIDDEN_HIDDEN_PASTE] = forbiddenHiddenPasteSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_INTERRUPTION_DIALOG] = interruptionDialogSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_KEYWORD_NOT_FOUND] = keywordNotFoundSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_MODE_ALREADY_EDITING] = modeAlreadyEditingSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_MODE_ALREADY_SELECTING] = modeAlreadySelectingSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_MODE_SWITCH_TO_EDITING] = modeSwitchToEditingSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_MODE_SWITCH_TO_SELECTION] = modeSwitchToSelectionSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_PARAMETER_ALONE] = parameterAloneSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_PARAMETER_AT_LEAF] = parameterAtLeafSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_PARAMETER_AT_ROOT] = parameterAtRootSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_PARAMETER_NOT_FOUND] = parameterNotFoundSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_PARAMETER_REQUIRED] = parameterRequiredSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_PARENT_STRICT] = parentStrictSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_PROGRAM_TERMINATED] = programTerminatedSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_PROGRAM_FAILED] = programFailedSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_PROGRAM_CANCELED] = programCanceledSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_PROGRAM_SUCCEEDED] = programSucceededSentence;
  }

  void Langu::ageMessage::printResponse(
    const SENTENCE_CODE &responseCode,
    mt::CR_BOL withYesOrNoLabel
  ) {
    Console::logResponse(
      Langu::xMessage::consoleCodes[responseCode],
      Langu::xMessage::sentences[Langu::xManager::currentISOCode][responseCode]
      + (withYesOrNoLabel ? " " + Langu::ageBooleanizer::getYesOrNoLabel() : "")
    );
  }

  void Langu::ageMessage::printTemplateResponse(
    const SENTENCE_CODE &responseCode,
    mt::CR_STR replacementText,
    mt::CR_BOL withYesOrNoLabel
  ) {
    std::string templateString = Langu::xMessage::sentences
      [Langu::xManager::currentISOCode][responseCode];

    size_t foundIndex = templateString.find(xManager::placeholder);

    // insert 'replacementText' into placeholder
    if (foundIndex != std::string::npos) {
      templateString = templateString.substr(0, foundIndex) + replacementText
        + templateString.substr(foundIndex + xManager::placeholder.length());
    }

    Console::logResponse(
      Langu::xMessage::consoleCodes[responseCode],
      templateString + (withYesOrNoLabel ? " " + Langu::ageBooleanizer::getYesOrNoLabel() : "")
    );
  }

  mt::STRUNORMAP_STR Langu::xMessage::welcomeToString = {{"en", "Welcome to "}};

  std::string Langu::ageMessage::getWelcomeToString() {
    return Langu::xMessage::welcomeToString[Langu::xManager::currentISOCode];
  }

  //_________|
  // CONTROL |
  //_________|

  mt::STRUNORMAP<mt::ARR_STR<Langu::xControl::totalSymbols>> Langu::xControl::terms = {{"en", {
    "help", "list", "enter", "back", "next", "previous", "modify", "select",
    "reset this", "view this", "reset all", "view all", "copy", "paste", "quit"
  }}};

  mt::STRUNORMAP_STR
    Langu::xControl::abbreviationsTitle = {{"en", "Controller List"}},
    Langu::xControl::booleanAvailableValuesTitle = {{"en", "Boolean Available Values"}};

  void Langu::xControl::limitTerm(
    const CONTROL_CODE &code,
    std::string &newTerm
  ) {
    if (newTerm.length() > Langu::xControl::maxTermLength) {
      newTerm = newTerm.substr(0, Langu::xControl::maxTermLength - 2);
      newTerm += "..";
    }

    Langu::xControl::terms[Langu::xManager::currentISOCode][code] = newTerm;
  }

  void Langu::ageControl::setTerms(
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
  ) {
    Langu::xControl::limitTerm(CONTROL_HELP, helpTerm);
    Langu::xControl::limitTerm(CONTROL_LIST, listTerm);
    Langu::xControl::limitTerm(CONTROL_ENTER, enterTerm);
    Langu::xControl::limitTerm(CONTROL_BACK, backTerm);
    Langu::xControl::limitTerm(CONTROL_NEXT, nextTerm);
    Langu::xControl::limitTerm(CONTROL_PREVIOUS, previousTerm);
    Langu::xControl::limitTerm(CONTROL_MODIFY, modifyTerm);
    Langu::xControl::limitTerm(CONTROL_SELECT, selectTerm);
    Langu::xControl::limitTerm(CONTROL_RESET_THIS, resetThisTerm);
    Langu::xControl::limitTerm(CONTROL_VIEW_THIS, viewThisTerm);
    Langu::xControl::limitTerm(CONTROL_RESET_ALL, resetAllTerm);
    Langu::xControl::limitTerm(CONTROL_VIEW_ALL, viewAllTerm);
    Langu::xControl::limitTerm(CONTROL_COPY, copyTerm);
    Langu::xControl::limitTerm(CONTROL_PASTE, pasteTerm);
    Langu::xControl::limitTerm(CONTROL_QUIT, quitTerm);
  }

  void Langu::ageControl::setAbbreviationsTitle(mt::CR_STR title) {
    Langu::xControl::abbreviationsTitle[Langu::xManager::currentISOCode] = title;
  }

  void Langu::ageControl::setBooleanAvailableValuesTitle(mt::CR_STR title) {
    Langu::xControl::booleanAvailableValuesTitle[Langu::xManager::currentISOCode] = title;
  }

  std::string Langu::ageControl::getTerm(const CONTROL_CODE &code) {
    return Langu::xControl::terms[Langu::xManager::currentISOCode][code];
  }

  std::string Langu::ageControl::getAbbreviationsTitle() {
    return Langu::xControl::abbreviationsTitle[Langu::xManager::currentISOCode];
  }

  std::string Langu::ageControl::getBooleanAvailableValuesTitle() {
    return Langu::xControl::booleanAvailableValuesTitle[Langu::xManager::currentISOCode];
  }

  //_____________|
  // BOOLEANIZER |
  //_____________|

  mt::STRUNORMAP_STR Langu::xBooleanizer::yesOrNoLabel = {{"en", "Y/n"}};

  void Langu::ageBooleanizer::setTerms(
    mt::CR_VEC_STR existingTrueTerms,
    mt::CR_VEC_STR existingFalseTerms
  ) {
    Langu::xBooleanizer::object.changeTerms(
      Langu::xManager::currentISOCode,
      existingTrueTerms,
      existingFalseTerms
    );
  }

  void Langu::ageBooleanizer::setYesOrNoLabel(
    mt::CR_STR existingYesOrNoLabel
  ) {
    Langu::xBooleanizer::yesOrNoLabel[
      Langu::xManager::currentISOCode
    ] = existingYesOrNoLabel;
  }

  bool Langu::ageBooleanizer::test(mt::CR_STR raw) {
    return Langu::xBooleanizer::object.test(
      Langu::xManager::currentISOCode, raw
    );
  }

  mt::PAIR<mt::VEC_STR> Langu::ageBooleanizer::getTerms() {
    return std::make_pair(
      Langu::xBooleanizer::object.getTrueTerms(Langu::xManager::currentISOCode),
      Langu::xBooleanizer::object.getFalseTerms(Langu::xManager::currentISOCode)
    );
  }

  std::string Langu::ageBooleanizer::getYesOrNoLabel() {
    return Langu::xBooleanizer::yesOrNoLabel[Langu::xManager::currentISOCode];
  }

  //_________|
  // COMMAND |
  //_________|

  mt::STRUNORMAP<mt::ARR_STR<Langu::xCommand::totalTypes>>
  Langu::xCommand::stringifiedTypes = {{"en", {
    "WORD", "NUMBER", "BOOLEAN"
  }}};

  void Langu::ageCommand::setStringifiedTypes(
    mt::CR_STR wordStringifiedType,
    mt::CR_STR numberStringifiedType,
    mt::CR_STR booleanStringifiedType
  ) {
    Langu::xCommand::stringifiedTypes
    [Langu::xManager::currentISOCode]
    [STRINGIFIED_TYPE_WORD] = wordStringifiedType;

    Langu::xCommand::stringifiedTypes
    [Langu::xManager::currentISOCode]
    [STRINGIFIED_TYPE_NUMBER] = numberStringifiedType;

    Langu::xCommand::stringifiedTypes
    [Langu::xManager::currentISOCode]
    [STRINGIFIED_TYPE_BOOLEAN] = booleanStringifiedType;
  }

  std::string Langu::ageCommand::getStringifiedType(
    const STRINGIFIED_TYPE_COMMAND_CODE &code
  ) {
    return Langu::xCommand::stringifiedTypes
    [Langu::xManager::currentISOCode][code];
  }

  //_________|
  // PROGRAM |
  //_________|

  mt::STRUNORMAP<mt::ARR_STR<Langu::xProgram::totalLabels>>
  Langu::xProgram::labels = {{"en", {
    "Version", "Author", "Link"
  }}};

  void Langu::ageProgram::setLabels(
    mt::CR_STR versionLabel,
    mt::CR_STR authorLabel,
    mt::CR_STR linkLabel
  ) {
    Langu::xProgram::labels
    [Langu::xManager::currentISOCode]
    [PROGRAM_LABEL_VERSION] = versionLabel;

    Langu::xProgram::labels
    [Langu::xManager::currentISOCode]
    [PROGRAM_LABEL_AUTHOR] = authorLabel;

    Langu::xProgram::labels
    [Langu::xManager::currentISOCode]
    [PROGRAM_LABEL_LINK] = linkLabel;
  }

  std::string Langu::ageProgram::getLabel(
    const PROGRAM_LABEL_CODE &code
  ) {
    return Langu::xProgram::labels
    [Langu::xManager::currentISOCode][code];
  }

  //________|
  // PRESET |
  //________|

  mt::STRUNORMAP<mt::ARR_STR<Langu::xPreset::totalKeywords>>
  Langu::xPreset::keywords = {{"en", {
    "in", "out", "help", "list"
  }}},
  Langu::xPreset::descriptions = {{"en", {
    "Enter input filename",
    "Enter output filename",
    "", ""
  }}};

  std::string Langu::agePreset::fileOutDefaultExtension = ".txt";
  mt::PAIR_STR Langu::agePreset::fileOutBracketsForNumbering = {"(", ")"};

  void Langu::agePreset::setKeywords(
    mt::CR_STR inKeyword,
    mt::CR_STR outKeyword,
    mt::CR_STR helpKeyword,
    mt::CR_STR listKeyword
  ) {
    Langu::xPreset::keywords
    [Langu::xManager::currentISOCode]
    [PRESET_IN] = inKeyword;

    Langu::xPreset::keywords
    [Langu::xManager::currentISOCode]
    [PRESET_OUT] = outKeyword;

    Langu::xPreset::keywords
    [Langu::xManager::currentISOCode]
    [PRESET_HELP] = helpKeyword;

    Langu::xPreset::keywords
    [Langu::xManager::currentISOCode]
    [PRESET_LIST] = listKeyword;
  }

  void Langu::agePreset::setDescription(
    mt::CR_STR inDescription,
    mt::CR_STR outDescription,
    mt::CR_STR helpDescription,
    mt::CR_STR listDescription
  ) {
    Langu::xPreset::descriptions
    [Langu::xManager::currentISOCode]
    [PRESET_IN] = inDescription;

    Langu::xPreset::descriptions
    [Langu::xManager::currentISOCode]
    [PRESET_OUT] = outDescription;

    Langu::xPreset::descriptions
    [Langu::xManager::currentISOCode]
    [PRESET_HELP] = helpDescription;

    Langu::xPreset::descriptions
    [Langu::xManager::currentISOCode]
    [PRESET_LIST] = listDescription;
  }

  std::string Langu::agePreset::getKeyword(
    const PRESET_CODE &code
  ) {
    return Langu::xPreset::keywords
    [Langu::xManager::currentISOCode][code];
  }

  std::string Langu::agePreset::getDescription(
    const PRESET_CODE &code
  ) {
    return Langu::xPreset::descriptions
    [Langu::xManager::currentISOCode][code];
  }
}

#endif // __CLI_MENU__LANGUAGE_CPP__