#ifndef __CLI_MENU__CONSOLE_HPP__
#define __CLI_MENU__CONSOLE_HPP__

#include "color.hpp"

namespace cli_menu {

  enum CONSOLE_CODE {
    CONSOLE_ERROR, CONSOLE_WARNING, CONSOLE_HIGHLIGHT,
    CONSOLE_HINT, CONSOLE_CORRECT, CONSOLE_CANCEL
  };

  class Console {
  private:
    static constexpr int totalStatus = 6;

  public:
    Console() = delete;

    inline static char boundaryCharacter = '-';
    inline static size_t boundaryCharactersAmount = 45;
    inline static std::string listPointStyle = ">";
    inline static bool outlineStyle = true;

    static Color messageColors[totalStatus];

    static Color
      boundaryModifyColor,
      boundarySelectionColor,
      boxModifyColors[2],
      boxSelectionColors[2];

    /** NORMAL */

    static void logString(
      mt::CR_STR text,
      CR_CLR foreground
    );

    static void logString(
      mt::CR_STR text,
      CR_CLR foreground,
      CR_CLR background
    );

    /** ITALIC */

    static void logItalicString(mt::CR_STR text);

    static void logItalicString(
      mt::CR_STR text,
      CR_CLR foreground
    );

    static void logItalicString(
      mt::CR_STR text,
      CR_CLR foreground,
      CR_CLR background
    );

    /** UNDERLINE */

    static void logUnderlineString(mt::CR_STR text);

    static void logUnderlineString(
      mt::CR_STR text,
      CR_CLR foreground
    );

    static void logUnderlineString(
      mt::CR_STR text,
      CR_CLR foreground,
      CR_CLR background
    );

    /**
     * SPECIALS
     * The 'editing' parameter, defined in 'Command::editing',
     * is used to switch between editing and selecting modes.
     * In this class, it serves to distinguish the theme.
     */

    static CR_CLR chooseBoundaryColor(mt::CR_BOL editing);
    static void logBoundaryLine(mt::CR_BOL editing);

    static void logStylishHeader(
      mt::CR_STR title,
      mt::CR_BOL editing
    );

    /**
     * Call 'logString' with 'listPointStyle' at the beginning,
     * colored with the predefined 'messageColors'.
     */
    static void logResponse(
      const CONSOLE_CODE &code,
      mt::CR_STR reason
    );
  };
}

#endif // __CLI_MENU__CONSOLE_HPP__