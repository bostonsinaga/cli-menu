#ifndef __CLI_MENU__CONSOLE_HPP__
#define __CLI_MENU__CONSOLE_HPP__

#include "color.hpp"

namespace cli_menu {

  enum CONSOLE_CODE {
    CONSOLE_ERROR, CONSOLE_WARNING,
    CONSOLE_HINT, CONSOLE_CORRECT, CONSOLE_CANCEL
  };

  typedef const CONSOLE_CODE& CR_CONSOLE_CODE;

  class Console {
  private:
    static constexpr int totalStatus = 5;

    inline static Color colors[totalStatus] = {
      Color::RED,
      Color::YELLOW,
      Color::BLUE,
      Color::GREEN,
      Color::ORANGE
    };

  public:
    inline static char boundaryCharacter = '-';
    inline static size_t boundaryCharactersAmount = 45;
    inline static std::string listPointStyle = ">";
    inline static bool outlineStyle = true;

    // edit the 'colors'
    static void setColor(
      CR_CONSOLE_CODE code,
      CR_CLR color_in
    );

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

    /** SPECIALS */

    // always has 2 newlines between the boundary
    static void logBoundaryLine();

    static void logResponse(
      CR_CONSOLE_CODE code,
      mt::CR_STR reason
    );
  };
}

#endif // __CLI_MENU__CONSOLE_HPP__