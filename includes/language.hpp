#ifndef __CLI_MENU__LANGUAGE_HPP__
#define __CLI_MENU__LANGUAGE_HPP__

#include "console.hpp"

namespace cli_menu {

  enum LANGUAGE_CODE {
    LANGUAGE_ALREADY_MODIFYING,
    LANGUAGE_ALREADY_SELECTING,
    LANGUAGE_ARGUMENT_REQUIRED,
    LANGUAGE_CLIPBOARD_OPEN_FAILURE,
    LANGUAGE_CLIPBOARD_GET_FAILURE,
    LANGUAGE_CLIPBOARD_LOCK_FAILURE,
    LANGUAGE_CLIPBOARD_PASTED,
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

  enum LANGUAGE_COMMAND_STRINGIFIED_TYPE {
    LANGUAGE_WORD_STRINGIFIED_TYPE,
    LANGUAGE_NUMBER_STRINGIFIED_TYPE,
    LANGUAGE_TOGGLE_STRINGIFIED_TYPE
  };

  class Language {
  private:
    // messages
    static constexpr int totalMessages = 16;
    static mt::STRUNORMAP<mt::ARR_STR<totalMessages>> messages;
    static CONSOLE_CODE consoleCodes[totalMessages];

    // command stringified types
    static constexpr int totalCommandTypes = 3;
    static mt::STRUNORMAP<mt::ARR_STR<totalCommandTypes>> stringifiedCommandTypes;

    // default english
    inline static std::string currentISOCode = "en";

    friend class Control;

  public:
    Language() = delete;

    // ask keyword existance in 'messages'
    static bool hasISOCode(mt::CR_STR existingISOCode);

    // change 'currentISOCode' value
    static void selectISOCode(mt::CR_STR existingISOCode);

    static void setMessages(
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
      mt::CR_STR programCanceledMessage,
      mt::CR_STR programFailedMessage,
      mt::CR_STR programSucceededMessage
    );

    static void printResponse(const LANGUAGE_CODE &responseCode);

    static void setStringifiedCommandTypes(
      mt::CR_STR wordStringifiedType,
      mt::CR_STR numberStringifiedType,
      mt::CR_STR toggleStringifiedType
    );

    static mt::CR_STR getStringifiedType(
      const LANGUAGE_COMMAND_STRINGIFIED_TYPE &stringifiedTypeIndex
    );
  };
}

#endif // __CLI_MENU__LANGUAGE_HPP__