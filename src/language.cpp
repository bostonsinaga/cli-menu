#ifndef __CLI_MENU__LANGUAGE_CPP__
#define __CLI_MENU__LANGUAGE_CPP__

#include "language.hpp"

namespace cli_menu {

  /** English Presets */
  mt::STRUNORMAP<mt::ARR_STR<Language::totalMessages> Language::messages = {{"en", {
    // ALREADY_SELECTING
    "Already in selection mode.",
    // ARGUMENT_REQUIRED
    "Unable to process without explicit arguments."
    // CLIPBOARD_OPEN_FAILURE,
    "Failed to open clipboard.",
    // CLIPBOARD_GET_FAILURE,
    "Failed to get clipboard data.",
    // CLIPBOARD_LOCK_FAILURE
    "Failed to lock clipboard data.",
    // CLIPBOARD_PASTED
    "Pasted from clipboard.",
    // COMMAND_NOT_FOUND
    "Parameter not found.",
    // FORBIDDEN_HIDDEN_PASTE
    "Hidden text pasting is only available on insertion.",
    // MIDDLE_DIALOG
    "Unable to process until the rest of direct inputs are processed.",
    // PARAMETER_ALONE
    "This parameter has no neighbors.",
    // PARAMETER_AT_LEAF
    "This does not contain any parameters.",
    // PARAMETER_AT_ROOT
    "No more groups above.",
    // PROGRAM_CANCELED
    "Program canceled.",
    // PROGRAM_FAILED
    "Program failed.",
    // PROGRAM_SUCCEEDED
    "Program succeeded.",
  }}};

  Console::CODE Language::consoleCodes[Language::totalMessages] = {
    // ALREADY_SELECTING
    Console::WARNING,
    // ARGUMENT_REQUIRED
    Console::ERROR,
    // CLIPBOARD_OPEN_FAILURE,
    Console::ERROR,
    // CLIPBOARD_GET_FAILURE,
    Console::ERROR,
    // CLIPBOARD_LOCK_FAILURE
    Console::ERROR,
    // CLIPBOARD_PASTED
    Console::HINT,
    // COMMAND_NOT_FOUND
    Console::ERROR,
    // FORBIDDEN_HIDDEN_PASTE
    Console::WARNING,
    // MIDDLE_DIALOG
    Console::ERROR,
    // PARAMETER_ALONE
    Console::WARNING,
    // PARAMETER_AT_LEAF
    Console::WARNING,
    // PARAMETER_AT_ROOT
    Console::WARNING,
    // PROGRAM_CANCELED
    Console::CANCEL,
    // PROGRAM_FAILED
    Console::ERROR,
    // PROGRAM_SUCCEEDED
    Console::CORRECT
  };

  bool Language::messagesFound(mt::CR_STR existingISOCode) {
    return mt::STRUNORMAP_FOUND<mt::ARR_STR<totalMessages>>(
      messages, existingISOCode
    );
  }

  void Language::addMessages(
    mt::CR_STR existingISOCode,
    mt::CR_STR alreadySelectingMessage,
    mt::CR_STR argumentRequiredMessage,
    mt::CR_STR clipboardOpenFailureMessage,
    mt::CR_STR clipboardGetFailureMessage,
    mt::CR_STR clipboardLockFailureMessage,
    mt::CR_STR clipboardPastedMessage,
    mt::CR_STR commandNotFoundMessage,
    mt::CR_STR forbiddenHiddenPasteMessage,
    mt::CR_STR middleDialogMessage,
    mt::CR_STR parameterAloneMessage,
    mt::CR_STR parameterAtLeafMessage,
    mt::CR_STR parameterAtRootMessage,
    mt::CR_STR programCanceledMessage,
    mt::CR_STR programFailedMessage,
    mt::CR_STR programSucceededMessage
  ) {
    if (messagesFound(existingISOCode)) {
      messages[existingISOCode][ALREADY_SELECTING] = alreadySelectingMessage;
      messages[existingISOCode][ARGUMENT_REQUIRED] = argumentRequiredMessage;
      messages[existingISOCode][CLIPBOARD_OPEN_FAILURE] = clipboardOpenFailureMessage;
      messages[existingISOCode][CLIPBOARD_GET_FAILURE] = clipboardGetFailureMessage;
      messages[existingISOCode][CLIPBOARD_LOCK_FAILURE] = clipboardLockFailureMessage;
      messages[existingISOCode][CLIPBOARD_PASTED] = clipboardPastedMessage;
      messages[existingISOCode][COMMAND_NOT_FOUND] = commandNotFoundMessage;
      messages[existingISOCode][FORBIDDEN_HIDDEN_PASTE] = forbiddenHiddenPasteMessage;
      messages[existingISOCode][MIDDLE_DIALOG] = middleDialogMessage;
      messages[existingISOCode][PARAMETER_ALONE] = parameterAloneMessage;
      messages[existingISOCode][PARAMETER_AT_LEAF] = parameterAtLeafMessage;
      messages[existingISOCode][PARAMETER_AT_ROOT] = parameterAtRootMessage;
      messages[existingISOCode][PROGRAM_CANCELED] = programCanceledMessage;
      messages[existingISOCode][PROGRAM_FAILED] = programFailedMessage;
      messages[existingISOCode][PROGRAM_SUCCEEDED] = programSucceededMessage;
    }
  }

  void Language::removeMessages(mt::CR_STR existingISOCode) {
    messages[existingISOCode].erase();
  }

  void Language::printResponse(
    mt::CR_STR existingISOCode,
    const CODE &responseCode
  ) {
    Console::logResponse(
      Language::consoleCodes[responseCode],
      Language::messages[existingISOCode][responseCode]
    );
  }
}

#endif // __CLI_MENU__LANGUAGE_CPP__