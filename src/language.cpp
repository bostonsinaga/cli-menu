#ifndef __CLI_MENU__LANGUAGE_CPP__
#define __CLI_MENU__LANGUAGE_CPP__

#include "language.hpp"

namespace cli_menu {

  /** English Presets */
  mt::STRUNORMAP<mt::ARR_STR<Language::totalMessages>> Language::messages = {{"en", {
    // LANGUAGE_ALREADY_MODIFYING
    "Already in edit mode.",
    // LANGUAGE_ALREADY_SELECTING
    "Already in selection mode.",
    // LANGUAGE_ARGUMENT_REQUIRED
    "Unable to process without explicit arguments.",
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
    // LANGUAGE_MIDDLE_DIALOG
    "Unable to process until the rest of direct inputs are processed.",
    // LANGUAGE_PARAMETER_ALONE
    "This node has no neighbors.",
    // LANGUAGE_PARAMETER_AT_LEAF
    "This node does not contain any children.",
    // LANGUAGE_PARAMETER_AT_ROOT
    "No more groups above.",
    // LANGUAGE_PARAMETER_NOT_FOUND
    "Parameter not found.",
    // LANGUAGE_PARENT_STRICT
    "Parent node needs explicit arguments for all its required children.",
    // LANGUAGE_PROGRAM_CANCELED
    "Program canceled.",
    // LANGUAGE_PROGRAM_FAILED
    "Program failed.",
    // LANGUAGE_PROGRAM_SUCCEEDED
    "Program succeeded."
  }}};

  CONSOLE_CODE Language::consoleCodes[Language::totalMessages] = {
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
    // LANGUAGE_MIDDLE_DIALOG
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

  mt::STRUNORMAP<mt::ARR_STR<Language::totalCommandTypes>>
  Language::stringifiedCommandTypes = {{"en", {
    "WORD", "NUMBER", "TOGGLE"
  }}};

  bool Language::hasISOCode(mt::CR_STR existingISOCode) {
    return mt::STRUNORMAP_FOUND<mt::ARR_STR<Language::totalMessages>>(
      Language::messages, existingISOCode
    );
  }

  void Language::selectISOCode(mt::CR_STR existingISOCode) {
    if (Language::hasISOCode(existingISOCode)) {
      Language::currentISOCode = existingISOCode;
    }
  }

  void Language::setMessages(
    mt::CR_STR alreadyModifyingMessage,
    mt::CR_STR alreadySelectingMessage,
    mt::CR_STR argumentRequiredMessage,
    mt::CR_STR clipboardOpenFailureMessage,
    mt::CR_STR clipboardGetFailureMessage,
    mt::CR_STR clipboardLockFailureMessage,
    mt::CR_STR clipboardPastedMessage,
    mt::CR_STR forbiddenHiddenPasteMessage,
    mt::CR_STR middleDialogMessage,
    mt::CR_STR parameterAloneMessage,
    mt::CR_STR parameterAtLeafMessage,
    mt::CR_STR parameterAtRootMessage,
    mt::CR_STR parameterNotFoundMessage,
    mt::CR_STR parentStrictMessage,
    mt::CR_STR programCanceledMessage,
    mt::CR_STR programFailedMessage,
    mt::CR_STR programSucceededMessage
  ) {
    Language::messages[Language::currentISOCode][LANGUAGE_ALREADY_MODIFYING] = alreadyModifyingMessage;
    Language::messages[Language::currentISOCode][LANGUAGE_ALREADY_SELECTING] = alreadySelectingMessage;
    Language::messages[Language::currentISOCode][LANGUAGE_ARGUMENT_REQUIRED] = argumentRequiredMessage;
    Language::messages[Language::currentISOCode][LANGUAGE_CLIPBOARD_OPEN_FAILURE] = clipboardOpenFailureMessage;
    Language::messages[Language::currentISOCode][LANGUAGE_CLIPBOARD_GET_FAILURE] = clipboardGetFailureMessage;
    Language::messages[Language::currentISOCode][LANGUAGE_CLIPBOARD_LOCK_FAILURE] = clipboardLockFailureMessage;
    Language::messages[Language::currentISOCode][LANGUAGE_CLIPBOARD_PASTED] = clipboardPastedMessage;
    Language::messages[Language::currentISOCode][LANGUAGE_FORBIDDEN_HIDDEN_PASTE] = forbiddenHiddenPasteMessage;
    Language::messages[Language::currentISOCode][LANGUAGE_MIDDLE_DIALOG] = middleDialogMessage;
    Language::messages[Language::currentISOCode][LANGUAGE_PARAMETER_ALONE] = parameterAloneMessage;
    Language::messages[Language::currentISOCode][LANGUAGE_PARAMETER_AT_LEAF] = parameterAtLeafMessage;
    Language::messages[Language::currentISOCode][LANGUAGE_PARAMETER_AT_ROOT] = parameterAtRootMessage;
    Language::messages[Language::currentISOCode][LANGUAGE_PARAMETER_NOT_FOUND] = parameterNotFoundMessage;
    Language::messages[Language::currentISOCode][LANGUAGE_PARENT_STRICT] = parentStrictMessage;
    Language::messages[Language::currentISOCode][LANGUAGE_PROGRAM_CANCELED] = programCanceledMessage;
    Language::messages[Language::currentISOCode][LANGUAGE_PROGRAM_FAILED] = programFailedMessage;
    Language::messages[Language::currentISOCode][LANGUAGE_PROGRAM_SUCCEEDED] = programSucceededMessage;
  }

  void Language::printResponse(const LANGUAGE_CODE &responseCode) {
    Console::logResponse(
      Language::consoleCodes[responseCode],
      Language::messages[Language::currentISOCode][responseCode]
    );
  }

  void Language::setStringifiedCommandTypes(
    mt::CR_STR wordStringifiedType,
    mt::CR_STR numberStringifiedType,
    mt::CR_STR toggleStringifiedType
  ) {
    Language::stringifiedCommandTypes
    [Language::currentISOCode]
    [LANGUAGE_WORD_STRINGIFIED_TYPE] = wordStringifiedType;

    Language::stringifiedCommandTypes
    [Language::currentISOCode]
    [LANGUAGE_NUMBER_STRINGIFIED_TYPE] = numberStringifiedType;

    Language::stringifiedCommandTypes
    [Language::currentISOCode]
    [LANGUAGE_TOGGLE_STRINGIFIED_TYPE] = toggleStringifiedType;
  }

  mt::CR_STR Language::getStringifiedType(
    const LANGUAGE_COMMAND_STRINGIFIED_TYPE &stringifiedTypeIndex
  ) {
    return Language::stringifiedCommandTypes
    [Language::currentISOCode]
    [stringifiedTypeIndex];
  }
}

#endif // __CLI_MENU__LANGUAGE_CPP__