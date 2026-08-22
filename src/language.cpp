#ifndef __CLI_MENU__LANGUAGE_CPP__
#define __CLI_MENU__LANGUAGE_CPP__

#include "language.hpp"

namespace cli_menu {

  //_________|
  // MANAGER |
  //_________|

  bool Langu::ageManager::hasISOCode(mt::CR_STR existingISOCode) {
    return Langu::xMessage::sentences.find(existingISOCode) != Langu::xMessage::sentences.end();
  }

  void Langu::ageManager::selectISOCode(mt::CR_STR existingISOCode) {
    if (Langu::ageManager::hasISOCode(existingISOCode)) {
      Langu::xManager::currentISOCode = existingISOCode;
    }
  }

  void Langu::ageManager::addISOCode(mt::CR_STR newISOCode) {
    Langu::xMessage::sentences[newISOCode] = {};
    Langu::xColorSet::title[newISOCode] = {};
    Langu::xColorSet::names[newISOCode] = {};
    Langu::xControl::terms[newISOCode] = {};
    Langu::xControl::abbreviationsTitle[newISOCode] = {};
    Langu::xControl::abbreviationsDescription[newISOCode] = {};
    Langu::xControl::booleanAvailableValuesTitle[newISOCode] = {};
    Langu::xControl::booleanAvailableValuesDescription[newISOCode] = {};
    Langu::xBooleanizer::object.addTerms(newISOCode, {}, {});
    Langu::xParameter::stringifiedTypes[newISOCode] = {};
    Langu::xProgram::labels[newISOCode] = {};
  }

  void Langu::ageManager::removeISOCode(mt::CR_STR existingISOCode) {
    Langu::xMessage::sentences.erase(existingISOCode);
    Langu::xColorSet::title.erase(existingISOCode);
    Langu::xColorSet::names.erase(existingISOCode);
    Langu::xControl::terms.erase(existingISOCode);
    Langu::xControl::abbreviationsTitle.erase(existingISOCode);
    Langu::xControl::abbreviationsDescription.erase(existingISOCode);
    Langu::xControl::booleanAvailableValuesTitle.erase(existingISOCode);
    Langu::xControl::booleanAvailableValuesDescription.erase(existingISOCode);
    Langu::xBooleanizer::object.removeTerms(existingISOCode);
    Langu::xParameter::stringifiedTypes.erase(existingISOCode);
    Langu::xProgram::labels.erase(existingISOCode);
  }

  std::string Langu::ageManager::replaceTemplateString(
    mt::CR_STR templateString,
    mt::VEC_STR replacementStrings
  ) {
    size_t startPosition = std::string::npos;
    mt::VEC_SZ foundIndexes;

    do { // find all occurrence indexes of the placeholder
      foundIndexes.push_back(
        templateString.find(Langu::xManager::placeholder, startPosition + 1)
      );

      // next slice initial index
      startPosition = foundIndexes.back();

    } while (startPosition != std::string::npos);

    // the last must be 'std::npos'
    foundIndexes.pop_back();

    // erase excessive 'foundIndexes' over 'replacementStrings'
    if (foundIndexes.size() > replacementStrings.size()) {
      mt_uti::VecTool<size_t>::eraseIntervalStable(
        foundIndexes, { replacementStrings.size(), foundIndexes.size() - 1 }
      );
    }

    // copy for return
    std::string replacedString;
    startPosition = 0;

    for (int i = 0; i < foundIndexes.size(); i++) {

      // insert each replacement into each placeholder
      replacedString += templateString.substr(
        startPosition, foundIndexes[i] - startPosition
      ) + replacementStrings[i];

      // next slice initial index
      startPosition = foundIndexes[i] + Langu::xManager::placeholder.length();
    }

    // add the rest
    return replacedString + templateString.substr(startPosition);
  }

  //_________|
  // MESSAGE |
  //_________|

  mt::UNORMAP_STR<mt::ARR_STR<SENTENCE_TOTAL>> Langu::xMessage::sentences = {{ Langu::xManager::defaultISOCode, {
    // SENTENCE_ARGUMENT_ADDED
    "$ arguments added to '$'",
    // SENTENCE_ARGUMENT_REQUIRED
    "prohibited without explicit arguments",
    // SENTENCE_CLIPBOARD_OPEN_FAILURE
    "failed to open clipboard",
    // SENTENCE_CLIPBOARD_GLOBAL_LOCK_FAILURE
    "failed to global lock clipboard",
    // SENTENCE_CLIPBOARD_GLOBAL_ALLOC_FAILURE
    "failed to global allocate clipboard",
    // SENTENCE_CLIPBOARD_GET_DATA_FAILURE
    "failed to get clipboard data",
    // SENTENCE_CLIPBOARD_COPY_SUCCEED
    "copied to clipboard",
    // SENTENCE_CLIPBOARD_PASTE_SUCCEED
    "pasted from clipboard",
    // SENTENCE_COMMAND_UNREDO_NO_NEW_VISITS
    "no new nodes have been visited yet",
    // SENTENCE_EMPTY_INPUT_THIS
    "this input is empty",
    // SENTENCE_EMPTY_OUTPUT_THIS
    "this output is empty",
    // SENTENCE_EMPTY_OUTPUT_NAMED
    "output inside '$' is empty",
    // SENTENCE_FILE_OVERWRITE_QUESTION
    "are you sure you want to overwrite '$'?",
    // SENTENCE_FILE_WRITE_FAILURE
    "cannot write output to '$'",
    // SENTENCE_FILE_WRITE_SUCCEED
    "output is written to '$'",
    // SENTENCE_FORBIDDEN_CONTROLLER
    "forbidden controller '$'",
    // SENTENCE_FORBIDDEN_HIDDEN_PASTE
    "hidden text pasting is only available on insertion",
    // SENTENCE_INTERRUPTION_DIALOG
    "prohibited until the remaining direct inputs are processed",
    // SENTENCE_KEYWORD_NOT_FOUND
    "keyword '$' is not found",
    // SENTENCE_MODE_ALREADY_MODIFYING
    "already in edit mode",
    // SENTENCE_MODE_ALREADY_SELECTING
    "already in select mode",
    // SENTENCE_MODE_SWITCH_TO_MODIFICATION
    "switch to edit mode",
    // SENTENCE_MODE_SWITCH_TO_SELECTION
    "switch to select mode",
    // SENTENCE_PARAMETER_ALONE
    "this node has no neighbors",
    // SENTENCE_PARAMETER_AT_LEAF
    "this node has no children",
    // SENTENCE_PARAMETER_AT_ROOT
    "there are no more parents above",
    // SENTENCE_PARAMETER_REQUIRED
    "'$' needs explicit arguments",
    // SENTENCE_PARENT_STRICT
    "parent needs explicit arguments for all its required children",
    // SENTENCE_PROGRAM_TERMINATED
    "TERMINATED",
    // SENTENCE_PROGRAM_ERROR
    "ERROR",
    // SENTENCE_PROGRAM_CANCELED
    "CANCELED",
    // SENTENCE_PROGRAM_DONE
    "DONE",
    // SENTENCE_RESET_INPUT_THIS
    "this input is removed",
    // SENTENCE_RESET_INPUT_DESCENDANTS
    "descendant inputs are removed",
    // SENTENCE_RESET_OUTPUT_THIS
    "this output is removed",
    // SENTENCE_RESET_OUTPUT_DESCENDANTS
    "descendant outputs are removed",
    // SENTENCE_SELECT_INPUT_DOWN
    "input selected down [$]",
    // SENTENCE_SELECT_INPUT_UP
    "input selected up [$]",
    // SENTENCE_SELECT_OUTPUT_DOWN
    "output selected down [$]",
    // SENTENCE_SELECT_OUTPUT_UP
    "output selected up [$]",
    // SENTENCE_UNKNOWN_VALUE
    "unknown value '$'"
  }}};

  CONLOR_CODE Langu::xMessage::consoleCodes[SENTENCE_TOTAL] = {
    // SENTENCE_ARGUMENT_ADDED
    CONLOR_CORRECT,
    // SENTENCE_ARGUMENT_REQUIRED
    CONLOR_ERROR,
    // SENTENCE_CLIPBOARD_OPEN_FAILURE
    CONLOR_ERROR,
    // SENTENCE_CLIPBOARD_GLOBAL_LOCK_FAILURE
    CONLOR_ERROR,
    // SENTENCE_CLIPBOARD_GLOBAL_ALLOC_FAILURE
    CONLOR_ERROR,
    // SENTENCE_CLIPBOARD_GET_DATA_FAILURE
    CONLOR_ERROR,
    // SENTENCE_CLIPBOARD_COPY_SUCCEED
    CONLOR_HINT,
    // SENTENCE_CLIPBOARD_PASTE_SUCCEED
    CONLOR_HINT,
    // SENTENCE_COMMAND_UNREDO_NO_NEW_VISITS
    CONLOR_WARNING,
    // SENTENCE_EMPTY_INPUT_THIS
    CONLOR_WARNING,
    // SENTENCE_EMPTY_OUTPUT_THIS
    CONLOR_WARNING,
    // SENTENCE_EMPTY_OUTPUT_NAMED
    CONLOR_ERROR,
    // SENTENCE_FILE_OVERWRITE_QUESTION
    CONLOR_WARNING,
    // SENTENCE_FILE_WRITE_FAILURE
    CONLOR_ERROR,
    // SENTENCE_FILE_WRITE_SUCCEED
    CONLOR_HINT,
    // SENTENCE_FORBIDDEN_CONTROLLER
    CONLOR_ERROR,
    // SENTENCE_FORBIDDEN_HIDDEN_PASTE
    CONLOR_WARNING,
    // SENTENCE_INTERRUPTION_DIALOG
    CONLOR_ERROR,
    // SENTENCE_KEYWORD_NOT_FOUND
    CONLOR_ERROR,
    // SENTENCE_MODE_ALREADY_MODIFYING
    CONLOR_WARNING,
    // SENTENCE_MODE_ALREADY_SELECTING
    CONLOR_WARNING,
    // SENTENCE_MODE_SWITCH_TO_MODIFICATION
    CONLOR_HINT,
    // SENTENCE_MODE_SWITCH_TO_SELECTION
    CONLOR_HINT,
    // SENTENCE_PARAMETER_ALONE
    CONLOR_WARNING,
    // SENTENCE_PARAMETER_AT_LEAF
    CONLOR_WARNING,
    // SENTENCE_PARAMETER_AT_ROOT
    CONLOR_WARNING,
    // SENTENCE_PARAMETER_REQUIRED
    CONLOR_ERROR,
    // SENTENCE_PARENT_STRICT
    CONLOR_ERROR,
    // SENTENCE_PROGRAM_TERMINATED
    CONLOR_CANCEL,
    // SENTENCE_PROGRAM_ERROR
    CONLOR_ERROR,
    // SENTENCE_PROGRAM_CANCELED
    CONLOR_CANCEL,
    // SENTENCE_PROGRAM_DONE
    CONLOR_CORRECT,
    // SENTENCE_RESET_INPUT_THIS
    CONLOR_HINT,
    // SENTENCE_RESET_INPUT_DESCENDANTS
    CONLOR_HINT,
    // SENTENCE_RESET_OUTPUT_THIS
    CONLOR_HINT,
    // SENTENCE_RESET_OUTPUT_DESCENDANTS
    CONLOR_HINT,
    // SENTENCE_SELECT_INPUT_DOWN
    CONLOR_HINT,
    // SENTENCE_SELECT_INPUT_UP
    CONLOR_HINT,
    // SENTENCE_SELECT_OUTPUT_DOWN
    CONLOR_HINT,
    // SENTENCE_SELECT_OUTPUT_UP
    CONLOR_HINT,
    // SENTENCE_UNKNOWN_VALUE
    CONLOR_ERROR
  };

  void Langu::ageMessage::setSentences(
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
    mt::CR_STR selectInputDownSentence,
    mt::CR_STR selectInputUpSentence,
    mt::CR_STR selectOutputDownSentence,
    mt::CR_STR selectOutputUpSentence,
    mt::CR_STR unknownValueSentence
  ) {
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_ARGUMENT_ADDED] = argumentAddedSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_ARGUMENT_REQUIRED] = argumentRequiredSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_CLIPBOARD_OPEN_FAILURE] = clipboardOpenFailureSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_CLIPBOARD_GLOBAL_LOCK_FAILURE] = clipboardGlobalLockFailureSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_CLIPBOARD_GLOBAL_ALLOC_FAILURE] = clipboardGlobalAllocFailureSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_CLIPBOARD_GET_DATA_FAILURE] = clipboardGetDataFailureSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_CLIPBOARD_COPY_SUCCEED] = clipboardCopySucceedSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_CLIPBOARD_PASTE_SUCCEED] = clipboardPasteSucceedSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_COMMAND_UNREDO_NO_NEW_VISITS] = commandUnredoNoNewVisitsSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_EMPTY_INPUT_THIS] = emptyInputThisSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_EMPTY_OUTPUT_THIS] = emptyOutputThisSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_EMPTY_OUTPUT_NAMED] = emptyOutputNamedSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_FILE_OVERWRITE_QUESTION] = fileOverwriteQuestionSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_FILE_WRITE_FAILURE] = fileWriteFailureSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_FILE_WRITE_SUCCEED] = fileWriteSucceedSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_FORBIDDEN_CONTROLLER] = forbiddenControllerSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_FORBIDDEN_HIDDEN_PASTE] = forbiddenHiddenPasteSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_INTERRUPTION_DIALOG] = interruptionDialogSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_KEYWORD_NOT_FOUND] = keywordNotFoundSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_MODE_ALREADY_MODIFYING] = modeAlreadyModifyingSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_MODE_ALREADY_SELECTING] = modeAlreadySelectingSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_MODE_SWITCH_TO_MODIFICATION] = modeSwitchToModificationSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_MODE_SWITCH_TO_SELECTION] = modeSwitchToSelectionSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_PARAMETER_ALONE] = parameterAloneSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_PARAMETER_AT_LEAF] = parameterAtLeafSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_PARAMETER_AT_ROOT] = parameterAtRootSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_PARAMETER_REQUIRED] = parameterRequiredSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_PARENT_STRICT] = parentStrictSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_PROGRAM_TERMINATED] = programTerminatedSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_PROGRAM_ERROR] = programErrorSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_PROGRAM_CANCELED] = programCanceledSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_PROGRAM_DONE] = programDoneSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_RESET_INPUT_THIS] = resetInputThisSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_RESET_INPUT_DESCENDANTS] = resetInputDescendantsSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_RESET_OUTPUT_THIS] = resetOutputThisSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_RESET_OUTPUT_DESCENDANTS] = resetOutputDescendantsSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_SELECT_INPUT_DOWN] = selectInputDownSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_SELECT_INPUT_UP] = selectInputUpSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_SELECT_OUTPUT_DOWN] = selectOutputDownSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_SELECT_OUTPUT_UP] = selectOutputUpSentence;
    Langu::xMessage::sentences[Langu::xManager::currentISOCode][SENTENCE_UNKNOWN_VALUE] = unknownValueSentence;
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
    mt::VEC_STR replacementStrings,
    mt::CR_BOL withYesOrNoLabel
  ) {
    Console::logResponse(
      Langu::xMessage::consoleCodes[responseCode],
      Langu::ageManager::replaceTemplateString(
        Langu::xMessage::sentences[Langu::xManager::currentISOCode][responseCode],
        replacementStrings
      ) + (withYesOrNoLabel ? " " + Langu::ageBooleanizer::getYesOrNoLabel() : "")
    );
  }

  mt::UNORMAP_STR<std::string> Langu::xMessage::welcomeToString = {{
    Langu::xManager::defaultISOCode, "Welcome to"
  }};

  std::string Langu::ageMessage::getWelcomeToString() {
    return Langu::xMessage::welcomeToString[Langu::xManager::currentISOCode];
  }

  //___________|
  // COLOR SET |
  //___________|

  mt::UNORMAP_STR<std::string> Langu::xColorSet::title = {{
    Langu::xManager::defaultISOCode, "PRESET COLORS"
  }};

  mt::UNORMAP_STR<mt::ARR_STR<COLOR_TOTAL>> Langu::xColorSet::names = {{ Langu::xManager::defaultISOCode, {
    "AZURE", "BLACK", "BLUE", "BROWN", "CANARY",
    "CHARTREUSE", "CHOCOLATE", "CRIMSON", "CYAN", "FOREST_GREEN",
    "GOLD", "GRAY", "GREEN", "LIGHT_BLUE", "LIGHT_GREEN",
    "LIME", "MAGENTA", "MAROON", "MINT", "NAVY_BLUE",
    "OLIVE", "ORANGE", "PINK", "PURPLE", "RED",
    "ROYAL_BLUE", "SALMON", "SILVER", "SKY_BLUE", "TEAL",
    "VIOLET", "WENGE", "WHEAT", "WHITE", "YELLOW"
  }}};

  void Langu::ageColorSet::setTitle(mt::CR_STR title) {
    Langu::xColorSet::title[Langu::xManager::currentISOCode] = title;
  }

  void Langu::ageColorSet::setNames(
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
  ) {
    Langu::xColorSet::names[Langu::xManager::currentISOCode][AZURE] = azureName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][BLACK] = blackName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][BLUE] = blueName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][BROWN] = brownName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][CANARY] = canaryName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][CHARTREUSE] = chartreuseName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][CHOCOLATE] = chocolateName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][CRIMSON] = crimsonName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][CYAN] = cyanName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][FOREST_GREEN] = forestgreenName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][GOLD] = goldName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][GRAY] = grayName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][GREEN] = greenName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][LIGHT_BLUE] = lightblueName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][LIGHT_GREEN] = lightgreenName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][LIME] = limeName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][MAGENTA] = magentaName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][MAROON] = maroonName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][MINT] = mintName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][NAVY_BLUE] = navyblueName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][OLIVE] = oliveName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][ORANGE] = orangeName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][PINK] = pinkName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][PURPLE] = purpleName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][RED] = redName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][ROYAL_BLUE] = royalblueName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][SALMON] = salmonName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][SILVER] = silverName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][SKY_BLUE] = skyblueName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][TEAL] = tealName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][VIOLET] = violetName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][WENGE] = wengeName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][WHEAT] = wheatName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][WHITE] = whiteName;
    Langu::xColorSet::names[Langu::xManager::currentISOCode][YELLOW] = yellowName;
  }

  std::string Langu::ageColorSet::getTitle() {
    return Langu::xColorSet::title[Langu::xManager::currentISOCode];
  }

  std::string Langu::ageColorSet::getName(mt::CR<COLOR_CODE> code) {
    return Langu::xColorSet::names[Langu::xManager::currentISOCode][code];
  }

  //_________|
  // CONTROL |
  //_________|

  mt::UNORMAP_STR<mt::ARR_STR<CONTROL_TOTAL>> Langu::xControl::terms = {{ Langu::xManager::defaultISOCode, {
    "show this help",             // CONTROL_COMMAND_HELP
    "show controller list",       // CONTROL_CONTROLLER_LIST
    "show children list",         // CONTROL_CHILDREN_LIST
    "clear terminal screen",      // CONTROL_CLEAR_SCREEN
    "go to next neighbor",        // CONTROL_NEIGHBOR_NEXT
    "go to previous neighbor",    // CONTROL_NEIGHBOR_PREVIOUS
    "enter children level",       // CONTROL_CHILDREN_ENTER
    "execute callbacks",          // CONTROL_CHILDREN_EXECUTE
    "modify this input",          // CONTROL_SWITCH_MODIFICATION
    "select another node",        // CONTROL_SWITCH_SELECTION
    "select input up",            // CONTROL_SELECT_INPUT_DOWN
    "select input down",          // CONTROL_SELECT_INPUT_UP
    "select output up",           // CONTROL_SELECT_OUTPUT_DOWN
    "select output down",         // CONTROL_SELECT_OUTPUT_UP
    "view this input",            // CONTROL_VIEW_INPUT_THIS
    "view descendant inputs",     // CONTROL_VIEW_INPUT_CHILDREN
    "view this output",           // CONTROL_VIEW_OUTPUT_THIS
    "view descendant outputs",    // CONTROL_VIEW_OUTPUT_CHILDREN
    "reset this input",           // CONTROL_RESET_INPUT_THIS
    "reset descendant inputs",    // CONTROL_RESET_INPUT_DESCENDANTS
    "reset this output",          // CONTROL_RESET_OUTPUT_THIS
    "reset descendant outputs",   // CONTROL_RESET_OUTPUT_DESCENDANTS
    "reset this data",            // CONTROL_RESET_DATA_THIS
    "reset descendant data",      // CONTROL_RESET_DATA_DESCENDANTS
    "copy text from this output", // CONTROL_COPY_OUTPUT
    "paste text into this input", // CONTROL_PASTE_INPUT
    "undo node level changes",    // CONTROL_NODE_LEVEL_UNDO
    "redo node level changes",    // CONTROL_NODE_LEVEL_REDO
    "back to parent level",       // CONTROL_PARENT_BACK
    "back to root level",         // CONTROL_ROOT_BACK
    "exit program"                // CONTROL_PROGRAM_QUIT
  }}};

  mt::UNORMAP_STR<std::string>
    Langu::xControl::abbreviationsTitle = {{
      Langu::xManager::defaultISOCode, "Controller List"
    }},
    Langu::xControl::abbreviationsDescription = {{
      Langu::xManager::defaultISOCode,
      std::string("Each of them can only be detected if the prompt appears as itself. ")
      + std::string("When combined with other strings in selection mode, it is treated as a keyword, ")
      + std::string("whereas in dialog mode, it is treated as a value.")
    }},
    Langu::xControl::booleanAvailableValuesTitle = {{
      Langu::xManager::defaultISOCode, "Boolean Available Values"
    }},
    Langu::xControl::booleanAvailableValuesDescription = {{
      Langu::xManager::defaultISOCode,
      std::string("Accepts truthy and falsy values as well as a controller. ")
      + std::string("For cancelation, pick one of the bottommost list.")
    }};

  void Langu::ageControl::setTerms(
    mt::CR_STR commandHelpTerm,
    mt::CR_STR controllerListTerm,
    mt::CR_STR childrenListTerm,
    mt::CR_STR clearScreenTerm,
    mt::CR_STR neighborNextTerm,
    mt::CR_STR neighborPreviousTerm,
    mt::CR_STR childrenEnterTerm,
    mt::CR_STR childrenExecuteTerm,
    mt::CR_STR switchModificationTerm,
    mt::CR_STR switchSelectionTerm,
    mt::CR_STR selectInputDownTerm,
    mt::CR_STR selectInputUpTerm,
    mt::CR_STR selectOutputDownTerm,
    mt::CR_STR selectOutputUpTerm,
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
  ) {
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_COMMAND_HELP] = commandHelpTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_CONTROLLER_LIST] = controllerListTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_CHILDREN_LIST] = childrenListTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_CLEAR_SCREEN] = clearScreenTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_NEIGHBOR_NEXT] = neighborNextTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_NEIGHBOR_PREVIOUS] = neighborPreviousTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_CHILDREN_ENTER] = childrenEnterTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_CHILDREN_EXECUTE] = childrenExecuteTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_SWITCH_MODIFICATION] = switchModificationTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_SWITCH_SELECTION] = switchSelectionTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_SELECT_INPUT_DOWN] = selectInputDownTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_SELECT_INPUT_UP] = selectInputUpTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_SELECT_OUTPUT_DOWN] = selectOutputDownTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_SELECT_OUTPUT_UP] = selectOutputUpTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_VIEW_INPUT_THIS] = viewInputThisTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_VIEW_INPUT_CHILDREN] = viewInputChildrenTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_VIEW_OUTPUT_THIS] = viewOutputThisTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_VIEW_OUTPUT_CHILDREN] = viewOutputChildrenTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_RESET_INPUT_THIS] = resetInputThisTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_RESET_INPUT_DESCENDANTS] = resetInputDescendantsTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_RESET_OUTPUT_THIS] = resetOutputThisTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_RESET_OUTPUT_DESCENDANTS] = resetOutputDescendantsTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_RESET_DATA_THIS] = resetDataThisTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_RESET_DATA_DESCENDANTS] = resetDataDescendantsTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_COPY_OUTPUT] = copyOutputTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_PASTE_INPUT] = pasteInputTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_NODE_LEVEL_UNDO] = nodeLevelUndoTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_NODE_LEVEL_REDO] = nodeLevelRedoTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_PARENT_BACK] = parentBackTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_ROOT_BACK] = rootBackTerm;
    Langu::xControl::terms[Langu::xManager::currentISOCode][CONTROL_PROGRAM_QUIT] = programQuitTerm;
  }

  void Langu::ageControl::setAbbreviationsTitle(mt::CR_STR title) {
    Langu::xControl::abbreviationsTitle[Langu::xManager::currentISOCode] = title;
  }

  void Langu::ageControl::setAbbreviationsDescription(mt::CR_STR description) {
    Langu::xControl::abbreviationsDescription[Langu::xManager::currentISOCode] = description;
  }

  void Langu::ageControl::setBooleanAvailableValuesTitle(mt::CR_STR description) {
    Langu::xControl::booleanAvailableValuesTitle[Langu::xManager::currentISOCode] = description;
  }

  void Langu::ageControl::setBooleanAvailableValuesDescription(mt::CR_STR title) {
    Langu::xControl::booleanAvailableValuesDescription[Langu::xManager::currentISOCode] = title;
  }

  std::string Langu::ageControl::getTerm(mt::CR<CONTROL_CODE> code) {
    return Langu::xControl::terms[Langu::xManager::currentISOCode][code];
  }

  std::string Langu::ageControl::getAbbreviationsTitle() {
    return Langu::xControl::abbreviationsTitle[Langu::xManager::currentISOCode];
  }

  std::string Langu::ageControl::getAbbreviationsDescription() {
    return Langu::xControl::abbreviationsDescription[Langu::xManager::currentISOCode];
  }

  std::string Langu::ageControl::getBooleanAvailableValuesTitle() {
    return Langu::xControl::booleanAvailableValuesTitle[Langu::xManager::currentISOCode];
  }

  std::string Langu::ageControl::getBooleanAvailableValuesDescription() {
    return Langu::xControl::booleanAvailableValuesDescription[Langu::xManager::currentISOCode];
  }

  //_____________|
  // BOOLEANIZER |
  //_____________|

  mt::UNORMAP_STR<std::string> Langu::xBooleanizer::yesOrNoLabel = {{
    Langu::xManager::defaultISOCode, "Y/n"
  }};

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

  mt_uti::BOOLEANIZER_CODE Langu::ageBooleanizer::test(mt::CR_STR raw) {
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

  //___________|
  // PARAMETER |
  //___________|

  mt::UNORMAP_STR<mt::ARR_STR<PARAMETER_TYPE_TOTAL>>
  Langu::xParameter::stringifiedTypes = {{ Langu::xManager::defaultISOCode, {
    "WORD", "NUMBER", "BOOLEAN"
  }}};

  void Langu::ageParameter::setStringifiedTypes(
    mt::CR_STR wordStringifiedType,
    mt::CR_STR numberStringifiedType,
    mt::CR_STR booleanStringifiedType
  ) {
    Langu::xParameter::stringifiedTypes
    [Langu::xManager::currentISOCode]
    [STRINGIFIED_TYPE_INPUT_WORD] = wordStringifiedType;

    Langu::xParameter::stringifiedTypes
    [Langu::xManager::currentISOCode]
    [STRINGIFIED_TYPE_INPUT_NUMBER] = numberStringifiedType;

    Langu::xParameter::stringifiedTypes
    [Langu::xManager::currentISOCode]
    [STRINGIFIED_TYPE_INPUT_BOOLEAN] = booleanStringifiedType;
  }

  std::string Langu::ageParameter::getStringifiedType(
    const STRINGIFIED_TYPE_COMMAND_CODE &code
  ) {
    return Langu::xParameter::stringifiedTypes
    [Langu::xManager::currentISOCode][code];
  }

  //_________|
  // PROGRAM |
  //_________|

  mt::UNORMAP_STR<mt::ARR_STR<PROGRAM_LABEL_TOTAL>>
  Langu::xProgram::labels = {{ Langu::xManager::defaultISOCode, {
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

  mt::UNORMAP_STR<mt::ARR_STR<PRESET_KEYWORD_TOTAL>>
  Langu::xPreset::keywords = {{ Langu::xManager::defaultISOCode, {
    "in", "out", "help", "list", "colors"
  }}},
  Langu::xPreset::descriptions = {{ Langu::xManager::defaultISOCode, {
    "Enter input filename",
    "Enter output filename",
    "", "",
    "Print preset colors"
  }}};

  std::string Langu::agePreset::fileOutDefaultExtension = ".txt";
  mt::PAIR_STR Langu::agePreset::fileOutBracketsForNumbering = {"(", ")"};

  void Langu::agePreset::setKeywords(
    mt::CR_STR inKeyword,
    mt::CR_STR outKeyword,
    mt::CR_STR helpKeyword,
    mt::CR_STR listKeyword,
    mt::CR_STR colorSetKeyword
  ) {
    Langu::xPreset::keywords
    [Langu::xManager::currentISOCode]
    [PRESET_KEYWORD_IN] = inKeyword;

    Langu::xPreset::keywords
    [Langu::xManager::currentISOCode]
    [PRESET_KEYWORD_OUT] = outKeyword;

    Langu::xPreset::keywords
    [Langu::xManager::currentISOCode]
    [PRESET_KEYWORD_HELP] = helpKeyword;

    Langu::xPreset::keywords
    [Langu::xManager::currentISOCode]
    [PRESET_KEYWORD_LIST] = listKeyword;

    Langu::xPreset::keywords
    [Langu::xManager::currentISOCode]
    [PRESET_KEYWORD_COLOR_SET] = colorSetKeyword;
  }

  void Langu::agePreset::setDescription(
    mt::CR_STR inDescription,
    mt::CR_STR outDescription,
    mt::CR_STR helpDescription,
    mt::CR_STR listDescription,
    mt::CR_STR colorSetDescription
  ) {
    Langu::xPreset::descriptions
    [Langu::xManager::currentISOCode]
    [PRESET_KEYWORD_IN] = inDescription;

    Langu::xPreset::descriptions
    [Langu::xManager::currentISOCode]
    [PRESET_KEYWORD_OUT] = outDescription;

    Langu::xPreset::descriptions
    [Langu::xManager::currentISOCode]
    [PRESET_KEYWORD_HELP] = helpDescription;

    Langu::xPreset::descriptions
    [Langu::xManager::currentISOCode]
    [PRESET_KEYWORD_LIST] = listDescription;

    Langu::xPreset::descriptions
    [Langu::xManager::currentISOCode]
    [PRESET_KEYWORD_COLOR_SET] = colorSetDescription;
  }

  std::string Langu::agePreset::getKeyword(
    const PRESET_KEYWORD_CODE &code
  ) {
    return Langu::xPreset::keywords
    [Langu::xManager::currentISOCode][code];
  }

  std::string Langu::agePreset::getDescription(
    const PRESET_KEYWORD_CODE &code
  ) {
    return Langu::xPreset::descriptions
    [Langu::xManager::currentISOCode][code];
  }
}

#endif // __CLI_MENU__LANGUAGE_CPP__