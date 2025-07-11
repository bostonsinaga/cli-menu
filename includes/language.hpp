#ifndef __CLI_MENU__LANGUAGE_HPP__
#define __CLI_MENU__LANGUAGE_HPP__

#include <atomic>
#include "console.hpp"

namespace cli_menu {

  enum LANGUAGE_CODE {
    LANGUAGE_ALREADY_SELECTING,
    LANGUAGE_ARGUMENT_REQUIRED,
    LANGUAGE_CLIPBOARD_OPEN_FAILURE,
    LANGUAGE_CLIPBOARD_GET_FAILURE,
    LANGUAGE_CLIPBOARD_LOCK_FAILURE,
    LANGUAGE_CLIPBOARD_PASTED,
    LANGUAGE_CTRL_C_SIGNAL_RECEIVED,
    LANGUAGE_FORBIDDEN_HIDDEN_PASTE,
    LANGUAGE_MIDDLE_DIALOG,
    LANGUAGE_PARAMETER_ALONE,
    LANGUAGE_PARAMETER_AT_LEAF,
    LANGUAGE_PARAMETER_AT_ROOT,
    LANGUAGE_PARAMETER_NOT_FOUND,
    LANGUAGE_PROGRAM_CANCELED,
    LANGUAGE_PROGRAM_FAILED,
    LANGUAGE_PROGRAM_SUCCEEDED
  };

  typedef const LANGUAGE_CODE& CR_LANGUAGE_CODE;

  class Language {
  private:
    static constexpr int totalMessages = 16;
    static mt::STRUNORMAP<mt::ARR_STR<totalMessages>> messages;
    static CONSOLE_CODE consoleCodes[totalMessages];

    // default english
    inline static std::string currentISOCode = "en";

    // use an atomic boolean to signal an interrupt
    inline static std::atomic<bool> INTERRUPTED_CTRL_C = false;

    friend class Control;

  public:
    Language() = delete;

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
      mt::CR_STR forbiddenHiddenPasteMessage,
      mt::CR_STR middleDialogMessage,
      mt::CR_STR parameterAloneMessage,
      mt::CR_STR parameterAtLeafMessage,
      mt::CR_STR parameterAtRootMessage,
      mt::CR_STR parameterNotFoundMessage,
      mt::CR_STR programCanceledMessage,
      mt::CR_STR programFailedMessage,
      mt::CR_STR programSucceededMessage
    );

    static void printResponse(CR_LANGUAGE_CODE responseCode);

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