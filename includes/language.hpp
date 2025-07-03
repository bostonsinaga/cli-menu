#ifndef __CLI_MENU__LANGUAGE_HPP__
#define __CLI_MENU__LANGUAGE_HPP__

#include "console.hpp"

namespace cli_menu {

  class Language {
  public:
    enum CODE {
      ALREADY_SELECTING,
      ARGUMENT_REQUIRED,
      CLIPBOARD_OPEN_FAILURE,
      CLIPBOARD_GET_FAILURE,
      CLIPBOARD_LOCK_FAILURE,
      CLIPBOARD_PASTED,
      COMMAND_NOT_FOUND,
      FORBIDDEN_HIDDEN_PASTE,
      MIDDLE_DIALOG,
      PARAMETER_ALONE,
      PARAMETER_AT_LEAF,
      PARAMETER_AT_ROOT,
      PROGRAM_CANCELED,
      PROGRAM_FAILED,
      PROGRAM_SUCCEEDED
    };

  private:
    static constexpr int totalMessages = 15;
    static mt::STRUNORMAP<mt::ARR_STR<totalMessages> messages;
    static consoleCodes[totalMessages];

  public:
    Language() = delete;
    bool hasISOCode(mt::CR_STR existingISOCode);

    static void addMessages(
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
    );

    static void removeMessages(mt::CR_STR existingISOCode);

    static void printResponse(
      mt::CR_STR responseCode,
      const CODE &code
    );
  };
}

#endif // __CLI_MENU__LANGUAGE_HPP__