#ifndef __CLI_MENU__MESSAGE_HPP__
#define __CLI_MENU__MESSAGE_HPP__

#include <atomic>
#include "color.hpp"
#include "language.hpp"

namespace cli_menu {

  // to select the color of a string or tag word
  enum MESSAGE_ENUM {
    MESSAGE_HINT, MESSAGE_WARNING, MESSAGE_ERROR,
    MESSAGE_SUCCEED, MESSAGE_CANCELED
  };

  typedef const MESSAGE_ENUM& CR_MESSAGE_ENUM;

  class Message {
  private:
    static constexpr int statusCodeCount = 5;

    // use an atomic boolean to signal an interrupt
    inline static std::atomic<bool> INTERRUPTED_CTRL_C = false;

    inline static colors[statusCodeCount] = {
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
      CR_MESSAGE_ENUM statusCode,
      mt::CR_STR reason
    );

    /** SETTERS */

    // to prevent infinite loop after pressing 'ctrl+c'
    static void setInterruptedCtrlC();

    // check if interrupted before waiting for input
    static bool isInterruptedCtrlC();

    // decorated input interface
    static bool setDialogInput(std::string &buffer);

    // outline style by default
    static void setAsFilledStyle() { outlineStyle = false; }

    // edit the 'colors'
    static void setColor(
      CR_MESSAGE_ENUM statusCode,
      CR_CLR color_in
    );
  };
}

#endif // __CLI_MENU__MESSAGE_HPP__