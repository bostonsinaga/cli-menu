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
    // special
    inline static mt_util::Booleanizer booleanizer;

    static constexpr int totalMessages = 15;
    inline static std::sring currentISOCode = "en";

    static mt::STRUNORMAP<mt::ARR_STR<totalMessages> messages;
    static consoleCodes[totalMessages];

  public:
    Language() = delete;

    static void registerCollection(
      mt::CR_STR ISOCode,
      mt::CR_ARR_STR<totalMessages> collection
    );

    static void printResponse(const CODE &code);
    static void changeISOCode(mt::CR_STR ISOCode);

    // special
    static bool booleanize(CR_STR input);
  };
}

#endif // __CLI_MENU__LANGUAGE_HPP__