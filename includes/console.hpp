#ifndef __CLI_MENU__CONSOLE_HPP__
#define __CLI_MENU__CONSOLE_HPP__

#include <atomic>
#include "color.hpp"

namespace cli_menu {

  class Console {
  public:
    enum CODE {
      CANCELED, ERROR, HINT, SUCCEED, WARNING, 
    };

  private:
    static constexpr int totalStatus = 5;

    // use an atomic boolean to signal an interrupt
    inline static std::atomic<bool> INTERRUPTED_CTRL_C = false;

    inline static colors[totalStatus] = {
      Color::BLUE,
      Color::YELLOW,
      Color::RED,
      Color::GREEN,
      Color::ORANGE
    };

  public:
    inline static char boundaryCharacter = '-';
    inline static size_t boundaryCharactersAmount = 45;
    inline static std::string listPointStyle = ">";
    inline static bool outlineStyle = true;

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
      const CODE &code,
      mt::CR_STR reason
    );

    // decorated input interface
    static bool cinDialogInput(std::string &buffer);

    /** SETTERS */

    // to prevent infinite loop after pressing 'ctrl+c'
    static void setInterruptedCtrlC();

    // check if interrupted before waiting for input
    static bool isInterruptedCtrlC();

    // edit the 'colors'
    static void setColor(
      const CODE &code,
      CR_CLR color_in
    );
  };
}

#endif // __CLI_MENU__CONSOLE_HPP__