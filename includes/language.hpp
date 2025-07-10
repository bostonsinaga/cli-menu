#ifndef __CLI_MENU__LANGUAGE_HPP__
#define __CLI_MENU__LANGUAGE_HPP__

#include <atomic>
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
      CTRL_C_SIGNAL_RECEIVED,
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
    static constexpr int totalMessages = 16;
    static mt::STRUNORMAP<mt::ARR_STR<totalMessages>> messages;
    static Console::CODE consoleCodes[totalMessages];

    // default english
    inline static std::string currentISOCode = "en";

    // use an atomic boolean to signal an interrupt
    inline static std::atomic<bool> INTERRUPTED_CTRL_C = false;

  public:
    Language() = delete;

    static mt::CR_STR getCurrentISOCode() {
      return currentISOCode;
    }

    // ask keyword existance in 'messages'
    static bool hasISOCode(mt::CR_STR existingISOCode);

    // change 'currentISOCode' value
    static void selectISOCode(mt::CR_STR existingISOCode);

    static void setMessages(
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

    static void printResponse(const CODE &responseCode);

    /** Interrupted 'Ctrl+C' Interactions */

    // decorated input interface
    static bool cinDialogInput(std::string &buffer);

    // to prevent infinite loop after pressing 'Ctrl+C'
    static void setInterruptedCtrlC(int);

    // check if interrupted before waiting for input
    static bool isInterruptedCtrlC();
  };
}

#endif // __CLI_MENU__LANGUAGE_HPP__