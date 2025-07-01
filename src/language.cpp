#ifndef __CLI_MENU__LANGUAGE_CPP__
#define __CLI_MENU__LANGUAGE_CPP__

#include "language.hpp"

namespace cli_menu {

  /** English Presets */
  std::string Language::messages["en"] = {
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
  };

  Console::CODE consoleCodes[totalMessages] = {
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

  void Language::registerCollection(
    mt::CR_STR ISOCode,
    mt::CR_ARR_STR<totalMessages> collection
  );

  void Language::printResponse(const CODE &code) {
    Console::logResponse(
      consoleCodes[code],
      messages[code]
    );
  }

  void Language::changeISOCode(mt::CR_STR ISOCode) {
    currentISOCode = ISOCode;
  }

  bool Language::booleanize(CR_STR input) {
    return booleanizer.test(input, currentISOCode);
  }
}

#endif // __CLI_MENU__LANGUAGE_CPP__