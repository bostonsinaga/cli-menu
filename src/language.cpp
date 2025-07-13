#ifndef __CLI_MENU__LANGUAGE_CPP__
#define __CLI_MENU__LANGUAGE_CPP__

#include "language.hpp"

namespace cli_menu {

  /** English Presets */
  mt::STRUNORMAP<mt::ARR_STR<Language::totalMessages>> Language::messages = {{"en", {
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
    "This parameter has no neighbors.",
    // LANGUAGE_PARAMETER_AT_LEAF
    "This does not contain any parameters.",
    // LANGUAGE_PARAMETER_AT_ROOT
    "No more groups above.",
    // LANGUAGE_PARAMETER_NOT_FOUND
    "Parameter not found.",
    // LANGUAGE_PROGRAM_CANCELED
    "Program canceled.",
    // LANGUAGE_PROGRAM_FAILED
    "Program failed.",
    // LANGUAGE_PROGRAM_SUCCEEDED
    "Program succeeded."
  }}};

  CONSOLE_CODE Language::consoleCodes[Language::totalMessages] = {
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
    CONSOLE_HINT,
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
    // LANGUAGE_PROGRAM_CANCELED
    CONSOLE_CANCEL,
    // LANGUAGE_PROGRAM_FAILED
    CONSOLE_ERROR,
    // LANGUAGE_PROGRAM_SUCCEEDED
    CONSOLE_CORRECT
  };

  bool Language::hasISOCode(mt::CR_STR existingISOCode) {
    return mt::STRUNORMAP_FOUND<mt::ARR_STR<totalMessages>>(
      messages, existingISOCode
    );
  }

  void Language::selectISOCode(mt::CR_STR existingISOCode) {
    if (hasISOCode(existingISOCode)) {
      currentISOCode = existingISOCode;
    }
  }

  void Language::setMessages(
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
    mt::CR_STR programCanceledMessage,
    mt::CR_STR programFailedMessage,
    mt::CR_STR programSucceededMessage
  ) {
    messages[currentISOCode][LANGUAGE_ALREADY_SELECTING] = alreadySelectingMessage;
    messages[currentISOCode][LANGUAGE_ARGUMENT_REQUIRED] = argumentRequiredMessage;
    messages[currentISOCode][LANGUAGE_CLIPBOARD_OPEN_FAILURE] = clipboardOpenFailureMessage;
    messages[currentISOCode][LANGUAGE_CLIPBOARD_GET_FAILURE] = clipboardGetFailureMessage;
    messages[currentISOCode][LANGUAGE_CLIPBOARD_LOCK_FAILURE] = clipboardLockFailureMessage;
    messages[currentISOCode][LANGUAGE_CLIPBOARD_PASTED] = clipboardPastedMessage;
    messages[currentISOCode][LANGUAGE_FORBIDDEN_HIDDEN_PASTE] = forbiddenHiddenPasteMessage;
    messages[currentISOCode][LANGUAGE_MIDDLE_DIALOG] = middleDialogMessage;
    messages[currentISOCode][LANGUAGE_PARAMETER_ALONE] = parameterAloneMessage;
    messages[currentISOCode][LANGUAGE_PARAMETER_AT_LEAF] = parameterAtLeafMessage;
    messages[currentISOCode][LANGUAGE_PARAMETER_AT_ROOT] = parameterAtRootMessage;
    messages[currentISOCode][LANGUAGE_PARAMETER_NOT_FOUND] = parameterNotFoundMessage;
    messages[currentISOCode][LANGUAGE_PROGRAM_CANCELED] = programCanceledMessage;
    messages[currentISOCode][LANGUAGE_PROGRAM_FAILED] = programFailedMessage;
    messages[currentISOCode][LANGUAGE_PROGRAM_SUCCEEDED] = programSucceededMessage;
  }

  void Language::printResponse(const LANGUAGE_CODE &responseCode) {
    Console::logResponse(
      Language::consoleCodes[responseCode],
      Language::messages[currentISOCode][responseCode]
    );
  }
}

#endif // __CLI_MENU__LANGUAGE_CPP__