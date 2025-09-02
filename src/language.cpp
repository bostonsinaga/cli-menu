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
    Langu::xControl::toggleAvailableValuesTitle[newISOCode] = {};
    Langu::xBooleanizer::object.addTerms(newISOCode, {}, {});
    Langu::xCommand::stringifiedTypes[newISOCode] = {};
    Langu::xProgram::labels[newISOCode] = {};
  }

  void Langu::ageManager::removeISOCode(mt::CR_STR existingISOCode) {
    Langu::xMessage::sentences.erase(existingISOCode);
    Langu::xControl::terms.erase(existingISOCode);
    Langu::xControl::abbreviationsTitle.erase(existingISOCode);
    Langu::xControl::toggleAvailableValuesTitle.erase(existingISOCode);
    Langu::xBooleanizer::object.removeTerms(existingISOCode);
    Langu::xCommand::stringifiedTypes.erase(existingISOCode);
    Langu::xProgram::labels.erase(existingISOCode);
  }

  //_________|
  // MESSAGE |
  //_________|

  mt::STRUNORMAP<mt::ARR_STR<Langu::xMessage::totalSentences>> Langu::xMessage::sentences = {{"en", {
    // LANGUAGE_ALREADY_MODIFYING
    "Already in edit mode.",
    // LANGUAGE_ALREADY_SELECTING
    "Already in selection mode.",
    // LANGUAGE_ARGUMENT_REQUIRED
    "Prohibited without explicit arguments.",
    // LANGUAGE_CLIPBOARD_OPEN_FAILURE,
    "Failed to open clipboard.",
    // LANGUAGE_CLIPBOARD_GET_FAILURE,
    "Failed to get clipboard data.",
    // LANGUAGE_CLIPBOARD_LOCK_FAILURE
    "Failed to lock clipboard data.",
    // LANGUAGE_CLIPBOARD_PASTED
    "Pasted from clipboard.",
    // LANGUAGE_FORBIDDEN_HIDDEN_PASTE
    "Hidden text pasting is only available on insertion.",
    // LANGUAGE_INTERRUPTION_DIALOG
    "Prohibited until the remaining direct inputs are processed.",
    // LANGUAGE_PARAMETER_ALONE
    "This node has no neighbors.",
    // LANGUAGE_PARAMETER_AT_LEAF
    "This node does not contain any children.",
    // LANGUAGE_PARAMETER_AT_ROOT
    "No more groups above.",
    // LANGUAGE_PARAMETER_NOT_FOUND
    "Parameter not found.",
    // LANGUAGE_PARENT_STRICT
    "Parent needs explicit arguments for all its required children.",
    // LANGUAGE_PROGRAM_CANCELED
    "Program canceled.",
    // LANGUAGE_PROGRAM_FAILED
    "Program failed.",
    // LANGUAGE_PROGRAM_SUCCEEDED
    "Program succeeded."
  }}};

  CONSOLE_CODE Langu::xMessage::consoleCodes[Langu::xMessage::totalSentences] = {
    // LANGUAGE_ALREADY_MODIFYING
    CONSOLE_WARNING,
    // LANGUAGE_ALREADY_SELECTING
    CONSOLE_WARNING,
    // LANGUAGE_ARGUMENT_REQUIRED
    CONSOLE_ERROR,
    // LANGUAGE_CLIPBOARD_OPEN_FAILURE,
    CONSOLE_ERROR,
    // LANGUAGE_CLIPBOARD_GET_FAILURE,
    CONSOLE_ERROR,
    // LANGUAGE_CLIPBOARD_LOCK_FAILURE
    CONSOLE_ERROR,
    // LANGUAGE_CLIPBOARD_PASTED
    CONSOLE_HINT_1,
    // LANGUAGE_FORBIDDEN_HIDDEN_PASTE
    CONSOLE_WARNING,
    // LANGUAGE_INTERRUPTION_DIALOG
    CONSOLE_ERROR,
    // LANGUAGE_PARAMETER_ALONE
    CONSOLE_WARNING,
    // LANGUAGE_PARAMETER_AT_LEAF
    CONSOLE_WARNING,
    // LANGUAGE_PARAMETER_AT_ROOT
    CONSOLE_WARNING,
    // LANGUAGE_PARAMETER_NOT_FOUND
    CONSOLE_ERROR,
    // LANGUAGE_PARENT_STRICT
    CONSOLE_ERROR,
    // LANGUAGE_PROGRAM_CANCELED
    CONSOLE_CANCEL,
    // LANGUAGE_PROGRAM_FAILED
    CONSOLE_ERROR,
    // LANGUAGE_PROGRAM_SUCCEEDED
    CONSOLE_CORRECT
  };

  void Langu::ageMessage::setSentences(
    mt::CR_STR alreadyModifyingSentence,
    mt::CR_STR alreadySelectingSentence,
    mt::CR_STR argumentRequiredSentence,
    mt::CR_STR clipboardOpenFailureSentence,
    mt::CR_STR clipboardGetFailureSentence,
    mt::CR_STR clipboardLockFailureSentence,
    mt::CR_STR clipboardPastedSentence,
    mt::CR_STR forbiddenHiddenPasteSentence,
    mt::CR_STR interruptionDialogSentence,
    mt::CR_STR parameterAloneSentence,
    mt::CR_STR parameterAtLeafSentence,
    mt::CR_STR parameterAtRootSentence,
    mt::CR_STR parameterNotFoundSentence,
    mt::CR_STR parentStrictSentence,
    mt::CR_STR programCanceledSentence,
    mt::CR_STR programFailedSentence,
    mt::CR_STR programSucceededSentence
  ) {
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][LANGUAGE_ALREADY_MODIFYING] = alreadyModifyingSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][LANGUAGE_ALREADY_SELECTING] = alreadySelectingSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][LANGUAGE_ARGUMENT_REQUIRED] = argumentRequiredSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][LANGUAGE_CLIPBOARD_OPEN_FAILURE] = clipboardOpenFailureSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][LANGUAGE_CLIPBOARD_GET_FAILURE] = clipboardGetFailureSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][LANGUAGE_CLIPBOARD_LOCK_FAILURE] = clipboardLockFailureSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][LANGUAGE_CLIPBOARD_PASTED] = clipboardPastedSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][LANGUAGE_FORBIDDEN_HIDDEN_PASTE] = forbiddenHiddenPasteSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][LANGUAGE_INTERRUPTION_DIALOG] = interruptionDialogSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][LANGUAGE_PARAMETER_ALONE] = parameterAloneSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][LANGUAGE_PARAMETER_AT_LEAF] = parameterAtLeafSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][LANGUAGE_PARAMETER_AT_ROOT] = parameterAtRootSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][LANGUAGE_PARAMETER_NOT_FOUND] = parameterNotFoundSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][LANGUAGE_PARENT_STRICT] = parentStrictSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][LANGUAGE_PROGRAM_CANCELED] = programCanceledSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][LANGUAGE_PROGRAM_FAILED] = programFailedSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][LANGUAGE_PROGRAM_SUCCEEDED] = programSucceededSentence;
  }

  void Langu::ageMessage::printResponse(const LANGUAGE_CODE &responseCode) {
    Console::logResponse(
      Langu::xMessage::consoleCodes[responseCode],
      Langu::xMessage::sentences[Langu::xManager::currentISOCode][responseCode]
    );
  }

  //_________|
  // CONTROL |
  //_________|

  mt::STRUNORMAP<mt::ARR_STR<Langu::xControl::totalSymbols>> Langu::xControl::terms = {{"en", {
    "help", "list", "enter", "back", "next", "previous", "modify",
    "select", "reset", "view", "copy", "paste", "quit"
  }}};

  mt::STRUNORMAP_STR
    Langu::xControl::abbreviationsTitle = {{"en", "Controller List"}},
    Langu::xControl::toggleAvailableValuesTitle = {{"en", "Boolean Available Values"}};

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
    std::string resetTerm,
    std::string viewTerm,
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
    Langu::xControl::limitTerm(CONTROL_RESET, resetTerm);
    Langu::xControl::limitTerm(CONTROL_VIEW, viewTerm);
    Langu::xControl::limitTerm(CONTROL_COPY, copyTerm);
    Langu::xControl::limitTerm(CONTROL_PASTE, pasteTerm);
    Langu::xControl::limitTerm(CONTROL_QUIT, quitTerm);
  }

  void Langu::ageControl::setAbbreviationsTitle(mt::CR_STR title) {
    Langu::xControl::abbreviationsTitle[Langu::xManager::currentISOCode] = title;
  }

  void Langu::ageControl::setToggleAvailableValuesTitle(mt::CR_STR title) {
    Langu::xControl::toggleAvailableValuesTitle[Langu::xManager::currentISOCode] = title;
  }

  std::string Langu::ageControl::getTerm(const CONTROL_CODE &code) {
    return Langu::xControl::terms[Langu::xManager::currentISOCode][code];
  }

  std::string Langu::ageControl::getAbbreviationsTitle() {
    return Langu::xControl::abbreviationsTitle[Langu::xManager::currentISOCode];
  }

  std::string Langu::ageControl::getToggleAvailableValuesTitle() {
    return Langu::xControl::toggleAvailableValuesTitle[Langu::xManager::currentISOCode];
  }

  //_____________|
  // BOOLEANIZER |
  //_____________|

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

  //_________|
  // COMMAND |
  //_________|

  mt::STRUNORMAP<mt::ARR_STR<Langu::xCommand::totalTypes>>
  Langu::xCommand::stringifiedTypes = {{"en", {
    "WORD", "NUMBER", "TOGGLE"
  }}};

  void Langu::ageCommand::setStringifiedTypes(
    mt::CR_STR wordStringifiedType,
    mt::CR_STR numberStringifiedType,
    mt::CR_STR toggleStringifiedType
  ) {
    Langu::xCommand::stringifiedTypes
    [Langu::xManager::currentISOCode]
    [STRINGIFIED_TYPE_WORD] = wordStringifiedType;

    Langu::xCommand::stringifiedTypes
    [Langu::xManager::currentISOCode]
    [STRINGIFIED_TYPE_NUMBER] = numberStringifiedType;

    Langu::xCommand::stringifiedTypes
    [Langu::xManager::currentISOCode]
    [STRINGIFIED_TYPE_TOGGLE] = toggleStringifiedType;
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