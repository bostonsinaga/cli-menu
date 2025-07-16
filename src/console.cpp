#ifndef __CLI_MENU__CONSOLE_CPP__
#define __CLI_MENU__CONSOLE_CPP__

#include "console.hpp"

namespace cli_menu {

  Color Console::colors[Console::totalStatus] = {
    Color::RED,
    Color::YELLOW,
    Color::WHITE,
    Color::AZURE,
    Color::GREEN,
    Color::ORANGE
  };

  void Console::setColor(
    const CONSOLE_CODE &code,
    CR_CLR color_in
  ) {
    Console::colors[code] = color_in;
  }

  /** NORMAL */

  void Console::logString(
    mt::CR_STR text,
    CR_CLR foreground
  ) {
    std::cout << Color::getString(
      text, foreground
    );
  }

  void Console::logString(
    mt::CR_STR text,
    CR_CLR foreground,
    CR_CLR background
  ) {
    std::cout << Color::getString(
      text, foreground, background
    );
  }

  /** ITALIC */

  void Console::logItalicString(mt::CR_STR text) {
    std::cout << Color::getItalicString(text);
  }

  void Console::logItalicString(
    mt::CR_STR text,
    CR_CLR foreground
  ) {
    std::cout << Color::getItalicString(
      text, foreground
    );
  }

  void Console::logItalicString(
    mt::CR_STR text,
    CR_CLR foreground,
    CR_CLR background
  ) {
    std::cout << Color::getItalicString(
      text, foreground, background
    );
  }

  /** UNDERLINE */

  void Console::logUnderlineString(mt::CR_STR text) {
    std::cout << Color::getUnderlineString(text);
  }

  void Console::logUnderlineString(
    mt::CR_STR text,
    CR_CLR foreground
  ) {
    std::cout << Color::getUnderlineString(
      text, foreground
    );
  }

  void Console::logUnderlineString(
    mt::CR_STR text,
    CR_CLR foreground,
    CR_CLR background
  ) {
    std::cout << Color::getUnderlineString(
      text, foreground, background
    );
  }

  /** SPECIALS */

  void Console::logBoundaryLine() {
    std::cout << std::endl
      << std::string(boundaryCharactersAmount, boundaryCharacter)
      << std::endl;
  }

  void Console::logResponse(
    const CONSOLE_CODE &code,
    mt::CR_STR reason
  ) {
    Console::logString(
      listPointStyle + ' ' + reason,
      Console::colors[code]
    );
  }
}

#endif // __CLI_MENU__CONSOLE_CPP__